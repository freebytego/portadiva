#ifndef DSC_H
#define DSC_H

#include "include/core/list.h"
#include "include/game/core/dsc/target.h"

typedef struct {
    list_t* targets;
} dsc_script_t;

int create_dsc_script_from_file(dsc_script_t** out, const char* filePath);
void free_dsc_script(dsc_script_t* script);

#endif