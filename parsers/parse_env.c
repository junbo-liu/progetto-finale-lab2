#include "../scall.h"
#include "../data_struct.h"
#include "parser.h"

void parse_env() {
    FILE* file;

    SNCALL(file, fopen("config/env.conf", "r "), "errore durante open");
    
    SNCALL(env, (env_t*)malloc(sizeof(env_t)), "malloc env");
    SNCALL(env->queue, (char*)malloc(100 * sizeof(char)), "malloc queue name");

    if (fscanf(file, "queue=%[^\n]\nheight=%d\nwidth=%d", env->queue, &env->height, &env->width) != 3) {
        fprintf(stderr, "Errore nel parsing di env.conf\n");
        exit(1);
    }

    printf("%s %d %d \n", env->queue, env->height, env->width);
    
    
}