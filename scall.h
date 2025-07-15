#pragma once
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> // exit
#include <wait.h>
#include <threads.h>
#include <string.h>

#include "type.h"

#define SCALL_ERROR -1
#define SCALL(r, c, e) do { if ((r = c) == SCALL_ERROR) { perror(e); exit(EXIT_FAILURE); } } while (0)
#define SNCALL(r, c, e) do { if ((r = c) == NULL) { perror(e); exit(EXIT_FAILURE); } } while (0)
#define PARENT_CHILD(pid, parent, child) do {if (pid == 0) { child; } else { parent; } } while (0)
#define SCALLREAD(r, r_loop, w_loop, e) do { while ((r = r_loop) > 0) { w_loop; } if (r == SCALL_ERROR) { perror(e); exit(EXIT_FAILURE); } } while(0)