#pragma once
#include "type.h"
#define MAX_SIZE_MSG 1024      // max dimensione del messaggio da inserire in mq

extern FILE *log_file;

extern env_t *env;      // contiene nome della coda e dimensione della griglia

extern rescuer_type_t **all_rescuer_types;       // un array di rescuers type
extern int rescuer_type_count;                  // numero di rescuers type

void rescuer_insect(rescuer_type_t *rescuer);

rescuer_type_t* rescuer_search(char *type);


extern rescuer_digital_twin_t **all_rescuers;       // un array di digital twin
extern int rescuer_count;                         // numero di digital twin

void digital_twin_insect(rescuer_digital_twin_t *rescuer);
rescuer_digital_twin_t* digital_twin_search(rescuer_type_t *rescuer);


extern emergency_type_t **all_emergency_types;       // un array di emergencies type
extern int emergency_type_count;                  // numero di emergencies type

void emergency_insect(emergency_type_t *emrg);

emergency_type_t* emergency_search(char *type);