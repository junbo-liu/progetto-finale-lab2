#include "../scall.h"
#include "../data_struct.h"
#include "parser.h"
#define LENG_NAME 20
#define LENG_LINE 60
#define DEBUG
void parse_rescuers(){
    FILE* file;

    SNCALL(file, fopen("config/rescuers.conf", "r"), "Errore durante fopen di rescuers.conf");
    
    char line[LENG_LINE], name[LENG_NAME];
    int num, speed, x, y;
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        line[strcspn(line, "\n")]='\0';
        if ((sscanf(line, "[%[^]]][%d][%d][%d;%d]", name, &num, &speed, &x, &y)) == 5)
        {
            #ifdef DEBUG
                printf("nome %s, num %d, speed %d, x %d, y %d\n", name, num, speed, x, y);
            #endif
            rescuer_type_t *rescuer;
            SNCALL(rescuer, (rescuer_type_t*)malloc(sizeof(rescuer_type_t)), "Errore malloc rescuer");
            SNCALL(rescuer->rescuer_type_name, (char*)malloc((1+strlen(name))*sizeof(char)), "Errore malloc rescuer type name");
            strcpy(rescuer->rescuer_type_name, name);
            rescuer->speed = speed;
            rescuer->x = x;
            rescuer->y = y;
            // inserisco tipo di rescuer
            rescuer_insect(rescuer);

            rescuer_digital_twin_t *dtwin;
            for (int i = 0; i < num; i++)
            {
                SNCALL(dtwin, (rescuer_digital_twin_t*)malloc(sizeof(rescuer_digital_twin_t)), "Errore malloc dtwin");
                dtwin->id = i + 1;
                dtwin->rescuer = rescuer;
                dtwin->status = IDLE;
                dtwin->x = 0;                   // all'inzio si trova in (0,0)
                dtwin->y = 0;

                digital_twin_insect(dtwin);
            }
        }
        else printf("la riga non valido\n");

    }
    FCLOSECALL(file);
}