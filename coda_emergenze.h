#pragma once
#include "macro.h"
#include "data_struct.h"

typedef struct nodo_emergenze
{
    emergency_t *head;
    struct nodo_emergenze *next;

}nodo_emergenze_t;

typedef struct coda_emergenze
{
    nodo_emergenze_t *front; // punta alla testa della coda
    nodo_emergenze_t *rear;  // punta alla fine della coda
    sem_t mutex; 
    sem_t num_elem;
} coda_emergenze_t;

void free_emergenza(emergency_t *emerg);

void inizializza_coda_emergenze(coda_emergenze_t *p_coda_emerg);

void free_coda_emergenze(coda_emergenze_t *p_coda_emerg);

void push_in_coda_emergenze(coda_emergenze_t *p_coda_emerg, emergency_t *emerg);

emergency_t* get_from_coda_emergenze(coda_emergenze_t *p_coda_emerg);