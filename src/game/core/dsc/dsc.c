#include "include/game/core/dsc/dsc.h"

#include "include/game/core/dsc/opcode.h"

int create_dsc_script_from_file(dsc_script_t** out, const char* filePath)
{
    dsc_script_t* script = (dsc_script_t*)malloc(sizeof(dsc_script_t));
    if (NULL == script)
    {
        perror("failed to allocate a dsc script");
        return -1;
    }
    if (create_node_list(&script->timeElements) != 0)
    {
        perror("failed to allocate a time elements list in a dsc script");
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
    dsc_time_element_t* lastTimeElement;
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
                    dsc_time_element_t* element = (dsc_time_element_t*)malloc(sizeof(dsc_time_element_t));
                    if (NULL == element)
                    {
                        // TODO: free script struct
                        perror("failed to allocate a time element");
                        fclose(dscFile);
                        return -1;
                    }
                    element->time = lastReadTime;
                    lastTimeElement = element;
                    if (create_node_list(&element->targets) != 0)
                    {
                        // TODO: free script struct
                        perror("failed to allocate a time element");
                        fclose(dscFile);
                        return -1;
                    }
                    add_node_to_node_list(script->timeElements, element);
                    break;
                case DSC_OPCODE_TARGET:
                {
                    dsc_target_t* target = (dsc_target_t*)malloc(sizeof(dsc_target_t));
                    if (NULL == target)
                    {
                        // TODO: free script struct
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
                    add_node_to_node_list(lastTimeElement->targets, target);
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
    list_node_t* timeElementNode = script->timeElements->begin;
    while (NULL != timeElementNode)
    {
        dsc_time_element_t* timeElement = (dsc_time_element_t*)timeElementNode->data;
        list_node_t* targetNode = timeElement->targets->begin;
        while (NULL != targetNode) 
        {
            free(targetNode->data);
            targetNode = targetNode->next;
        }
        free_node_list(timeElement->targets);
        free(timeElementNode->data);
        timeElementNode = timeElementNode->next;
    }
    free_node_list(script->timeElements);
    free(script);
}