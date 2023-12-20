#include "include/game/core/objects/target.h"

int game_target_create(game_target_t** out, dsc_target_t* dscTarget, int32_t flyingTime)
{
    game_target_t* target = (game_target_t*)malloc(sizeof(game_target_t));
    if (NULL == target)
    {
        fprintf(stderr, "failed to allocate a target");
        return -1;
    }

    render_properties_t renderProperties;
    renderProperties.width = 30.0f;
    renderProperties.height = 30.0f;
    renderProperties.offsetType = RENDER_OFFSET_CENTER;

    const char* type;
    switch (dscTarget->type)
    {
        case 0: case 4: case 18: type = "triangle_black"; break;
        case 1: case 5: case 19: type = "square_black"; break;
        case 2: case 6: case 20: type = "cross_black"; break;
        case 3: case 7: case 21: type = "circle_black"; break;
        case 12: case 14: type = "slide_left_black"; break;
        case 13: case 16: type = "slide_right_black"; break;
        default: type = "triangle_black"; break; // temp
    }

    texture_part_t* texturePart = texture_manager_find_texture_part_in_registered("textures", "buttons", type);
    
    SDL_FPoint position;
    position.x = dscTarget->x * 480 / 480000.0f;
    position.y = dscTarget->y * 272 / 272000.0f + 50; // TODO: figure out the positioning
    if (game_object_create(&target->object, "target", position, renderProperties, texturePart) != 0)
    {
        fprintf(stderr, "failed to create a game object for target");
        free(target);
        return -1;
    }
    
    target->createdAt = SDL_GetTicks() * 100; // targets don't know the chart start time offset
    target->flyingTime = flyingTime;
    target->finished = 0;

    game_object_set_implementation(target->object, target);
    game_object_set_cycle(target->object, &game_target_cycle);
    game_object_set_render(target->object, &game_target_render);

    *out = target;

    return 0;
}

void game_target_cycle(void* target)
{
    game_target_t* gameTarget = (game_target_t*)target;
    if (SDL_GetTicks() * 100 >= gameTarget->createdAt + gameTarget->flyingTime * 100 && !gameTarget->finished)
    {
        gameTarget->finished = 1;
    }
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