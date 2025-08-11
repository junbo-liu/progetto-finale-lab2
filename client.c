#include "macro.h"
#include "data_struct.h"
#include "type.h"

int main(int argc, char *argv[]){
    // variabile ausiliare
    int ret;

    // definire gli attributi e aprire message queue
    mqd_t coda;
    struct mq_attr attr;
    char shm_msg[SHM_SIZE] = "", msg[MAX_SIZE_MSG] = "";

    attr.mq_flags = 0;
    attr.mq_maxmsg = 30;
    attr.mq_msgsize = MAX_SIZE_MSG;
    attr.mq_curmsgs = 0;

    
    int fd_mem;
    SCALL(fd_mem, shm_open(SHRD_MEM_NAME, O_CREAT | O_RDONLY, 0644), "Errore shm_open");
    
    
    void *ptr_shrd;
    MMAPCALL(ptr_shrd, mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd_mem, 0), "Errore mmap");

    strcpy(shm_msg, (char*)ptr_shrd);
    
    
//   shrd_msg_t *ptr_shrd;
//   MMAPCALL(ptr_shrd, mmap(NULL, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd_mem, 0), "Errore mmap");

    // // ottengo il pid del server per mandare un segnale quando finisce 
//   strcpy(shm_msg, ptr_shrd->queue);
    // printf("pid del server %d\n", ptr_shrd->server_pid);
    // Questo modo mi da errore di Segmentation fault

    // chiusura della sharing memory
     SCALL(ret, munmap(ptr_shrd, SHM_SIZE), "Errore munmap");
    SCALL(ret, close(fd_mem), "Errore close fd_mem");
   
    printf("arriva %s\n", shm_msg);
    SCALL(coda, mq_open(shm_msg, O_CREAT | O_RDWR, 0644, &attr), "Errore mq open from client.c");
    
    if (argc == 5)
    {
        
        for (int i = 1; i < 4; i++)
        {
            strcat(msg, argv[i]);
            strcat(msg, " ");
        }
        
        strcat(msg, argv[4]);
        printf("invio: %s\n", msg);
        MQCALL(mq_send(coda, msg, strlen(msg), 0), "Errore mq send from client.c");
    }
    if (argc == 3 && strcmp(argv[1], "-f") == 0)
    {
        FILE *file;
        SNCALL(file, fopen(argv[2], "r"), "Errore fopen from client.c");
       
        
        while (fgets(msg, MAX_SIZE_MSG, file))
        {
            MQCALL(mq_send(coda, msg, strlen(msg), 0), "Errore mq send from client.c");
            printf("richiesta inviata: %s\n", msg);

        }
        FCLOSECALL(file, "Errore fclose from client");
    }
    else {
        fprintf(stderr, "Uso: ./client <nome> <x> <y> <delay> oppure ./client -f file.txt\n");
        exit(1);
    }
    // SCALL(ret, kill(ptr_shrd->server_pid, SIGUSR1), "Errore kill");
    SCALL(ret, mq_close(coda), "Errore mq_close");
    // SCALL(ret, )
    return 0;
}