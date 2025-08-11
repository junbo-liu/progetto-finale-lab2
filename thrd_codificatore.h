#pragma once
#include "macro.h"
#include "type.h"
#include "file_log.h"
#include "coda_emergenze.h"

// 
typedef struct arg_thrd_codificatore
{
    int max_msg_size;
    mqd_t mq;
    coda_emergenze_t *coda_emergenze;
    int *max_tempo_attesa;
    env_t *dati_ambiente;
    rescuer_digital_twin_t **soccorritori;
    int num_soccorritori;
    emergency_type_t **tipi_emergenze;
    int num_tipi_emergenze;
    file_log_t *file_log;
    int id_request_emerg;
    mtx_t id_request_emerg_lock;
}arg_thrd_codificatore_t;

int codificatore(void *arg_thrd_codificatore);
