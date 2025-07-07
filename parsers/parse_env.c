#include "../scall.h"

int main() {
    FILE* file;

    SNCALL(file, fopen("../config/env.conf", "r "), "errore durante open");
    char q[100];
    int h, w;
    
    if(fscanf(file, "queue=%[^\n]\nheight=%d\nwidth=%d", q, &h, &w))
    {
        printf("%s %d %d \n", q, h, w);
    }
    else 
    {
        printf("riga non valido");
    }
    
    
}