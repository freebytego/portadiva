#ifndef GAME_TARGET_EFFECT_APPEAR_H
#define GAME_TARGET_EFFECT_APPEAR_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"
#include "include/engine/engine.h"

typedef struct game_target_t game_target_t;
typedef struct game_target_effect_appear_t
{
    game_object_t* object;
    short effectDone;
} game_target_effect_appear_t;

int game_target_effect_appear_create(game_target_effect_appear_t** out, texture_part_t* texturePart, SDL_FPoint position);
void game_target_effect_appear_render(game_target_effect_appear_t* effect);
void game_target_effect_appear_cycle(game_target_effect_appear_t* effect);
void game_target_effect_appear_free(game_target_effect_appear_t* effect);

#endif