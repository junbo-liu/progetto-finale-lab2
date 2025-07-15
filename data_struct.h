#pragma once
#include "type.h"

#define RESCUER_T_NUM 20


extern env_t *env;


typedef struct rescuer_node {
    rescuer_type_t *head;
    struct rescuer_node *next;
} rescuer_node_t;

// un hash table per i tipi di rescuers
extern rescuer_node_t *all_rescuers[RESCUER_T_NUM];

int resc_hFunction(char *name);

void rescuer_insect(rescuer_type_t *rescuer);

rescuer_type_t* rescuer_search(char *name);

// una lista di priorità per emergency type
typedef struct emergency_node{
    emergency_type_t *head;
    struct emergency_node *next;
}emergency_node_t;

extern emergency_node_t *emerg_list;

void emergency_insect(emergency_type_t *emrg);

emergency_type_t *emergency_search();