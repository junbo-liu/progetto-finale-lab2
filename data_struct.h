#pragma once
#include "type.h"

#define RESCUER_T_NUM 20       // dimensione del hash table rescuer_t
#define EMERGENCY_T_NUM 30     // dimensione del hash table rescuer_t
#define MAX_SIZE_MSG 1024      // max dimensione del messaggio da inserire in mq

extern env_t *env;      // contiene nome della coda e dimensione della griglia
extern mqd_t coda;
extern struct mq_attr attr;

// una hash table per i tipi di rescuers
typedef struct rescuer_node {
    rescuer_type_t *head;
    struct rescuer_node *next;
} rescuer_node_t;

extern rescuer_node_t *all_rescuers[RESCUER_T_NUM];

int hFunction(char *name);

void rescuer_insect(rescuer_type_t *rescuer);

rescuer_type_t* rescuer_search(char *type);

// 

// una hash table per emergency type
typedef struct emergency_node{
    emergency_type_t *head;
    struct emergency_node *next;
}emergency_node_t;

extern emergency_node_t *all_emerg_type[EMERGENCY_T_NUM];

void emergency_insect(emergency_type_t *emrg);

emergency_type_t *emergency_search(char *type);