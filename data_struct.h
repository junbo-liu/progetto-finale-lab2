#pragma once
#include "type.h"
#define MAX_SIZE_MSG 1024      // max dimensione del messaggio da inserire in mq
#define STOP "fermati"
#define SHRD_MEM_NAME "/my_shrd_mem"
#define SHM_SIZE 100

#define NUM_PRIORITY 3

#define NUM_THRD_CODIFICATORI 10

#define NUM_THRD_GESTORE 30


// inizializza un array contiene il tempo massimo in cui l'emergenza può aspettare
void popola_max_tempo(int arr[]);

