#include "../scall.h"
#include "../data_struct.h"
#define _XOPEN_SOURCE 700 // per strdup
#define LENGTH_LINE 200
#define LENG_TYPE 20
void parse_emergency_types(){
    FILE *file;
    SNCALL(file, fopen("config/emergency_types.conf", "r"), "durante fopen");

    char line[LENGTH_LINE], tp_rs[LENG_TYPE], tp_emg[LENG_TYPE], res[LENGTH_LINE];
    int prty, n, t;
    char *tok;

    // inizializzo la tabella hash di emergency_type_t
    memset(all_emerg_type, 0, sizeof(all_emerg_type));   

    while (fgets(line, LENGTH_LINE, file))
    {
        emergency_type_t *emergency;
        SNCALL(emergency, (emergency_type_t*)malloc(sizeof(emergency_type_t)), "malloc");
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n",line);
        if (sscanf(line, "[%[^]]] [%d] %[^\n]", tp_emg, &prty, res) == 3)
        {
            printf("tipo di emg %s con priorità %d\trest %s\n", tp_emg, prty, res);
            emergency->emergency_desc=strdup(tp_emg);
            emergency->priority=prty;
            // printf("em prima %s \n",emergency->emergency_desc);
            // strcpy(tp_emg,"esa");
            // printf("%s",emergency->emergency_desc);
            
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
        SNCALL(soccorritori, (rescuer_request_t*)malloc(num_socc*sizeof(rescuer_request_t*)), "malloc");
        i=0;
        while (tok != NULL)
        {   
            sscanf(tok, "%[^:]:%d,%d;", tp_rs, &n, &t);
            printf("tipo %s n %d t %d\n", tp_rs, n, t);
            // SNCALL(soccorritori[i].type, (rescuer_type_t*)malloc(sizeof(type_t)), "malloc rescuertype");
            // soccorritori[i].type->rescuer_type_name=strdup(tp_rs);  per test
            soccorritori[i].type = rescuer_search(tp_rs);
            printf("trovato tipo %s\n", soccorritori[i].type->rescuer_type_name);
            
            soccorritori[i].required_count=n;
            soccorritori[i].time_to_manage=t;
            tok =strtok(NULL, ";");
            i++;
        }

        emergency->rescuers=soccorritori;
        emergency->rescuers_req_number=num_socc;
        
        #ifdef debug
        for (i = 0; i < num_socc; i++)
        {
            printf("soccorritori: tipo %s, num %d, time %d\n", soccorritori[i].type->rescuer_type_name, soccorritori[i].required_count, soccorritori[i].time_to_manage);
        }
        #endif
    }
    
}