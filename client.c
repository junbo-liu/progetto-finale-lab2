#include "scall.h"
#include "data_struct.h"
#include "parsers/parser.h"

void send_msg(char *type, int x, int y, int delay, mqd_t mq){
    emergency_request_t *msg;
    SNCALL(msg, (emergency_request_t*)malloc(sizeof(emergency_request_t)), "malloc msg strucct from client");

    strcpy(msg->emergency_name, type);
    msg->x = x;
    msg->y = y;
    msg->timestamp = time(NULL);
    sleep(delay);
    printf("%s %d %d %d\n", msg->emergency_name, msg->x, msg->y, delay);
    // MQCALL(mq_send(mq, msg, sizeof(msg), 0), "mq send from client");
    MQCALL(mq_send(mq, (char*)msg, sizeof(*msg), 0), "mq send from client");

}
int main(int argc, char *argv[]){
    
    parse_env();
    // definire gli attributi e aprire message queue
    mqd_t coda;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 30;
    attr.mq_msgsize = MAX_SIZE_MSG;
    attr.mq_curmsgs = 0;
    
    SCALL(coda, mq_open(env->queue, O_CREAT | O_RDWR, 0644, &attr), "Errore mq open");

    if (argc == 5)
    {
        send_msg(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), coda);
    }
    else if (argc == 3 && strcmp(argv[1], "-f") == 0)
    {
        FILE *file;
        SNCALL(file, fopen(argv[2], "r"), "fopen from client");
        char type[20];
        int x, y, delay;

        while (fscanf(file, "%s %d %d %d", type, &x, &y, &delay) == 4)
        {
            send_msg(type, x, y, delay, coda);
        }
        MQCALL(fclose(file), "fclose from client");
        return 0;
    }
    
    else {
        fprintf(stderr, "Uso: ./client <nome> <x> <y> <delay> oppure ./client -f file.txt\n");
        exit(1);
    }

}