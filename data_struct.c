#include "scall.h"
#include "data_struct.h"

env_t *env;

// una hash table per i tipi di rescuers
rescuer_node_t *all_rescuers[RESCUER_T_NUM];

int hFunction(char *name){
    return (atoi(name))%RESCUER_T_NUM;
}

void rescuer_insect(rescuer_type_t *rescuer){
    int key = hFunction(rescuer->rescuer_type_name);
    rescuer_node_t *node;
    SNCALL(node, (rescuer_node_t*)malloc(sizeof(rescuer_node_t)), "malloc node");
    node->head = rescuer;
    node->next = all_rescuers[key];
    all_rescuers[key] = node;
}

rescuer_type_t* rescuer_search(char *type){
    return all_rescuers[hFunction(type)]->head;
}

// una hash table per emergency type
emergency_node_t *all_emerg_type[EMERGENCY_T_NUM];
void emergency_insect(emergency_type_t *emerg){
    int key = hFunction(emerg->emergency_desc);

    emergency_node_t *new_node;
    SCALL(new_node, (emergency_node_t*)malloc(sizeof(emergency_node_t)), "malloc new emergency node");
    new_node->head = emerg;
    new_node->next = all_emerg_type[key];
    all_emerg_type[key] = new_node;
}

emergency_type_t *emergency_search(char *type){
    return all_emerg_type[hFunction(type)]->head;
}