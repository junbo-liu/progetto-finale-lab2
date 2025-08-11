#include "coda_emergenze.h"
#include "type.h"


void inizializza_coda_emergenze(coda_emergenze_t *p_coda_emerg){
    // variabile ausiliare
    int ret;

    // inizializziamo la coda
    p_coda_emerg->front = NULL;
    p_coda_emerg->rear = NULL;

    // inizializziamo i semafori
    SCALL(ret, sem_init(&p_coda_emerg->mutex, 0, 1), "Errore sem_init di mutex");
    SCALL(ret, sem_init(&p_coda_emerg->num_elem, 0, 0), "Errore sem_init di num_elem");
}

void free_emergenza(emergency_t *emerg){
    free(emerg);
}

void free_coda_emergenze(coda_emergenze_t *coda){
    int ret;

    // libero la coda se non è vuota
    while (coda->front != NULL)
    {
        free_coda_emergenze(get_from_coda_emergenze(coda));
    }
    
    SCALL(ret, sem_destroy(&coda->num_elem), "Errore sem_destroy di num_elem");
    SCALL(ret, sem_destroy(&coda->mutex), "Errore sem_destroy di mutex");
    coda = NULL;
}

// Inseriamo la nuova richiesta validata nella coda 
void push_in_coda_emergenze(coda_emergenze_t *p_coda_emerg, emergency_t *emerg){
    printf("\nemergenza da inserire: %s %d %d\n\n", emerg->type.emergency_desc, emerg->x, emerg->y);

    int ret;
    // creiamo un nuovo nodo da inserire nella coda
    nodo_emergenze_t *nodo;
    SNCALL(nodo, (nodo_emergenze_t*)malloc(sizeof(nodo_emergenze_t)), "Errore malloc di nodo_emergenze_t nodo");
    nodo->head = emerg;
    nodo->next = NULL;

    // accendiamo la sezione critica con il semaforo mutex
    SCALL(ret, sem_wait(&p_coda_emerg->mutex), "Errore sem_wait di mutex");
    if (p_coda_emerg->front = NULL)
    {
        p_coda_emerg->front = nodo;
        p_coda_emerg->rear = nodo;
    }
    else
    {
        p_coda_emerg->rear->next = nodo;
        p_coda_emerg->rear = nodo;
    }
    SCALL(ret, sem_post(&p_coda_emerg->mutex), "Errore sem_post di mutex");
    // chiamiamo sem_post su num_elem per segnalare che è stato aggiunto un nodo
    SCALL(ret, sem_post(&p_coda_emerg->num_elem), "Errore sem_post di mutex");
}

// estrarre un'emergenza alla volta dalla coda
emergency_t* get_from_coda_emergenze(coda_emergenze_t *p_coda_emerg){
   int ret;
    // aspettiamo che c'è un nodo nella coda
   SCALL(ret, sem_wait(&p_coda_emerg->num_elem), "Errore sem_wait si num_elem");
   SCALL(ret, sem_wait(&p_coda_emerg->mutex), "Errore sem_wait di mutex");

    // prendiamo il primo nodo 
    emergency_t *emerg = p_coda_emerg->front->head;
    
    
    // Avanza nella coda
    p_coda_emerg->front = p_coda_emerg->front->next;

    // Se la coda è ora vuota, azzera anche rear
    if (p_coda_emerg->front == NULL) {
        p_coda_emerg->rear = NULL;
    }
    SCALL(ret, sem_post(&p_coda_emerg->mutex), "Errore sem_post di mutex");

    return emerg;
}