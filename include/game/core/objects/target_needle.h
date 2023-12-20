#ifndef GAME_TARGET_NEEDLE_H
#define GAME_TARGET_NEEDLE_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"

typedef struct game_target_t game_target_t;
typedef struct game_target_needle_t
{
    game_object_t* object;
    game_target_t* target;
} game_target_needle_t;

int game_target_needle_create(game_target_needle_t** out, game_target_t* target);
void game_target_needle_cycle(void* target);
void game_target_needle_free(game_target_needle_t* target);

#endif