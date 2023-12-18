#include "include/game/core/objects/target.h"

int game_target_create(game_target_t** out)
{
    game_target_t* target = (game_target_t*)malloc(sizeof(game_target_t));
    if (NULL == target)
    {
        perror("failed to allocate a target");
        return -1;
    }

    SDL_Point position;
    position.x = 100;
    position.y = 70;

    render_properties_t renderProperties;
    renderProperties.width = 30.0f;
    renderProperties.height = 30.0f;
    renderProperties.offsetType = RENDER_OFFSET_CENTER;

    if (game_object_create(&target->object, "target", position, renderProperties) != 0)
    {
        perror("failed to create a game object for target");
        free(target);
        return -1;
    }
    game_object_set_implementation(target->object, target);
    game_object_set_cycle(target->object, &game_target_cycle);
    game_object_set_render(target->object, &game_target_render);

    *out = target;

    return 0;
}

float dir = -0.04f;

void game_target_cycle(void* target)
{
    game_target_t* gameTarget = (game_target_t*)target;
    gameTarget->object->renderProperties.width += dir;
    if (gameTarget->object->renderProperties.width <= 0) dir = 0.04f;
    else if (gameTarget->object->renderProperties.width >= 30) dir = -0.04f;
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