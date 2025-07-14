#include "scall.h"

#define RESCUER_T_NUM 20
typedef struct rescuer_node {
    rescuer_type_t *head;
    struct rescuer_node *next;
} rescuer_node_t;

// un hash table per i tipi di rescuers
rescuer_node_t *all_rescuers[RESCUER_T_NUM];

int hFunction(char *name){
    return (atoi(name))%RESCUER_T_NUM;
}

void rescuer_insect(rescuer_type_t *rescuer){
    int key=hFunction(rescuer->rescuer_type_name);
    rescuer_node_t *node;
    SNCALL(node, (rescuer_node_t*)malloc(sizeof(rescuer_node_t)), "malloc node");
    node->head = rescuer;
    node->next = all_rescuers[key];
    all_rescuers[key] = node;
}
rescuer_type_t* rescuer_search(char *name){
    return all_rescuers[hFunction(name)];
}

// extern rescuer_twin_t *all_twin;
// extern int twin_count;

