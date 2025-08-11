#include "macro.h"
#include "data_struct.h"
#include "parser.h"
#include "type.h"
#include "file_log.h"

#define LENG_NAME 20
#define LENG_LINE 60
#define DEBUG


void parse_rescuers(rescuer_type_t **rescuer_types, int *n_rescuer_types, rescuer_digital_twin_t **rescuers, int *n_rescuers, env_t *data_env, file_log_t *file_log){
    FILE* file;

    SNCALL(file, fopen("config/rescuers.conf", "r"), "Errore durante fopen di rescuers.conf");
    
    scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "il file rescuers.conf è aperto e inizia il parser");

    // variabili ausiliari
    char line[LENG_LINE] = "", name[LENG_NAME] = "";
    int num, speed, x, y, rescuer_type_capacity = 0, rescuers_capacity = 0, counter = 0;
    
    // lettura per riga
    while (fgets(line, sizeof(line), file))
    {
        counter++;
        scrivi_sul_file_log_lettura_di_riga(file_log, "rescuers.conf", "PARSE_RESCUERS", counter, line);
        
        #ifdef DEBUG
            printf("%s", line);
        #endif
        line[strcspn(line, "\n")]='\0';
        if ((sscanf(line, "[%[^]]][%d][%d][%d;%d]", name, &num, &speed, &x, &y)) == 5)
        {
            // controlliamo il nome di soccorritore non sia vuoto
            if (strcmp(name, "") == 0)
            {
                scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "Errore del formato: nome di soccorritore");
                exit(EXIT_FAILURE);
            }
            // controlliamo che ci sia almeno uno
            if (num <= 0)
            {
                scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "Errore del formato: numero di soccorritori");
                exit(EXIT_FAILURE);
            }
            // controlliamo che la velocità non sia 0
            if (speed <= 0)
            {
                scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "Errore del formato: velocità di soccorritori");
                exit(EXIT_FAILURE);
            }
            // controlliamo che l'ascissa sia nel range
            if (x < 0 || x > data_env->width)
            {
                scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "Errore del formato: ascissa di soccorritori");
                exit(EXIT_FAILURE);
            }
            
            // controlliamo che l'ordinata sia nel range
            if (y < 0 || y > data_env->height)
            {
                scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "Errore del formato: l'ordinata di soccorritori");
                exit(EXIT_FAILURE);
            }
            
            #ifdef DEBUG
                printf("nome %s, num %d, speed %d, x %d, y %d\n", name, num, speed, x, y);
            #endif
            // controlliamo se c'è ancora spazio libero nel array rescuer_types, in caso negativo facciamo una realloc
            if (*n_rescuer_types >= rescuer_type_capacity)
            {
                rescuer_type_capacity += 5;    // aggiungiamo 5 celle alla volta
                SNCALL(*rescuer_types, realloc(*rescuer_types, rescuer_type_capacity * sizeof(rescuer_type_t)), "Errore realloc di rescuer_types");
            }
        
            rescuer_type_t *rescuer = &(*rescuer_types)[*n_rescuer_types];
            (*n_rescuer_types)++;
            // SNCALL(rescuer, (rescuer_type_t*)malloc(sizeof(rescuer_type_t)), "Errore malloc rescuer");
            SNCALL(rescuer->rescuer_type_name, (char*)malloc((1+strlen(name))*sizeof(char)), "Errore malloc rescuer type name");
            strcpy(rescuer->rescuer_type_name, name);
            rescuer->n_instance = num;
            rescuer->speed = speed;
            rescuer->x = x;
            rescuer->y = y;

            rescuers_capacity += num;      // aggiungiamo num celle alla volta
            SNCALL(*rescuers, realloc(*rescuers, rescuers_capacity * sizeof(rescuer_digital_twin_t)), "Errore realloc di rescuers");
                
            
            for (int i = 0; i < num; i++)
            {
            rescuer_digital_twin_t *dtwin =&(*rescuers)[*n_rescuers];
                // SNCALL(dtwin, (rescuer_digital_twin_t*)malloc(sizeof(rescuer_digital_twin_t)), "Errore malloc dtwin");
                
                dtwin->id = i + 1;
                dtwin->rescuer = rescuer;
                dtwin->status = IDLE;
                dtwin->x = 0;                   // all'inizio si trova in (0,0)
                dtwin->y = 0;

                (*n_rescuers)++;

                // digital_twin_insect(dtwin);
            }
        }
        // la riga non è rispettato alla sintassi BNF e viene rifiutata
        else 
            scrivi_sul_file_log_lettura_di_riga(file_log, "rescuers.conf", "PARSE_RESCUERS", counter, "Errore del sintassi di questa riga e non viene memorizzata nella struttura dati");

    }
    scrivi_sul_file_log_IDstring(file_log, "rescuers.conf", "PARSER_RESCUERS", "il parser di rescuers.conf è completato");

    FCLOSECALL(file, "Errore durante fclose di rescuers.conf");
}

void free_rescuers(rescuer_type_t **rescuer_types, int n_rescuer_types, rescuer_digital_twin_t **rescuers) {
    if (rescuer_types == NULL || *rescuer_types == NULL) return;

    for (int i = 0; i < n_rescuer_types; i++) {
        free((*rescuer_types)[i].rescuer_type_name); 
        (*rescuer_types)[i].rescuer_type_name = NULL;
    }

    free(*rescuer_types); 
    *rescuer_types = NULL;
    free(*rescuers);
    *rescuers = NULL;

    return;
}
