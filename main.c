#include "scall.h"
#include "data_struct.h"
#include "parsers/parser.h"

int main(){
    parse_rescuers();
    // parse_emergency_types();

    // definire gli attributi e aprire message queue
    attr.mq_flags = 0;
    attr.mq_maxmsg = 30;
    attr.mq_msgsize = MAX_SIZE_MSG;
    attr.mq_curmsgs = 0;

    SCALL(coda, mq_open(env->queue, O_CREAT | O_RDWR, &attr), "mq open");

    return 0;
}