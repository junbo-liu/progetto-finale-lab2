#include "macro.h"
#include "data_struct.h"
#include "type.h"
#define DEBUG
// env_t *env;

// rescuer_type_t **all_rescuer_types = NULL;
// int rescuer_type_count = 0;
// void rescuer_insect(rescuer_type_t *rescuer){
//     static int rescuer_type_capacity = 0;
//     if (rescuer_type_count >= rescuer_type_capacity)  // devo aumentare la dimensione dell'array
//     {
//         rescuer_type_capacity += 10;
//         SNCALL(all_rescuer_types, realloc(all_rescuer_types, rescuer_type_capacity * sizeof(rescuer_type_t*)), "Errore realloc all_rescuer_types");
//     }
     
//     all_rescuer_types[rescuer_type_count] = rescuer;

//     #ifdef DEBUG
//     printf("inserito nome %s, speed %d, x %d, y %d\n", all_rescuer_types[rescuer_type_count]->rescuer_type_name, all_rescuer_types[rescuer_type_count]->speed, all_rescuer_types[rescuer_type_count]->x, all_rescuer_types[rescuer_type_count]->y);
//     #endif
//     rescuer_type_count++;
// }

// rescuer_type_t* rescuer_search(int *idx, int dim, int *num, rescuer_type_t **rescuer_types, char *type){
//     for (int i = *idx; i < dim; i++)
//     {
//         if (strcmp(rescuer_types[i]->rescuer_type_name, type) == 0)
//         {
//             (*idx) = i;
//             (*num) += rescuer_types[i]->n_instance;
//             return rescuer_types[i];
//         }
//     }
//     printf("rescuer type %s non trovato!", type);
//     return NULL;
// }


// rescuer_digital_twin_t **all_rescuers = NULL;
// int rescuer_count = 0;
// void digital_twin_insect(rescuer_digital_twin_t *rescuer){
//     static int rescuer_capacity = 0;
//     if (rescuer_count >= rescuer_capacity)
//     {
//         rescuer_capacity += 10;
//         SNCALL(all_rescuers, realloc(all_rescuers, rescuer_capacity * sizeof(rescuer_digital_twin_t*)), "Errore realloc all_rescuers");
//     }
    
//     all_rescuers[rescuer_count] = rescuer;
    
//     #ifdef DEBUG
//     printf("twin inserito %s ID %d\n", all_rescuers[rescuer_count]->rescuer->rescuer_type_name, all_rescuers[rescuer_count]->id);
//     #endif
//     rescuer_count++;
// }

// rescuer_digital_twin_t* digital_twin_search(rescuer_type_t *rescuer){
//     return NULL;
// }

// emergency_type_t **all_emergency_types = NULL;
// int emergency_type_count = 0;
// void emergency_insect(emergency_type_t *emerg){
//     static int emergency_type_capacity = 0;
//     if (emergency_type_count >= emergency_type_capacity) {
//         emergency_type_capacity += 10; 
//         SNCALL(all_emergency_types, realloc(all_emergency_types, emergency_type_capacity * sizeof(emergency_type_t*)), "Errore realloc emergency types");
//     }
    
//     all_emergency_types[emergency_type_count] = emerg;

//     #ifdef DEBUG
//     printf("inserito tipo %s priorità %d req_num %d\n", all_emergency_types[emergency_type_count]->emergency_desc, all_emergency_types[emergency_type_count]->priority, all_emergency_types[emergency_type_count]->rescuers_req_number);
//     #endif
//     emergency_type_count++;

// }

// emergency_type_t* emergency_search(char *type){
//     for (int i = 0; i < emergency_type_count; i++)
//     {
//         if (strcmp(all_emergency_types[i]->emergency_desc, type) == 0)
//         {
//             return all_emergency_types[i];
//         }
//     }
//     return NULL;
// }

// inizializza un array contiene il tempo massimo in cui l'emergenza può aspettare
void popola_max_tempo(int arr[]){
    arr[0] = -1;   // priorità 0 nessun limite
    arr[1] = 30;     // priorità 1 30 secondi
    arr[2] = 10;     // priorità 2 10 secondi
}