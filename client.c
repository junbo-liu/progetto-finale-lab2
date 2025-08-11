#include "macro.h"
#include "data_struct.h"
#include "type.h"
/*
void send_msg(char *type, int x, int y, int delay, mqd_t mq){
    emergency_request_t *emerg_ric;
    SNCALL(emerg_ric, (emergency_request_t*)malloc(sizeof(emergency_request_t)), "malloc emerg_ric struct from client");

    strcpy(emerg_ric->emergency_name, type);
    emerg_ric->x = x;
    emerg_ric->y = y;
    time(&emerg_ric->timestamp);
    sleep(delay);
    printf("invio: %s %d %d %d\n", emerg_ric->emergency_name, emerg_ric->x, emerg_ric->y, delay);
    MQCALL(mq_send(mq, (char*)emerg_ric, sizeof(emergency_request_t), 0), "mq send from client");

}
*/

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
    
    // // ottengo il pid del server per mandare un segnale quando finisce 
    // pid_t server_pid;
    // memcpy(&server_pid, (char*)ptr_shrd + strlen(shm_msg) + 1, sizeof(pid_t));
    // printf("pid del server %d\n", server_pid);
    // chiusura della sharing memory
     SCALL(ret, munmap(ptr_shrd, SHM_SIZE), "Errore munmap");
    SCALL(ret, close(fd_mem), "Errore close fd_mem");
   
    printf("arriva %s\n", shm_msg);
    SCALL(coda, mq_open(shm_msg, O_CREAT | O_RDWR, 0644, &attr), "Errore mq open from client.c");
    
    if (argc == 5)
    {
        // send_msg(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), coda);
        
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
        // char type[30];
        // int x, y, delay;

        // while (fscanf(file, "%s %d %d %d", type, &x, &y, &delay) == 4)
        // {
        //     send_msg(type, x, y, delay, coda);
        // }
        
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
    // SCALL(ret, kill(server_pid, SIGUSR1), "Errore kill");
    SCALL(ret, mq_close(coda), "Errore mq_close");
    // SCALL(ret, )
    return 0;
}