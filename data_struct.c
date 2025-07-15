#include "scall.h"
#include "data_struct.h"

env_t *env;

// un hash table per i tipi di rescuers
rescuer_node_t *all_rescuers[RESCUER_T_NUM];

int resc_hFunction(char *name){
    return (atoi(name))%RESCUER_T_NUM;
}

void rescuer_insect(rescuer_type_t *rescuer){
    int key=resc_hFunction(rescuer->rescuer_type_name);
    rescuer_node_t *node;
    SNCALL(node, (rescuer_node_t*)malloc(sizeof(rescuer_node_t)), "malloc node");
    node->head = rescuer;
    node->next = all_rescuers[key];
    all_rescuers[key] = node;
}

rescuer_type_t* rescuer_search(char *name){
    return all_rescuers[hFunction(name)]->head;
}

// una lista di priorità per emergency type
emergency_node_t *emerg_list;
void emergency_insect(emergency_type_t *emerg){
    emergency_node_t *new_node;
    SCALL(new_node, (emergency_node_t*)malloc(sizeof(emergency_node_t)), "malloc new emergency node");
    new_node->head = emerg;
    new_node->next = NULL;

    //inserimento in testa
    if (emerg_list->head == NULL || new_node->head->priority > emerg_list->head->priority)
    {
        new_node->next = emerg_list;
        emerg_list = new_node;
        return;
    }

    emergency_node_t *current_node = emerg_list;
    while (new_node->head->priority <= current_node->head->priority)
    {
        current_node = current_node->next;
    }
    new_node->next = current_node->next;
    current_node->next = new_node;
}

emergency_type_t *emergency_search();