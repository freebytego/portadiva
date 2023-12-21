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

    render_properties_t renderProperties; // READ THIS FROM THE TEXTURE'S CONFIG!!!!
    renderProperties.width = 30.0f;
    renderProperties.height = 22.0f;
    renderProperties.angle = 0.0f;
    renderProperties.center.x = renderProperties.width / 2.0f;
    renderProperties.center.y = renderProperties.height - 2.8f;
    renderProperties.offset.x = 0.0f; // todo: from config
    renderProperties.offset.y = -8.0f; // todo: from config

    renderProperties.offsetType = RENDER_OFFSET_CENTER;

    texture_part_t* texturePart = texture_manager_find_texture_part_in_registered("textures", "buttons", "needle");
    
    if (game_object_create(&targetNeedle->object, "target", target->object->position, renderProperties, texturePart) != 0)
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

void game_target_needle_render(game_target_needle_t* targetNeedle)
{
    engine_generic_renderer_render(targetNeedle->object);
}

void game_target_needle_free(game_target_needle_t* targetNeedle)
{
    game_object_free(targetNeedle->object);
    free(targetNeedle);
}