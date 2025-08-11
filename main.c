#include "macro.h"
#include "data_struct.h"
#include "parser.h"
#include "coda_emergenze.h"
#include "file_log.h"
#include "thrd_codificatore.h"

volatile sig_atomic_t sig = 0;


int main(){
  // variabile ausiliare
  int ret;

  file_log_t file_log;
  inizializza_file_log(&file_log, "program_activity.log");


  // la seguente struttura dati dell'ambiente
  env_t env_data;

  rescuer_type_t *all_rescuer_types = NULL;
  int n_rescuer_types = 0;

  rescuer_digital_twin_t *all_rescuers = NULL;
  int n_rescuers = 0;

  emergency_type_t *all_emergency_types = NULL;
  int n_emergency_types = 0;
  
  parse_env(&env_data, &file_log);
  printf("fine parse_env\n\n");
  printf("queue: %s width: %d height: %d \n", env_data.queue, env_data.width, env_data.height);

  parse_rescuers(&all_rescuer_types, &n_rescuer_types, &all_rescuers, &n_rescuers, &env_data, &file_log);
  printf("fine parse_rescuer\n\n");

  parse_emergency_types(&all_emergency_types, &n_emergency_types, &all_rescuer_types, n_rescuer_types, &file_log);
  printf("fine parse_emergency\n\n\n");


  printf("i tipi di soccorritori configurati:\n\n");
  for (int i = 0; i < n_rescuer_types; i++)
  {
    printf("nome %s, n %d, speed %d, base (%d, %d)\n", all_rescuer_types[i].rescuer_type_name, all_rescuer_types[i].n_instance, all_rescuer_types[i].speed, all_rescuer_types[i].x, all_rescuer_types[i].y);
    
  }
  printf("\n\n");

  // printf("le istanze di soccorritori configurate:\n\n");
  // for (int i = 0; i < n_rescuers; i++)
  // {
  //   printf("id %d, nome %s, posizione (%d, %d), stato %d\n", all_rescuers[i].id, all_rescuers[i].rescuer->rescuer_type_name, all_rescuers[i].x, all_rescuers[i].y, all_rescuers[i].status);
  // }
  // printf("\n\n");

  printf("i tipi di emergenze configurati:\n\n");
  for (int i = 0; i < n_emergency_types; i++)
  {
    printf("nome %s, priorità %d, n. tipi di soccorritori %d, n. soccorritori %d\nl'elenco di soccorritori richiesti:\n", all_emergency_types[i].emergency_desc, all_emergency_types[i].priority, all_emergency_types[i].rescuer_types_req_number, all_emergency_types[i].rescuers_req_number);

    for (int j = 0; j < all_emergency_types[i].rescuer_types_req_number; j++)
    {
      printf("nome %s, n %d, tempo di gestione %d\n", all_emergency_types[i].rescuers[j].type->rescuer_type_name, all_emergency_types[i].rescuers[j].required_count, all_emergency_types[i].rescuers[j].time_to_manage);
    }
    printf("\n");
  }
  printf("\n\n");


  int fd_mem;
  SCALL(fd_mem, shm_open(SHRD_MEM_NAME, O_CREAT | O_RDWR, 0644), "Errore shm_open");
  SCALL(ret, ftruncate(fd_mem, SHM_SIZE), "Errore ftuncate");

  void *ptr_shrd;
  MMAPCALL(ptr_shrd, mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd_mem, 0), "Errore mmap");

  strcpy((char*)ptr_shrd, env_data.queue);

  // shrd_msg_t *ptr_shrd;
  // MMAPCALL(ptr_shrd, mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd_mem, 0), "Errore mmap");

  // strcpy(ptr_shrd->queue, env_data.queue);
  // ptr_shrd->server_pid = getpid();

  // definire gli attributi e aprire message queue
  mqd_t mq;
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = 30;
  attr.mq_msgsize = MAX_SIZE_MSG;
  attr.mq_curmsgs = 0;

  SCALL(mq, mq_open(env_data.queue, O_CREAT | O_RDWR, 0644, &attr), "Errore mq_open");

  // la seguente coda contiene le emergenze da gestire
  coda_emergenze_t coda_emerg;
  inizializza_coda_emergenze(&coda_emerg);
  
  // un array contiene il tempo massimo in cui l'emergenza può aspettare
  int max_tempo_attesa[NUM_PRIORITY];
  popola_max_tempo(max_tempo_attesa);


  // inizializziamo la struct in cui contiene gli argomenti da passare a thrd_codificatore
  arg_thrd_codificatore_t arg_thrd_codificatore;
  arg_thrd_codificatore.coda_emergenze = &coda_emerg;
  arg_thrd_codificatore.dati_ambiente = &env_data;
  arg_thrd_codificatore.file_log = &file_log;
  arg_thrd_codificatore.id_request_emerg = 1; // parte da 1
  mtx_init(&arg_thrd_codificatore.id_request_emerg_lock, mtx_plain);
  arg_thrd_codificatore.max_tempo_attesa = max_tempo_attesa;
  arg_thrd_codificatore.mq = mq;
  arg_thrd_codificatore.max_msg_size = MAX_SIZE_MSG;
  arg_thrd_codificatore.tipi_emergenze = &all_emergency_types;
  arg_thrd_codificatore.num_tipi_emergenze = n_emergency_types;
  arg_thrd_codificatore.soccorritori = &all_rescuers;
  arg_thrd_codificatore.num_soccorritori = n_rescuers;



  thrd_t codificatori[NUM_THRD_CODIFICATORI];

  for (int i = 0; i < NUM_THRD_CODIFICATORI; i++)
  {
    THRDCALL(ret, thrd_create(&codificatori[i], codificatore, &arg_thrd_codificatore), "Errore thrd_create del codificatore");
    // THRDCALL(ret, thrd_detach(codificatori[i]), "Errore thrd_detach del codificatore");
  }
  

  for (int i = 0; i < NUM_THRD_CODIFICATORI; i++)
  {
    thrd_t rit = thrd_join(codificatori[i], NULL);
    if (rit == thrd_success)
    {
      printf("thrd %d terminato con successo\n", i);
    }
    else
    {
      printf("thrd %d non terminato con successo\n", i);
    }
    
  }
  






  // chiusura della sharing memory
  SCALL(ret, munmap(ptr_shrd, SHM_SIZE), "Errore munmap");
  SCALL(ret, close(fd_mem), "Errore close fd_mem");
  SCALL(ret, shm_unlink(SHRD_MEM_NAME), "Errore shm_unlink");
  SCALL(ret, mq_close(mq), "Errore mq_close");
  SCALL(ret, mq_unlink(env_data.queue), "Errore mq_unlink");

  // liberiamo le strutture dati
  free_env(&env_data);
  free_rescuers(&all_rescuer_types, n_rescuer_types, &all_rescuers);
  free_emergency_types(&all_emergency_types, n_emergency_types);
  free_coda_emergenze(&coda_emerg);
  free_file_log(&file_log);
  return 0;
}