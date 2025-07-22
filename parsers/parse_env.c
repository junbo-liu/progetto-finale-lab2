#include "../scall.h"
#include "../data_struct.h"
#include "parser.h"

void parse_env() {
    FILE *file;
    char name[30];
    SNCALL(file, fopen("config/env.conf", "r "), "Errore durante fopen di env.conf");
    
    SNCALL(env, (env_t*)malloc(sizeof(env_t)), "Errore malloc env");

    if (fscanf(file, "queue=%[^\n]\nheight=%d\nwidth=%d", name, &env->height, &env->width) != 3) {
        fprintf(stderr, "Errore nel parsing di env.conf\n");
        exit(1);
    }
    
    SNCALL(env->queue, (char*)malloc((strlen(name) + 2) * sizeof(char)), "Errore malloc queue name");
    env->queue[0] = '/';
    env->queue = strcat(env->queue, name);
    printf("%s %d %d \n", env->queue, env->height, env->width);
    FCLOSECALL(file);
}