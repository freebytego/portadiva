#include "include/game/objects/target_real.h"
#include "include/game/objects/target.h"
#include <math.h>

int game_target_real_create(game_target_real_t** out, game_target_t* target)
{
    game_target_real_t* targetReal = (game_target_real_t*)malloc(sizeof(game_target_real_t));
    if (NULL == targetReal)
    {
        fprintf(stderr, "failed to allocate a target needle");
        return -1;
    }

    render_properties_t renderProperties = render_properties_create(30.0f, 30.0f, 0.0f, 0.0f, 0.0f, RENDER_OFFSET_CENTER, 1.0f, 250);
    const char* type;
    switch (target->dscTarget->type)
    {
        case 0: case 4: case 18: 
        {
            type = "triangle_real";
            targetReal->trailColor[0] = 0.0f;
            targetReal->trailColor[1] = 0.84f;
            targetReal->trailColor[2] = 0.6f;
            break;
        }
        case 1: case 5: case 19: 
        {
            type = "circle_real";
            targetReal->trailColor[0] = 0.95f;
            targetReal->trailColor[1] = 0.16f;
            targetReal->trailColor[2] = 0.46f;
            break;
        }
        case 2: case 6: case 20: 
        {
            type = "cross_real";
            targetReal->trailColor[0] = 0.10f;
            targetReal->trailColor[1] = 0.62f;
            targetReal->trailColor[2] = 0.96f;
            break;
        }
        case 3: case 7: case 21:
        {
            type = "square_real";
            targetReal->trailColor[0] = 0.96f;
            targetReal->trailColor[1] = 0.30f;
            targetReal->trailColor[2] = 0.8f;
            break;
        }
        case 12: case 14:
        {
            type = "slide_left_real";
            targetReal->trailColor[0] = 0.99f;
            targetReal->trailColor[1] = 0.75f;
            targetReal->trailColor[2] = 0.0f;
            break;
        }
        case 13: case 16: 
        {
            type = "slide_right_real";
            targetReal->trailColor[0] = 0.99f;
            targetReal->trailColor[1] = 0.75f;
            targetReal->trailColor[2] = 0.0f;
            break;
        }
        default:
        {
            type = "triangle_real";
            targetReal->trailColor[0] = 1.0f;
            targetReal->trailColor[1] = 1.0f;
            targetReal->trailColor[2] = 1.0f;
            break;
        }
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

    targetReal->trailsSize = 0;

    *out = targetReal;

    return 0;
}

void game_target_real_cycle(game_target_real_t* targetReal)
{    
    double progress = 1.0 - targetReal->target->progress;
    float x = progress * targetReal->distance;
    float y = tsin(progress * 3.14f * targetReal->frequency) / 36.0f * targetReal->amplitude;
    targetReal->object->position.x = (tsin(targetReal->angle) * x + tcos(targetReal->angle) * y) + targetReal->target->object->position.x;
    targetReal->object->position.y = (tcos(targetReal->angle) * -x - tsin(targetReal->angle) * y) + targetReal->target->object->position.y;

    // todo: make this a bit cleaner
    float trailProgress = progress;
    for (int i = 0; i < MAX_TRAILS; ++i)
    {
        float trailX = trailProgress * targetReal->distance;
        float trailY = tsin(trailProgress * 3.14f * targetReal->frequency) / 33.0f * targetReal->amplitude;
        targetReal->trails[0][i][0] = (tsin(targetReal->angle) * trailX + tcos(targetReal->angle) * trailY) + targetReal->target->object->position.x;
        targetReal->trails[0][i][1] = (tcos(targetReal->angle) * -trailX - tsin(targetReal->angle) * trailY) + targetReal->target->object->position.y;
        trailProgress += 0.1;
    }

    trailProgress = progress;
    for (int i = 0; i < MAX_TRAILS; ++i)
    {
        float trailX = trailProgress * targetReal->distance;
        float trailY = tsin(trailProgress * 3.14f * (targetReal->frequency + 0.4f)) / 25.0f * targetReal->amplitude;
        targetReal->trails[1][i][0] = (tsin(targetReal->angle) * trailX + tcos(targetReal->angle) * trailY) + targetReal->target->object->position.x;
        targetReal->trails[1][i][1] = (tcos(targetReal->angle) * -trailX - tsin(targetReal->angle) * trailY) + targetReal->target->object->position.y;
        trailProgress += 0.11;
    }
}

void game_target_real_free(game_target_real_t* targetReal)
{
    game_object_free(targetReal->object);
    free(targetReal);
}