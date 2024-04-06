#ifndef GAME_TARGET_EFFECT_DESTROY_H
#define GAME_TARGET_EFFECT_DESTROY_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"

typedef struct game_target_t game_target_t;
typedef struct game_target_effect_destroy_t
{
    game_object_t* object;
} game_target_effect_destroy_t;

int game_target_effect_destroy_create(game_target_effect_destroy_t** out, SDL_FPoint position);
void game_target_effect_destroy_render(game_target_effect_destroy_t* renderer);
void game_target_effect_destroy_free(game_target_effect_destroy_t* renderer);

#endif