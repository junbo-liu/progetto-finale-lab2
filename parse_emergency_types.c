#include "macro.h"
#include "data_struct.h"
#include "parser.h"
#include "type.h"
#include "file_log.h"
#define LENGTH_LINE 200
#define LENG_TYPE_EMERG 20

#define DEBUG
void parse_emergency_types(emergency_type_t **emergency_types, int *n_emergency_types, rescuer_type_t **rescuer_types, int n_rescuer_types, file_log_t *file_log){
    FILE *file;
    SNCALL(file, fopen("config/emergency_types.conf", "r"), "Errore durante fopen di emergency_types.conf");

    scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "il file emergency_types.conf è aperto e inizia il parser");

    // variabili ausiliari
    char line[LENGTH_LINE] = "", tp_socc[LENG_TYPE_EMERG] = "", tp_emg[LENG_TYPE_EMERG] = "", lis_socc[LENGTH_LINE] = "";
    int prty, n, t, emergency_type_capacity = 0, counter = 0;
    char *tok;

    // lettura per riga
    while (fgets(line, LENGTH_LINE, file))
    {
        counter++;
        scrivi_sul_file_log_lettura_di_riga(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", counter, line);
        line[strcspn(line, "\n")] = '\0';
        #ifdef DEBUG
            printf("%s\n", line);
        #endif
        if (sscanf(line, "[%[^]]] [%d] %[^\n]", tp_emg, &prty, lis_socc) == 3)
        {
            // controlliamo il nome di emergenza non sia vuoto
            if (strcmp(tp_emg, "") == 0)
            {
                scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "Errore del formato: nome di emergenza");
                exit(EXIT_FAILURE);
            }
            // controlliamo che la priorità sia compreso tra 0 e 2
            if (prty < 0 || prty > 2)
            {
                scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "Errore del formato: priorità deve essere compreso tre 0 e 2");
                exit(EXIT_FAILURE);
            }
            // controlliamo che la lista dei soccorritori non sia vuota
            if (strcmp(lis_socc, "") == 0)
            {
                scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "Errore del formato: elenco dei soccorritori è vuota");
                exit(EXIT_FAILURE);
            }
            
            #ifdef DEBUG
                printf("tipo di emg %s con priorità %d\trest %s\n", tp_emg, prty, lis_socc);
            #endif
            // calcolo numero di tipi soccorritori necessari
            int i=0, n_rescuer_types_req=0;
            while (lis_socc[i] != '\0')
            {
                if (lis_socc[i] == ';')
                {
                    n_rescuer_types_req++;
                }
                i++;
            }
            tok = strtok(lis_socc, ";");
            
            rescuer_request_t *soccorritori;
            SNCALL(soccorritori, (rescuer_request_t*)malloc(n_rescuer_types_req*sizeof(rescuer_request_t)), "Errore malloc soccorritori");
            // estraiamo i dati dall'elenco dei tipi di soccorritori 
            i=0;
            int n_rescuers_req = 0;
            for ( i = 0; i < n_rescuer_types_req; i++)
            {
                if (sscanf(tok, "%[^:]:%d,%d;", tp_socc, &n, &t) == 3)
                {
                    // controlliamo il nome di soccorritore non sia vuoto
                    if (strcmp(tp_socc, "") == 0)
                    {
                        scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "Errore del formato: nome di soccorritore");
                        exit(EXIT_FAILURE);
                    }
                    // controlliamo il numero di soccorritore non sia <= 0
                    if (n <= 0)
                    {
                        scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "Errore del formato: numero di soccorritore");
                        exit(EXIT_FAILURE);
                    }
                    // controlliamo il tempo di gestione di soccorritore non sia <= 0
                    if (t <= 0)
                    {
                        scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "Errore del formato: tempo di gestione di soccorritore");
                        exit(EXIT_FAILURE);
                    }
                    // cerchiamo se esista il tipo di soccorritore nell'array rescuer_types e numero di istanze
                    int idx = -1, num = 0;
                    for (int k = 0; k < n_rescuer_types; k++) {
                        if (strcmp((*rescuer_types)[k].rescuer_type_name, tp_socc) == 0) {
                            num += (*rescuer_types)[k].n_instance;
                            idx = k;
                            // printf("k = %d e num = %d\n\n", k, num);
                        }
                    }

                    // controlliamo se tipo trovato
                    if (idx == -1)
                    {
                        scrivi_sul_file_log_IDstring_trova_tipo(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", tp_socc, "questo tipo non trovato");
                        exit(EXIT_FAILURE);
                    }

                    // controlliamo se il numero di istanze sia sufficiente per configurare
                    if (num < n)
                    {
                        scrivi_sul_file_log_IDstring_trova_tipo(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", tp_socc, "numero di istanze insufficiente per configurare");
                        exit(EXIT_FAILURE);
                    }
                    soccorritori[i].type = &(*rescuer_types)[idx];
                    #ifdef DEBUG
                    printf("trovato tipo nome %s, speed %d, x %d, y %d\n", soccorritori[i].type->rescuer_type_name, soccorritori[i].type->speed, soccorritori[i].type->x, soccorritori[i].type->y);
                    #endif
                    
                    n_rescuers_req += n;
                    soccorritori[i].required_count = n;
                    soccorritori[i].time_to_manage = t;
                }
                else
                {
                    scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSE_EMERGENCY_TYPES", "Errore del sintassi dell'elenco");
                    exit(EXIT_FAILURE);
                }
                
                tok = strtok(NULL, ";");
            }
            // controlliamo se c'è ancora spazio libero nel array emergency_types, in caso negativo facciamo una realloc
            if (*n_emergency_types >= emergency_type_capacity)
            {
                emergency_type_capacity += 10;
                SNCALL(*emergency_types, realloc(*emergency_types, emergency_type_capacity * sizeof(emergency_type_t)), "Errore realloc di emergency_types");
            }
            
            emergency_type_t *emergency = &(*emergency_types)[*n_emergency_types];
            (*n_emergency_types)++;
            // SNCALL(emergency, (emergency_type_t*)malloc(sizeof(emergency_type_t)), "Errore malloc emergency");
            SNCALL(emergency->emergency_desc, (char*)malloc((1+strlen(tp_emg)) * sizeof(char)), "Errore malloc di emergency_desc");
            strcpy(emergency->emergency_desc, tp_emg);
            emergency->priority = prty;
            
            emergency->rescuers = soccorritori;
            emergency->rescuer_types_req_number = n_rescuer_types_req;
            emergency->rescuers_req_number = n_rescuers_req;
        }
        // la riga non è rispettato alla sintassi BNF e viene rifiutata
        else 
            scrivi_sul_file_log_lettura_di_riga(file_log, "emergency_types.conf", "PARSE_EMERGENCY_TYPES", counter, "Errore del sintassi di questa riga e non viene memorizzata nella struttura dati");

    }
    scrivi_sul_file_log_IDstring(file_log, "emergency_types.conf", "PARSER_EMERGENCY_TYPES", "il parser di emergency_types.conf è completato");

    FCLOSECALL(file, "Errore durante fclose di emergency_types.conf");
}

void free_emergency_types(emergency_type_t **emergency_types, int n_emergency_types){
    if (emergency_types == NULL || *emergency_types == NULL) return;

    for (int i = 0; i < n_emergency_types; i++)
    {
        free((*emergency_types)[i].emergency_desc);
        (*emergency_types)[i].emergency_desc = NULL;
        free((*emergency_types)[i].rescuers);
        (*emergency_types)[i].rescuers = NULL;
    }
    
    free(*emergency_types);
    *emergency_types = NULL;

    return;
}