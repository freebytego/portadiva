#include <stdio.h>
#include "include/game/core/dsc/dsc.h"

int main(int argc, char** argv) {

    if (argc != 2)
    {
        printf("example: %s <path to .dsc file>", argv[0]);
        return 0;
    }

    dsc_script_t* script;
    if (create_dsc_script_from_file(&script, argv[1]) != 0)
    {
        perror("fail");
        return -1;
    }

    list_node_t* node = script->targets->begin;
    while (node != NULL)
    {
        dsc_target_t* target = ((dsc_target_t*)node->data);
        printf("target at %d : %d, %d, %d, %d, %d, %d, %d\n", target->time, target->type, target->x, target->y, target->angle, target->distance, target->amplitude, target->frequency);
        node = node->next;
    }

    free_dsc_script(script);

    return 1;
}