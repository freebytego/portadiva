#include "include/game/core/dsc/dsc.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "include/game/core/dsc/opcode.h"

int create_dsc_script_from_file(dsc_script_t** out, const char* filePath)
{
    dsc_script_t* script = (dsc_script_t*)malloc(sizeof(dsc_script_t));
    if (NULL == script)
    {
        perror("failed to allocate a dsc script");
        return -1;
    }
    if (create_node_list(&script->targets) != 0)
    {
        perror("failed to allocate a target list in a dsc script");
        return -1;
    }

    int32_t value;
    FILE* dscFile = fopen(filePath, "rb");
    if (dscFile == NULL)
    {
        perror("failed to read a file\n");
        return -1;
    }
    fseek(dscFile, 4, SEEK_CUR);

    int32_t lastReadTime = 0;

    while(fread(&value, sizeof(value), 1, dscFile) > 0)
    {
        // read parameters for opcode
        short parametersCount = get_parameters_count(value);
        if (parametersCount > 0)
        {
            int32_t* parametersList = (int32_t*)malloc(sizeof(int32_t) * parametersCount);
            if (NULL == parametersList)
            {
                perror("failed to allocate an array for opcode parameters");
                fclose(dscFile);
                return -1;
            }
            for (int i = 0; i < parametersCount; ++i) 
            {
                int32_t parameter;
                fread(&parameter, sizeof(parameter), 1, dscFile);
                parametersList[i] = parameter;
            }

            switch (value)
            {
                case DSC_OPCODE_TIME:
                    lastReadTime = parametersList[0];
                    break;
                case DSC_OPCODE_TARGET:
                {
                    dsc_target_t* target = (dsc_target_t*)malloc(sizeof(dsc_target_t));
                    if (NULL == target)
                    {
                        perror("failed to allocate a target");
                        fclose(dscFile);
                        return -1;
                    }
                    target->time = lastReadTime;
                    target->type = parametersList[0];
                    target->x = parametersList[1];
                    target->y = parametersList[3];
                    target->angle = parametersList[4];
                    target->amplitude = parametersList[5];
                    target->frequency = parametersList[6];
                    add_node_to_node_list(script->targets, target);
                    break;
                }
            }
            free(parametersList);
        }
        *out = script;
    }


    return 0;
}

void free_dsc_script(dsc_script_t* script)
{
    list_node_t* node = script->targets->begin;
    while (node != NULL)
    {
        free(node->data);
        node = node->next;
    }
    free_node_list(script->targets);
    free(script);
}