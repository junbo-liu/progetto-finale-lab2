#include "macro.h"
#include "data_struct.h"
#include "type.h"


// inizializza un array contiene il tempo massimo in cui l'emergenza può aspettare
void popola_max_tempo(int arr[]){
    arr[0] = -1;   // priorità 0 nessun limite
    arr[1] = 30;     // priorità 1 30 secondi
    arr[2] = 10;     // priorità 2 10 secondi
}