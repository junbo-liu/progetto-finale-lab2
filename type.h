#pragma once

#include <time.h>
//ambiente
typedef struct {
    char *queue;
    int height;
    int width;
} env_t;


// Stato dei soccorritori
typedef enum {
    IDLE, EN_ROUTE_TO_SCENE, ON_SCENE, RETURNING_TO_BASE
} rescuer_status_t;

// Tipologia di soccorritore
typedef struct {
    char *rescuer_type_name;
    int speed;
    int x;  // base x
    int y;  // base y
    int n_instance;
} rescuer_type_t;

// Istanza soccorritore (digital twin)
typedef struct {
    int id;
    int x;
    int y;
    rescuer_type_t *rescuer;
    rescuer_status_t status;
} rescuer_digital_twin_t;


// Richiesta di soccorso da parte di un'emergenza
typedef struct {
    rescuer_type_t *type;
    int required_count;
    int time_to_manage;
} rescuer_request_t;

// Tipo di emergenza
typedef struct {
    short priority;
    char *emergency_desc;
    rescuer_request_t *rescuers;
    int rescuers_req_number;
    int rescuer_types_req_number;
} emergency_type_t;

// Richiesta grezza dalla coda
#define EMERGENCY_NAME_LENGTH 64
typedef struct {
    char emergency_name[EMERGENCY_NAME_LENGTH];
    int x, y;
    time_t timestamp;
} emergency_request_t;

// Istanza attiva di un'emergenza
typedef enum {
    WAITING, ASSIGNED, IN_PROGRESS, PAUSED, COMPLETED, CANCELED, TIMEOUT
} emergency_status_t;

typedef struct {
    emergency_type_t type;
    emergency_status_t status;
    int x, y;
    int id;
    time_t time;
    int rescuer_count;
    rescuer_digital_twin_t *rescuers_dt;
} emergency_t;