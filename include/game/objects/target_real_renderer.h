#ifndef GAME_TARGET_REAL_RENDERER_H
#define GAME_TARGET_REAL_RENDERER_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/game/objects/target_real.h"

typedef struct game_target_t game_target_t;
typedef struct game_target_real_renderer_t
{
    game_object_t* object;
    game_target_t* target;
} game_target_real_renderer_t;

int game_target_real_renderer_create(game_target_real_renderer_t** out);
void game_target_real_renderer_render(game_target_real_renderer_t* renderer);
void game_target_real_renderer_free(game_target_real_renderer_t* renderer);

#endif