#include "include/game/core/objects/target.h"

int game_target_create(game_target_t** out)
{
    game_target_t* target = (game_target_t*)malloc(sizeof(game_target_t));
    if (NULL == target)
    {
        perror("failed to allocate a target");
        return -1;
    }
    if (game_object_create(&target->object, "target") != 0)
    {
        perror("failed to create a game object for target");
        free(target);
        return -1;
    }
    game_object_set_implementation(target->object, target);
    game_object_set_cycle(target->object, &game_target_cycle);

    *out = target;

    return 0;
}

void game_target_cycle(void* target)
{
    game_target_t* gameTarget = (game_target_t*)target;
    printf("GAME TARGET: %d - %s\n", gameTarget->object->id, gameTarget->object->name);
}

void game_target_free(game_target_t* target)
{
    game_object_free(target->object);
    free(target);
}