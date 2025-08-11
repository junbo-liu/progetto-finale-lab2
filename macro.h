#pragma once
#define _XOPEN_SOURCE 700   // per ftruncate
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> // exit
#include <threads.h>
#include <string.h>
#include <mqueue.h>
#include <stdatomic.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "type.h"

#define SCALL_ERROR -1
#define SCALL(r, c, e) do { if ((r = c) == SCALL_ERROR) { perror(e); exit(EXIT_FAILURE); } } while (0)
#define SNCALL(r, c, e) do { if ((r = c) == NULL) { perror(e); exit(EXIT_FAILURE); } } while (0)
#define MQCALL(c, e) do { if ((c) == SCALL_ERROR) { perror(e); exit(EXIT_FAILURE); } } while (0)
#define FCLOSECALL(file, e) do { if (fclose(file) != 0) { perror(e); } file = NULL; } while (0)
#define MMAPCALL(r, c, e) do { if ((r = c) == MAP_FAILED) { perror(e); exit(EXIT_FAILURE); } } while (0)
#define THRDCALL(r, c, e) do { if ((r = c) != thrd_success) { perror(e); exit(EXIT_FAILURE); } } while (0)