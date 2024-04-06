#include "include/game/core/objects/target_effect_appear.h"

int game_target_effect_appear_create(game_target_effect_appear_t** out, texture_part_t* texturePart, SDL_FPoint position)
{
    game_target_effect_appear_t* effect = (game_target_effect_appear_t*)malloc(sizeof(game_target_effect_appear_t));
    if (NULL == effect)
    {
        fprintf(stderr, "failed to allocate a target destroy effect");
        return -1;
    }

    render_properties_t renderProperties = render_properties_create(20.0f, 20.0f, 0.0f, 0.0f, 0.0f, RENDER_OFFSET_CENTER, 1.0f, 200);
    if (game_object_create(&effect->object, "target appear effect", position, renderProperties, texturePart) != 0)
    {
        fprintf(stderr, "failed to create a game object for target appear effect");
        free(effect);
        return -1;
    }

    game_object_set_implementation(effect->object, effect);
    game_object_set_render(effect->object, (void (*)(void*))&game_target_effect_appear_render);
    game_object_set_cycle(effect->object, (void (*)(void*))&game_target_effect_appear_cycle);
    game_object_set_free(effect->object, (void (*)(void*))&game_target_effect_appear_free);

    *out = effect;

    return 0;
}

void game_target_effect_appear_cycle(game_target_effect_appear_t* effect)
{
    
}

void game_target_effect_appear_render(game_target_effect_appear_t* effect)
{
    generic_renderable_t* renderable;
    if (engine_generic_renderer_create_renderable(&renderable) != 0)
    {
        return;
    }
    renderable->type = GAME_OBJECT_RENDER;
    renderable->gameObject = effect->object;
    engine_generic_renderer_add_to_queue(GLOBAL_ENGINE->renderer, renderable);
}

void game_target_effect_appear_free(game_target_effect_appear_t* effect)
{
    game_object_free(effect->object);
    free(effect);
}
