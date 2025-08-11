#include "thrd_gestore.h"
#include "macro.h"
#include "type.h"
#include "coda_emergenze.h"
#include "file_log.h"

// la seguente funzione calcola il tempo di percorrenza di soccorritori e restituisce il massimo tra i tempi di percorrenza 
float calcolo_tempo(emergency_t *emerg){ 
    float max_tempo_di_arrivo = -1, tempo;

    for (int i = 0; i < emerg->type.rescuer_types_req_number; i++)
    {
        // estraiamo i dati del soccorritore
        float x_socc = emerg->type.rescuers[i].type->x;
        float y_socc = emerg->type.rescuers[i].type->y;
        float speed_socc = emerg->type.rescuers[i].type->speed;

        // calcoliamo il tempo di percorrenza
        tempo =(abs(emerg->x - x_socc) + abs(emerg->y - y_socc)) / speed_socc;
        if (tempo > max_tempo_di_arrivo)
        {
            max_tempo_di_arrivo = tempo;
        }
    }


    return max_tempo_di_arrivo;
      

}

// la seguente funzione calcola il numero di soccorritori disponibili
void cerca_soccorritori(emergency_t *emerg, rescuer_digital_twin_t **soccorritori, int *arr_idx, int *num_disponibili){

    int idx_soccorritori = 0;

    for (int i = 0; i < emerg->type.rescuer_types_req_number; i++)
    {
        
        for (int j = 0; j < emerg->type.rescuers[i].required_count; j++)
        {
            if (strcmp((*soccorritori)[idx_soccorritori].rescuer->rescuer_type_name, emerg->type.rescuers[i].type->rescuer_type_name) == 0)
            {
                if ((*soccorritori)[idx_soccorritori].status == IDLE )
                {
                    arr_idx[*num_disponibili] = idx_soccorritori;
                    (*num_disponibili)++;
                }
                idx_soccorritori++;
            }
            
        }
        
    }
    
    return;
}

int gestore(void *p_arg_thrd_gestore){

    arg_thrd_gestore_t *arg = (arg_thrd_gestore_t*)p_arg_thrd_gestore;
    
    // variabili ausiliare
    int stato_pre, stato_succ;

    while (1)
    {
        emergency_t *emergenza_da_gestire = get_from_coda_emergenze(arg->coda_emergenze);

        if (emergenza_da_gestire->id == -1)
        {
            break;
        }

        int priorita = emergenza_da_gestire->type.priority;

        stato_pre = emergenza_da_gestire->status;
        emergenza_da_gestire->status = ASSIGNED;
        stato_succ = emergenza_da_gestire->status;

        scrivi_sul_file_log_cambio_di_emerg(arg->file_log, emergenza_da_gestire->id, "EMERGENCY_STATUS", stato_pre, stato_succ);

        time_t now;
        float tempo_di_percorrenza = calcolo_tempo(emergenza_da_gestire);
        time(&now);
        // vediamo se c'è abbastanza tempo sufficienti per gestione
        if ((emergenza_da_gestire->time -now) + tempo_di_percorrenza > arg->max_tempo_attesa[priorita])
        {
            emergenza_da_gestire->status = TIMEOUT;
            scrivi_sul_file_log_TIMEOUT(arg->file_log, emergenza_da_gestire->id, "TIMEOUT", "Tempo di gestione insufficiente");
            
            // dialloco la struttura emergency_t
            free_emergenza(emergenza_da_gestire);
            continue;
        }


        int *idx_disponibili, num_disponibile = 0, num_socc_necessari = emergenza_da_gestire->rescuer_count;

        // il seguente array memorizza gli indici dei soccorritori disponibili dell'array dei soccorritori
        SNCALL(idx_disponibili, (int*)malloc(num_socc_necessari * sizeof(int)), "Errore malloc di idx_disponibili");
        mtx_lock(&arg->soccorritori_lock);
        // 
        cerca_soccorritori(emergenza_da_gestire, arg->soccorritori, idx_disponibili, &num_disponibile);

        mtx_unlock(&arg->soccorritori_lock);

        // vediamo se c'è abbastanza soccorritori per gestione
        if (num_disponibile < num_socc_necessari)
        {
            emergenza_da_gestire->status = TIMEOUT;
            scrivi_sul_file_log_TIMEOUT(arg->file_log, emergenza_da_gestire->id, "TIMEOUT", "Non ci sono abbastanza soccorritori disponibili per gestire questa emergenza");

            // dialloco l'array di indici e la struttura emergency_t
            free(&idx_disponibili);
            free_emergenza(emergenza_da_gestire);
            continue;
        }

        // procediamo con assegnazione dei soccorritori e creiamo i thread per ognuno di essi


    }
    
    

}