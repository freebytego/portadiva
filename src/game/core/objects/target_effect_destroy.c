#include "include/game/core/objects/target_effect_destroy.h"

int game_target_effect_destroy_create(game_target_effect_destroy_t** out, SDL_FPoint position)
{
    game_target_effect_destroy_t* effect = (game_target_effect_destroy_t*)malloc(sizeof(game_target_effect_destroy_t));
    if (NULL == effect)
    {
        fprintf(stderr, "failed to allocate a target destroy effect");
        return -1;
    }

    render_properties_t renderProperties;
    SDL_FPoint position;
    if (game_object_create(&effect->object, "target destroy effect", position, renderProperties, NULL) != 0)
    {
        fprintf(stderr, "failed to create a game object for target destroy effect");
        free(effect);
        return -1;
    }

    game_object_set_implementation(effect->object, effect);
    game_object_set_render(effect->object, (void (*)(void*))&game_target_effect_destroy_render);
    game_object_set_cycle(effect->object, (void (*)(void*))&game_target_effect_destroy_cycle);
    game_object_set_free(effect->object, (void (*)(void*))&game_target_effect_destroy_free);

    *out = effect;

    return 0;
}

void game_target_effect_destroy_cycle(game_target_effect_destroy_t* renderer)
{

}

void game_target_effect_destroy_render(game_target_effect_destroy_t* renderer)
{

}

void game_target_effect_destroy_free(game_target_effect_destroy_t* renderer)
{

}
