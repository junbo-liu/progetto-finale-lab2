#pragma once
#include "macro.h"
typedef struct file_log
{
    FILE *file_log;
    mtx_t lock;
} file_log_t;

void inizializza_file_log(file_log_t *file_log_struct, char *file_name);

void free_file_log(file_log_t *file_log_struct);

void scrivi_sul_file_log_IDstring(file_log_t *file_log_struct, char *id, char *evento, char *msg);

void scrivi_sul_file_log_IDint(file_log_t *file_log_struct, int id, char *evento, char *msg);

void scrivi_sul_file_log_lettura_di_riga(file_log_t *file_log_struct, char *id, char *evento, int riga, char *msg);

void scrivi_sul_file_log_IDstring_trova_tipo(file_log_t *file_log_struct, char *id, char *evento, char *token, char *msg);

void scrivi_sul_file_log_IDint_trova_tipo(file_log_t *file_log_struct, int id, char *evento, char *token, char *msg);

void scrivi_sul_file_log_mq_receive(file_log_t *file_log_struct, int id, char *evento, char *tipo, int x, int y, long t);

void scrivi_sul_file_log_cambio_di_emerg(file_log_t *file_log_struct, int id, char *evento, int stato_pre, int stato_succ);

void scrivi_sul_file_log_cambio_di_socc(file_log_t *file_log_struct, int id, char *evento, int stato_pre, int stato_succ);

void scrivi_sul_file_log_TIMEOUT(file_log_t *file_log_struct, int id, char *evento, char* msg);