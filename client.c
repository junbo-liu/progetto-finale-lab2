#include "scall.h"
#include "data_struct.h"
#include "type.h"

void send_msg(char *type, int x, int y, int delay, mqd_t mq){
    emergency_request_t *msg;

    printf("%s %d %d %d", type, x, y, delay);
    strcpy(msg->emergency_name, type);
    msg->x = x;
    msg->y = y;

    MQCALL(mq_send(mq, msg, sizeof(msg), 0), "mq send from client");
}
int main(int argc, char *argv[]){
    if (argc == 5)
    {
        send_msg(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), coda);
        return 0;
    }
    if (argc == 3 && strcmp(argv[1], "-f") == 0)
    {
        FILE *file;
        SNCALL(file, fopen(argv[2], "r"), "fopen from client");
        char type[20];
        int x, y, delay;

        if (fscanf(file, "%s %d %d %d", type, &x, &y, &delay) == 4)
        {
            send_msg(type, x, y, delay, coda);
        }
        
    }
    
}