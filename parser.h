#include "type.h"
#include "file_log.h"

void parse_env(env_t *env, file_log_t *file_log);

void parse_rescuers(rescuer_type_t **all_rescuer_names, int *n_rescuer_names, rescuer_digital_twin_t **all_rescuers, int *n_rescuers, env_t *data_env, file_log_t *file_log);

void parse_emergency_types(emergency_type_t **emergency_types, int *n_emergency_types, rescuer_type_t **rescuer_types, int n_rescuer_types, file_log_t *file_log);

void free_env(env_t *env);

void free_rescuers(rescuer_type_t **rescuer_types, int n_rescuer_types, rescuer_digital_twin_t **rescuers);

void free_emergency_types(emergency_type_t **emergency_types, int n_emergency_types);