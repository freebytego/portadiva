#include "include/game/core/objects/target.h"

int game_target_create(game_target_t** out, SDL_FPoint position)
{
    game_target_t* target = (game_target_t*)malloc(sizeof(game_target_t));
    if (NULL == target)
    {
        perror("failed to allocate a target");
        return -1;
    }

    render_properties_t renderProperties;
    renderProperties.width = 16.0f;
    renderProperties.height = 16.0f;
    renderProperties.offsetType = RENDER_OFFSET_TOP_LEFT;

    if (game_object_create(&target->object, "target", position, renderProperties) != 0)
    {
        perror("failed to create a game object for target");
        free(target);
        return -1;
    }
    printf("Target core object at %p\n", target->object);
    game_object_set_implementation(target->object, target);
    game_object_set_cycle(target->object, &game_target_cycle);
    game_object_set_render(target->object, &game_target_render);

    *out = target;

    return 0;
}

void game_target_cycle(void* target)
{
    game_target_t* gameTarget = (game_target_t*)target;
}

void game_target_render(void* target)
{
    game_target_t* gameTarget = (game_target_t*)target;
    engine_generic_renderer_render(gameTarget->object);
}

void game_target_free(game_target_t* target)
{
    game_object_free(target->object);
    free(target);
}