#include "macro.h"
#include "data_struct.h"
#include "parser.h"
#include "type.h"
#include "file_log.h"
#define DIM_BUFF 30

void parse_env(env_t *env, file_log_t *file_log) {
    FILE *file;
    char *ret, buffer[DIM_BUFF] = "", q_name[DIM_BUFF] = " g ";
    SNCALL(file, fopen("config/env.conf", "r "), "Errore durante fopen di env.conf");
    scrivi_sul_file_log_IDstring(file_log, "env.conf", "PARSER_ENV", "il file env.conf è aperto e inizia il parser");
    
    //------------------------PRIMA RIGA-------------------------
    SNCALL(ret, fgets(buffer, DIM_BUFF, file), "errore nella lettura della prima riga del file env.conf");
    if (sscanf(buffer, "queue=%s", q_name) != 1)  //cioè non corrisponde
    {
        scrivi_sul_file_log_lettura_di_riga(file_log, "env.conf", "PARSE_ENV", 1, "Errore del formato di queue");
        exit(EXIT_FAILURE);
    }
    scrivi_sul_file_log_lettura_di_riga(file_log, "env.conf", "PARSE_ENV", 1, buffer);

    SNCALL(env->queue, (char*)malloc((strlen(q_name) + 2) * sizeof(char)), "Errore malloc queue ");
    env->queue[0] = '/';
    strcpy(env->queue + 1, q_name);

    //------------------------SECONDA RIGA-------------------------
    SNCALL(ret, fgets(buffer, DIM_BUFF, file), "errore nella lettura della seconda riga del file env.conf");
    if (sscanf(buffer, "height=%d", &env->height) != 1 || env->height <= 0)  //cioè non corrisponde
    {
        scrivi_sul_file_log_lettura_di_riga(file_log, "env.conf", "PARSE_ENV", 2, "Errore del formato di height");
        exit(EXIT_FAILURE);
    }
    scrivi_sul_file_log_lettura_di_riga(file_log, "env.conf", "PARSE_ENV", 2, buffer);

    //------------------------TERZA RIGA-------------------------
    SNCALL(ret, fgets(buffer, DIM_BUFF, file), "errore nella lettura della terza riga del file env.conf");
    if (sscanf(buffer, "width=%d", &env->width) != 1 || env->width <= 0)  //cioè non corrisponde
    {
        scrivi_sul_file_log_lettura_di_riga(file_log, "env.conf", "PARSE_ENV", 3, "Errore del formato di width");
        exit(EXIT_FAILURE);
    }
    scrivi_sul_file_log_lettura_di_riga(file_log, "env.conf", "PARSE_ENV", 3, buffer);
    scrivi_sul_file_log_IDstring(file_log, "env.conf", "PARSER_ENV", "il parser di env.conf è completato");
    //    printf("%s %d %d \n", env->queue, env->height, env->width);

    FCLOSECALL(file, "Errore durante fclose di env.conf");
}

void free_env(env_t *env){
    free(env->queue);
    env->queue = NULL;
}