#include "include/game/core/objects/target_real.h"
#include "include/game/core/objects/target.h"
#include <math.h>

int game_target_real_create(game_target_real_t** out, game_target_t* target)
{
    game_target_real_t* targetReal = (game_target_real_t*)malloc(sizeof(game_target_real_t));
    if (NULL == targetReal)
    {
        fprintf(stderr, "failed to allocate a target needle");
        return -1;
    }

    render_properties_t renderProperties; // READ THIS FROM THE TEXTURE'S CONFIG!!!!
    renderProperties.width = 30.0f;
    renderProperties.height = 30.0f;
    renderProperties.angle = 0.0f;
    renderProperties.center.x = renderProperties.width / 2.0f;
    renderProperties.center.y = renderProperties.height / 2.0f;
    renderProperties.offset.x = 0.0f; // todo: from config
    renderProperties.offset.y = 0.0f; // todo: from config

    renderProperties.offsetType = RENDER_OFFSET_CENTER;

    const char* type;
    switch (target->dscTarget->type)
    {
        case 0: case 4: case 18: type = "triangle_real"; break;
        case 1: case 5: case 19: type = "square_real"; break;
        case 2: case 6: case 20: type = "cross_real"; break;
        case 3: case 7: case 21: type = "circle_real"; break;
        case 12: case 14: type = "slide_left_real"; break;
        case 13: case 16: type = "slide_right_real"; break;
        default: type = "triangle_real"; break;
    }

    texture_part_t* texturePart = texture_manager_find_texture_part_in_registered("textures", "buttons", type);
    
    if (game_object_create(&targetReal->object, "target real", target->object->position, renderProperties, texturePart) != 0)
    {
        fprintf(stderr, "failed to create a game object for target real");
        free(targetReal);
        return -1;
    }

    game_object_set_implementation(targetReal->object, targetReal);
    game_object_set_cycle(targetReal->object, (void (*)(void*))&game_target_real_cycle);
    game_object_set_free(targetReal->object, (void (*)(void*))&game_target_real_free);

    targetReal->target = target;
    targetReal->angle = target->dscTarget->angle / 1000.0f * 3.14f / 180.f;
    targetReal->distance = target->dscTarget->distance / 1000.f;
    targetReal->amplitude = (float)targetReal->target->dscTarget->amplitude;
    targetReal->frequency = (float)targetReal->target->dscTarget->frequency;
    if (fmod(targetReal->frequency, 2.0) != 0)
    {
        targetReal->frequency *= -1;
    }

    printf("%d %d\n", targetReal->target->dscTarget->frequency, targetReal->target->dscTarget->amplitude);

    *out = targetReal;

    return 0;
}

void game_target_real_cycle(game_target_real_t* targetReal)
{    
    double progress = 1.0 - targetReal->target->progress;
    float x = progress * targetReal->distance;
    float y = sin(progress * 3.14f * targetReal->frequency) / 36.0f * targetReal->amplitude;
    // float y = 0.f;

    targetReal->object->position.x = (sin(targetReal->angle) * x + cos(targetReal->angle) * y) + targetReal->target->object->position.x;
    targetReal->object->position.y = (cos(targetReal->angle) * -x - sin(targetReal->angle) * y) + targetReal->target->object->position.y;

    printf("%f | %d | %f %f | %f %f\n", progress, targetReal->amplitude, x, y, targetReal->object->position.x, targetReal->object->position.y);
}

void game_target_real_render(game_target_real_t* targetReal)
{
    engine_generic_renderer_render(targetReal->object);
}

void game_target_real_free(game_target_real_t* targetReal)
{
    game_object_free(targetReal->object);
    free(targetReal);
}