#include "thrd_codificatore.h"
#include "macro.h"
#include "coda_emergenze.h"
#include "file_log.h"

#define DEBUG


int convalidare(emergency_request_t *emerg_richiesta, emergency_type_t **tipi_emergenze, int num_tipi_emergenze, int* max_tempo_attesa, env_t *data_env, file_log_t *file_log, int id, int id_msg) {
    
        // cerchiamo se c'è questo tipo di emergenza nell'array
        int idx = -1;
        for (int i = 0; i < num_tipi_emergenze; i++)
        {
            if (strcmp((*tipi_emergenze)[i].emergency_desc, emerg_richiesta->emergency_name) == 0)
            {
                idx = i;
                break;
            }
        }
        // verifichiamo se il tipo trovato
        if (idx == -1)
        {
            scrivi_sul_file_log_IDint_trova_tipo(file_log, id_msg, "CODIFICATORE", emerg_richiesta->emergency_name, "questo tipo non trovato");
            return -1;
        }
        // controlliamo che l'ascissa sia nel range
        if (emerg_richiesta->x < 0 || emerg_richiesta->x > data_env->width)
        {
            scrivi_sul_file_log_IDint_trova_tipo(file_log, id_msg, "CODIFICATORE", emerg_richiesta->emergency_name, "l'ascissa fuori range");
            return -1;
        }
        // controlliamo che l'ordinata sia nel range
        if (emerg_richiesta->y < 0 || emerg_richiesta->y > data_env->height)
        {
            scrivi_sul_file_log_IDint_trova_tipo(file_log, id_msg, "CODIFICATORE", emerg_richiesta->emergency_name, "l'ordinata fuori range");
            return -1;
        }
        // controlliamo l'intervallo di tempo fra timestamp e now sia minore di massimo tempo che un'emergenza può aspettare
        time_t now = time(NULL);
        int priority = (*tipi_emergenze)[idx].priority;
        printf("\n[CODIFICATORE ID %d] differenza %ld max tempo %d\n\n", id, now - emerg_richiesta->timestamp, max_tempo_attesa[priority]);
        if (priority > 1)
        {
            if ((now - emerg_richiesta->timestamp) > max_tempo_attesa[priority])
            {
                scrivi_sul_file_log_IDint_trova_tipo(file_log, id_msg, "CODIFICATORE", emerg_richiesta->emergency_name, "il tempo è superato");
                return -1;
            }
        }
        
        return idx;
}


int codificatore(void *arg_thrd_codificatore){

    arg_thrd_codificatore_t *arg = (arg_thrd_codificatore_t*)arg_thrd_codificatore;
    char buffer[arg->max_msg_size], emerg_name[20] = "";
    int x, y, delay;

    thrd_t id = thrd_current();
    printf("sono thread CODIFICATORE %ld\n\n", id);

    while (1)
    {
        ssize_t bytes;
        SCALL(bytes, mq_receive(arg->mq, buffer, MAX_SIZE_MSG, NULL), "Errore mq_receve");

        if (strcmp(buffer, STOP) == 0)
        {
            break;
        }

        mtx_lock(&arg->id_request_emerg_lock);
            int id_req_msg = arg->id_request_emerg;
            arg->id_request_emerg++;
        mtx_unlock(&arg->id_request_emerg_lock);

        #ifdef DEBUG
            printf("[CODIFICATORE ID %ld] emergenza %d ricevuta %s\n", id, id_req_msg, buffer);
        #endif

        if (sscanf(buffer, "%s %d %d %d", emerg_name, &x, &y, &delay) != 4){
            scrivi_sul_file_log_IDint_trova_tipo(arg->file_log, id_req_msg, "MQ_RECEIVE", buffer, "Errore di sintassi della richiesta e viene annullato");
        }

        scrivi_sul_file_log_mq_receive(arg->file_log, id_req_msg, "MQ_RECEIVE", emerg_name, x, y, delay);
        emergency_request_t emerg_richiesta;
        strcpy(emerg_richiesta.emergency_name, emerg_name);
        emerg_richiesta.timestamp = time(NULL);
        emerg_richiesta.x = x;
        emerg_richiesta.y = y;

        // sleep di delay_secs secondi
        struct timespec ts = { .tv_sec = delay, .tv_nsec = 0};
        thrd_sleep(&ts, NULL);

        // convalidiamo la richiesta
        int esito = convalidare(&emerg_richiesta, arg->tipi_emergenze, arg->num_tipi_emergenze, arg->max_tempo_attesa, arg->dati_ambiente, arg->file_log, id, id_req_msg);

        #ifdef DEBUG
            printf("[CODIFICATORE ID %ld] esito di %d è %d\n", id, id_req_msg, esito);
        #endif

        // controlliamo l'esito
        if (esito == -1)
        {
            scrivi_sul_file_log_IDint_trova_tipo(arg->file_log, id_req_msg, "CODIFICATORE", emerg_name, "Richiesta non convalidata");
        }
        else
        {
            scrivi_sul_file_log_IDint_trova_tipo(arg->file_log, id_req_msg, "CODIFICATORE", emerg_name, "Richiesta convalidata");
            emergency_t *emergenza_da_gestire;
            SNCALL(emergenza_da_gestire, (emergency_t*)malloc(sizeof(emergency_t)), "Errore malloc di emergenza_da_gestire");
            emergenza_da_gestire->type = (*arg->tipi_emergenze)[esito];
            emergenza_da_gestire->rescuer_count = (*arg->tipi_emergenze)[esito].rescuers_req_number;
            emergenza_da_gestire->status = WAITING;
            emergenza_da_gestire->time = emerg_richiesta.timestamp;
            emergenza_da_gestire->x = emerg_richiesta.x;
            emergenza_da_gestire->y = emerg_richiesta.y;
            emergenza_da_gestire->id = id_req_msg;
            // emergenza_da_gestire->rescuers_dt li assegno durante il thrd_gestore

            push_in_coda_emergenze(arg->coda_emergenze, emergenza_da_gestire);
        }
        
    }
    return thrd_success;
}
