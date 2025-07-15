#include "../scall.h"
#include "../data_struct.h"
#include "parser.h"
#define LENG_NAME 20
#define LENG_LINE 60
void parse_rescuers() 
// int main() 
{
    FILE* file;

    SNCALL(file, fopen("config/rescuers.conf", "r"), "errore durante open");
    
    memset(all_rescuers, 0, sizeof(all_rescuers));
    char line[LENG_LINE], name[LENG_NAME];
    int num, speed, x, y;
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        line[strcspn(line, "\n")]='\0';
        if ((sscanf(line, "[%[^]]][%d][%d][%d;%d]", name, &num, &speed, &x, &y)) == 5)
        {
            printf("nome %s, num %d, speed %d, x %d, y %d\n", name, num, speed, x, y);
            rescuer_type_t *rescuer;
            SNCALL(rescuer, (rescuer_type_t*)malloc(sizeof(rescuer_type_t)), "malloc");
            SNCALL(rescuer->rescuer_type_name, (char*)malloc((1+strlen(name))*sizeof(char)), "malloc");
            strcpy(rescuer->rescuer_type_name, name);
            rescuer->speed = speed;
            rescuer->x = x;
            rescuer->y = y;
            // inserisco tipo di rescuer
            rescuer_insect(rescuer);
            

            rescuer_digital_twin_t *dit;
            SNCALL(dit, (rescuer_digital_twin_t*)malloc(num*sizeof(rescuer_digital_twin_t)), "malloc");
            for (int i = 0; i < num; i++)
            {
                dit[i].id = i;
                dit[i].rescuer = rescuer;
                dit[i].status = IDLE;
                dit[i].x = 0;                   // all'inzio si trova in (0,0)
                dit[i].y = 0;
            }
        }
        else printf("la riga non valido\n");
        
    }
    fclose(file);
    
}
        