#ifndef DSC_H
#define DSC_H

#include "include/core/list.h"

#include "include/game/core/dsc/target.h"
#include "include/game/core/dsc/opcode.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t time;
    int32_t flyingTime;
    list_t* targets; // dsc_target_t
} dsc_time_element_t;

typedef struct {
    // some metadata here i guess
    list_t* timeElements; // dsc_time_element_t
} dsc_script_t;

int dsc_script_create_from_file(dsc_script_t** out, const char* filePath);
void dsc_time_element_free(dsc_time_element_t* element);
void dsc_script_free(dsc_script_t* script);

#endif