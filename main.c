#include "scall.h"
#include "data_struct.h"
#include "parsers/parser.h"

int main(){
    FILE *log_file;
    SNCALL(log_file, fopen("program_activity.log", "a"), "Errore fopen log file");

    parse_env();
    printf("fine parse_env\n\n");
    parse_rescuers();
    printf("fine parse_rescuer\n\n");
    parse_emergency_types();
    printf("fine parse_emergency\n\n");

    // definire gli attributi e aprire message queue
    mqd_t coda;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 30;
    attr.mq_msgsize = MAX_SIZE_MSG;
    attr.mq_curmsgs = 0;
    
    SCALL(coda, mq_open(env->queue, O_CREAT | O_RDWR, 0644, &attr), "Errore mq open");

    emergency_request_t *em;
    ssize_t bytes;

    SCALL(bytes, mq_receive(coda, em, MAX_SIZE_MSG, NULL), "Errore mq receve");
    printf("emergenza ricevuta %s $d %d %d\n", em->emergency_name, em->x, em->y, em->timestamp);

    FCLOSECALL(log_file);
    return 0;
}