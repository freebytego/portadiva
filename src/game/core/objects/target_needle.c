#include "include/game/core/objects/target_needle.h"
#include "include/game/core/objects/target.h"

int game_target_needle_create(game_target_needle_t** out, game_target_t* target)
{
    game_target_needle_t* targetNeedle = (game_target_needle_t*)malloc(sizeof(game_target_needle_t));
    if (NULL == targetNeedle)
    {
        fprintf(stderr, "failed to allocate a target needle");
        return -1;
    }

    render_properties_t renderProperties = render_properties_create(30.0f, 22.0f, 0.0f, 0.0f, -8.0f, RENDER_OFFSET_CENTER, 1.0f, 10);
    texture_part_t* texturePart = texture_manager_find_texture_part_in_registered("textures", "buttons", "needle");
    
    if (game_object_create(&targetNeedle->object, "target needle", target->object->position, renderProperties, texturePart) != 0)
    {
        fprintf(stderr, "failed to create a game object for target needle");
        free(targetNeedle);
        return -1;
    }

    game_object_set_implementation(targetNeedle->object, targetNeedle);
    game_object_set_cycle(targetNeedle->object, (void (*)(void*))&game_target_needle_cycle);
    game_object_set_free(targetNeedle->object, (void (*)(void*))&game_target_needle_free);

    targetNeedle->target = target;

    *out = targetNeedle;

    return 0;
}

void game_target_needle_cycle(game_target_needle_t* targetNeedle)
{
    targetNeedle->object->renderProperties.angle = 360.0 * targetNeedle->target->progress;
}

void game_target_needle_free(game_target_needle_t* targetNeedle)
{
    game_object_free(targetNeedle->object);
    free(targetNeedle);
}