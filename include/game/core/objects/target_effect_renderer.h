#ifndef GAME_TARGET_EFFECT_RENDERER_H
#define GAME_TARGET_EFFECT_RENDERER_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"

typedef struct game_target_t game_target_t;
typedef struct game_target_effect_renderer_t
{
    game_object_t* object;
    texture_part_t* spawnEffectTexturePart;
    list_t* spawnEffectTargets;
} game_target_effect_renderer_t;

int game_target_effect_renderer_create(game_target_effect_renderer_t** out);
void game_target_effect_renderer_render(game_target_effect_renderer_t* renderer);
void game_target_effect_renderer_cycle(game_target_effect_renderer_t* renderer);
void game_target_effect_renderer_free(game_target_effect_renderer_t* renderer);
void game_target_effect_renderer_spawn_create_effect(game_target_effect_renderer_t* renderer, game_target_t* target);
void game_target_effect_renderer_spawn_delete_effect(game_target_effect_renderer_t* renderer, game_target_t* target);

#endif