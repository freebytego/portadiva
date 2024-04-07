#ifndef GAME_TARGET_EFFECT_DESTROY_H
#define GAME_TARGET_EFFECT_DESTROY_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"
#include "include/engine/engine.h"
#include "include/game/utils/bezier.h"

typedef struct game_target_t game_target_t;
typedef struct game_target_effect_destroy_t
{
    game_object_t* object;
    game_object_t* objectLayers[3];
    short doingAnimation[3];
    uint32_t animationTime[3];
    uint32_t animationEndAt[3];
    uint32_t animationStartedAt[3];
    SDL_FPoint desiredSize[3];
} game_target_effect_destroy_t;

int game_target_effect_destroy_create(game_target_effect_destroy_t** out, texture_part_t** textureParts, SDL_FPoint position);
void game_target_effect_destroy_render(game_target_effect_destroy_t* effect);
void game_target_effect_destroy_cycle(game_target_effect_destroy_t* effect);
void game_target_effect_destroy_free(game_target_effect_destroy_t* effect);

#endif