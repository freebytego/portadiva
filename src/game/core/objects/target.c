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

    target->desiredSize.x = 30.0f;
    target->desiredSize.y = 30.0f;

    renderProperties.width = 0.0f;
    renderProperties.height = 0.0f;
    renderProperties.angle = 0.0f;
    renderProperties.center.x = renderProperties.width / 2.0f;
    renderProperties.center.y = renderProperties.height / 2.0f; 
    renderProperties.offsetType = RENDER_OFFSET_CENTER;
    renderProperties.offset.x = 0.0f; // todo: from config
    renderProperties.offset.y = 0.0f; // todo: from config

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
    position.y = dscTarget->y * 272 / 272000.0f; // TODO: figure out the positioning
    if (game_object_create(&target->object, "target", position, renderProperties, texturePart) != 0)
    {
        free(target);
        return -1;
    }
    
    target->createdAt = SDL_GetTicks() * 100; // targets don't know the chart start time offset
    target->flyingTime = flyingTime;
    target->finishingAt = target->createdAt + target->flyingTime * 100;
    target->finished = 0;
    target->progress = 0.0;
    target->dscTarget = dscTarget;

    game_object_set_implementation(target->object, target);
    game_object_set_cycle(target->object, (void (*)(void*))&game_target_cycle);
    game_object_set_render(target->object, (void (*)(void*))&game_target_render);
    game_object_set_free(target->object, (void (*)(void*))&game_target_free);

    game_target_needle_t* needle;
    if (game_target_needle_create(&needle, target) != 0)
    {
        game_object_free(target->object);
        free(target);
        return -1;
    }

    if (game_object_add_child(target->object, needle->object) != 0)
    {
        game_target_needle_free(needle);
        game_object_free(target->object);
        free(target);
        return -1;
    }

    target->needle = needle;
    target->real = NULL;
    target->doingAnimation = 1;

    target->animationTime = 0.25f * 1000 * 100;
    target->animationEndAt = target->createdAt + target->animationTime;

    *out = target;

    return 0;
}

double bezier(double t, double P0, double P1, double P2, double P3) {
    return (1 - t) * (1 - t) * (1 - t) * P0 +
           3 * (1 - t) * (1 - t) * t * P1 +
           3 * (1 - t) * t * t * P2 +
           t * t * t * P3;
}

void game_target_cycle(game_target_t* target)
{
    if (target->doingAnimation)
    {
        if (SDL_GetTicks() * 100 >= target->animationEndAt)
        {
            target->doingAnimation = 0;
            target->object->renderProperties.width = target->desiredSize.x;
            target->object->renderProperties.height = target->desiredSize.y;
        }
        else
        {
            double animationProgress = ((double)(SDL_GetTicks() * 100 - target->createdAt) / (double)(target->animationEndAt - target->createdAt));
            // i wanna have a non linear thing here
            target->object->renderProperties.width = bezier(animationProgress, 0.0, 0.6, 0.8, 1.0) * target->desiredSize.x;
            target->object->renderProperties.height = bezier(animationProgress, 0.0, 0.6, 0.8, 1.0) * target->desiredSize.y;
        }
    }

    target->progress = (double)(SDL_GetTicks() * 100 - target->createdAt) / (double)(target->finishingAt - target->createdAt);
    if (target->progress >= 1.0 && !target->finished)
    {
        target->finished = 1;
    }
}

void game_target_render(game_target_t* target)
{
    engine_generic_renderer_render(target->object);
    engine_generic_renderer_render(target->needle->object);
}

int game_target_create_target_real(game_target_t* target)
{
    game_target_real_t* real;
    if (game_target_real_create(&real, target) != 0)
    {
        game_target_needle_free(target->needle);
        game_object_free(target->object);
        free(target);
        return -1;
    }
    if (game_object_add_child(((game_rhythm_controller_t*)target->object->parent->implementation)->targetRealRenderer->object, real->object) != 0)
    {
        game_target_real_free(real);
        game_target_needle_free(target->needle);
        game_object_free(target->object);
        free(target);
        return -1;
    }

    target->real = real;

    return 0;
}

void game_target_free(game_target_t* target)
{
    game_target_real_free(target->real);
    game_target_needle_free(target->needle);
    game_object_free(target->object);
    free(target);
}
