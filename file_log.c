#include "file_log.h"
#include "macro.h"


//I seguenti sono array ausiliari per scrivere sul file di log gli stati come stringhe
char* arr_stati_emerg[7] = {"WAITING", "ASSIGNED", "IN_PROGRESS", "PAUSED", "COMPLETED", "CANCELED", "TIMEOUT"};
char* arr_stati_ist_socc[4] = {"IDLE", "EN_ROUTE_TO_SCENE", "ON_SCENE", "RETURNING_TO_BASE"};

void inizializza_file_log(file_log_t *file_log_struct, char *file_name){
    
    SNCALL(file_log_struct->file_log, fopen(file_name, "w+"), "Errore durante fopen di log_file");
    mtx_init(&file_log_struct->lock, mtx_plain);
}

void free_file_log(file_log_t *file_log_struct){
    
    FCLOSECALL(file_log_struct->file_log,"Errore durante fclose di log_file");
    mtx_destroy(&file_log_struct->lock);
}


void scrivi_sul_file_log_IDstring(file_log_t *file_log_struct, char *id, char *evento, char *msg){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';

    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%s] [%s] %s\n", time_str, id, evento, msg), "Errore fprint con IDstring");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_IDint(file_log_t *file_log_struct, int id, char *evento, char *msg){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%d] [%s] %s\n", time_str, id, evento, msg), "Errore fprint con IDint");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_lettura_di_riga(file_log_t *file_log_struct, char *id, char *evento, int riga, char *msg){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%s] [%s] LETTURA DI RIGA %d: %s\n", time_str, id, evento, riga, msg), "Errore fprint con lettura_di_riga");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}


void scrivi_sul_file_log_IDstring_trova_tipo(file_log_t *file_log_struct, char *id, char *evento, char *token, char *msg){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%s] [%s] %s: %s\n", time_str, id, evento, token, msg), "Errore fprint con IDstring_trova_tipo");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_IDint_trova_tipo(file_log_t *file_log_struct, int id, char *evento, char *token, char *msg){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%d] [%s] %s: %s\n", time_str, id, evento, token, msg), "Errore fprint con IDint_trova_tipo");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_mq_receive(file_log_t *file_log_struct, int id, char *evento, char *tipo, int x, int y, long t){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%d] [%s] %s %d %d %ld\n", time_str, id, evento, tipo, x, y, t), "Errore fprint con mq_receive");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_cambio_di_emerg(file_log_t *file_log_struct, int id, char *evento, int stato_pre, int stato_succ){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%d] [%s] emergenza cambia stato da %s a %s\n", time_str, id, evento, arr_stati_emerg[stato_pre], arr_stati_emerg[stato_succ]), "Errore fprint con cambio_di_emerg");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_cambio_di_socc(file_log_t *file_log_struct, int id, char *evento, int stato_pre, int stato_succ){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%d] [%s] soccorritore cambia stato da %s a %s\n", time_str, id, evento, arr_stati_ist_socc[stato_pre], arr_stati_ist_socc[stato_succ]), "Errore fprint con cambio_di_socc");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}

void scrivi_sul_file_log_TIMEOUT(file_log_t *file_log_struct, int id, char *evento, char* msg){
    int ret;
    mtx_lock(&file_log_struct->lock);
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    
    SCALL(ret, fprintf(file_log_struct->file_log, "[%s] [%d] [%s] soccorritore è andato in TIMEOUT per %s\n", time_str, id, evento, msg), "Errore fprint con TIMEOUT");
    fflush(file_log_struct->file_log);
    mtx_unlock(&file_log_struct->lock);
}