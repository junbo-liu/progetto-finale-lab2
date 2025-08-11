#pragma once
#include "macro.h"
#include "type.h"
#include "file_log.h"
#include "coda_emergenze.h"

typedef struct arg_thrd_gestore
{
    coda_emergenze_t *coda_emergenze;
    int *max_tempo_attesa;
    env_t *dati_ambiente;
    rescuer_digital_twin_t **soccorritori;
    mtx_t soccorritori_lock;
    int num_soccorritori;
    rescuer_type_t **tipi_soccorritori;
    file_log_t *file_log;
}arg_thrd_gestore_t;

int gestore(void *p_arg_thrd_gestore);