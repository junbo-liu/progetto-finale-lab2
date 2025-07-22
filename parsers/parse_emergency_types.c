#include "../scall.h"
#include "../data_struct.h"
#define LENGTH_LINE 200
#define LENG_TYPE 20
#define DEBUG
void parse_emergency_types(){
    FILE *file;
    SNCALL(file, fopen("config/emergency_types.conf", "r"), "Errore durante fopen di emergency_types.conf");

    char line[LENGTH_LINE], tp_rs[LENG_TYPE], tp_emg[LENG_TYPE], res[LENGTH_LINE];
    int prty, n, t;
    char *tok;

    while (fgets(line, LENGTH_LINE, file))
    {
        emergency_type_t *emergency;
        SNCALL(emergency, (emergency_type_t*)malloc(sizeof(emergency_type_t)), "Errore malloc emergency");
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n",line);
        if (sscanf(line, "[%[^]]] [%d] %[^\n]", tp_emg, &prty, res) == 3)
        {
            #ifdef DEBUG
                printf("tipo di emg %s con priorità %d\trest %s\n", tp_emg, prty, res);
            #endif
            SNCALL(emergency->emergency_desc, (char*)malloc((1+strlen(tp_emg)) * sizeof(char)), "Errore malloc emergency desc");
            strcpy(emergency->emergency_desc, tp_emg);
            emergency->priority=prty;
        }
        // calcolo numero di tipi soccorritori necessari
        int i=0, num_socc=0;
        while (res[i] !='\0')
        {
            if (res[i] == ';')
            {
                num_socc++;
            }
            i++;
        }
        printf("num socc %d\n", num_socc);
        tok = strtok(res, ";");
        emergency->rescuers_req_number=0;
        rescuer_request_t *soccorritori;
        SNCALL(soccorritori, (rescuer_request_t*)malloc(num_socc*sizeof(rescuer_request_t)), "Errore malloc soccorritori");
        i=0;
        while (tok != NULL)
        {   
            sscanf(tok, "%[^:]:%d,%d;", tp_rs, &n, &t);
            printf("tipo %s n %d t %d\n", tp_rs, n, t);
            // SNCALL(soccorritori[i].type, (rescuer_type_t*)malloc(sizeof(type_t)), "Errore malloc rescuertype");
            // soccorritori[i].type->rescuer_type_name=strdup(tp_rs);  per test
            soccorritori[i].type = rescuer_search(tp_rs);
            #ifdef DEBUG
            printf("trovato tipo nome %s, speed %d, x %d, y %d\n", soccorritori[i].type->rescuer_type_name, soccorritori[i].type->speed, soccorritori[i].type->x, soccorritori[i].type->y);
            #endif
            
            soccorritori[i].required_count=n;
            soccorritori[i].time_to_manage=t;
            tok = strtok(NULL, ";");
            i++;
        }

        emergency->rescuers = soccorritori;
        emergency->rescuers_req_number = num_socc;
        
        emergency_insect(emergency);        // inserisco nell'array
    }
    FCLOSECALL(file);
}