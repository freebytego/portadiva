#ifndef GAME_TARGET_H
#define GAME_TARGET_H

#include <SDL2/SDL.h>

#include "include/engine/game_object.h"
#include "include/game/core/dsc/target.h"
#include "include/engine/generic_renderer.h"
#include <stdbool.h>

typedef struct game_target_t
{
    game_object_t* object;
    dsc_target_t* dscTarget;
    struct game_target_t* connectedTarget;
    int32_t createdAt;
    int32_t flyingTime;
    short finished;
    
} game_target_t;

int game_target_create(game_target_t** out, SDL_FPoint position, int32_t flyingTime);
void game_target_cycle(void* target);
void game_target_render(void* target);
void game_target_free(game_target_t* target);

#endif