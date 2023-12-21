#include "include/game/core/dsc/dsc.h"

int dsc_script_create_from_file(dsc_script_t** out, const char* filePath)
{
    dsc_script_t* script = (dsc_script_t*)malloc(sizeof(dsc_script_t));
    if (NULL == script)
    {
        fprintf(stderr, "failed to allocate a dsc script");
        return -1;
    }
    if (node_list_create(&script->timeElements) != 0)
    {
        fprintf(stderr, "failed to allocate a time elements list in a dsc script");
        return -1;
    }

    int32_t value;
    FILE* dscFile = fopen(filePath, "rb");
    if (dscFile == NULL)
    {
        fprintf(stderr, "failed to read a file\n");
        return -1;
    }
    fseek(dscFile, 4, SEEK_CUR);

    int32_t lastReadTime = 0;
    int32_t lastFlyingTime = 0;
    dsc_time_element_t* lastTimeElement;
    while(fread(&value, sizeof(value), 1, dscFile) > 0)
    {
        // read parameters for opcode
        short parametersCount = opcode_get_parameters_count(value);
        if (parametersCount > 0)
        {
            int32_t* parametersList = (int32_t*)malloc(sizeof(int32_t) * parametersCount);
            if (NULL == parametersList)
            {
                fprintf(stderr, "failed to allocate an array for opcode parameters");
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
                {
                    lastReadTime = parametersList[0];
                    dsc_time_element_t* element = (dsc_time_element_t*)malloc(sizeof(dsc_time_element_t));
                    if (NULL == element)
                    {
                        // TODO: free script struct
                        fprintf(stderr, "failed to allocate a time element");
                        fclose(dscFile);
                        return -1;
                    }
                    element->time = lastReadTime;
                    element->flyingTime = lastFlyingTime;
                    lastTimeElement = element;
                    if (node_list_create(&element->targets) != 0)
                    {
                        // TODO: free script struct
                        fprintf(stderr, "failed to allocate a time element");
                        fclose(dscFile);
                        return -1;
                    }
                    node_list_add_node(script->timeElements, element);
                    break;
                }
                case DSC_OPCODE_TARGET_FLYING_TIME:
                {
                    lastFlyingTime = parametersList[0];
                    lastTimeElement->flyingTime = lastFlyingTime;
                    break;
                }
                case DSC_OPCODE_BAR_TIME_SET:
                {
                    lastFlyingTime = (int)((60.0f / (float)parametersList[0]) * (parametersList[1] + 1) * 1000.0f);
                    lastTimeElement->flyingTime = lastFlyingTime;
                    break;
                }
                case DSC_OPCODE_TARGET:
                {
                    dsc_target_t* target = (dsc_target_t*)malloc(sizeof(dsc_target_t));
                    if (NULL == target)
                    {
                        // TODO: free script struct
                        fprintf(stderr, "failed to allocate a target");
                        fclose(dscFile);
                        return -1;
                    }
                    target->time = lastReadTime;
                    target->type = parametersList[0];
                    target->x = parametersList[1];
                    target->y = parametersList[2];
                    target->angle = parametersList[3];
                    target->distance = parametersList[4];
                    target->amplitude = parametersList[5]; 
                    target->frequency = parametersList[6];
                    node_list_add_node(lastTimeElement->targets, target);
                    break;
                }
            }
            free(parametersList);
        }
    }

    *out = script;

    return 0;
}

void dsc_time_element_free(dsc_time_element_t* element)
{
    list_node_t* targetNode = element->targets->begin;
    while (NULL != targetNode) 
    {
        free(targetNode->data);
        targetNode = targetNode->next;
    }
    node_list_free(element->targets);
    free(element);
}

void dsc_script_free(dsc_script_t* script)
{
    list_node_t* timeElementNode = script->timeElements->begin;
    while (NULL != timeElementNode)
    {
        dsc_time_element_t* timeElement = (dsc_time_element_t*)timeElementNode->data;
        dsc_time_element_free(timeElement);
        timeElementNode = timeElementNode->next;
    }
    node_list_free(script->timeElements);
    free(script);
}