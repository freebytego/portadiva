#ifndef GAME_TARGET_H
#define GAME_TARGET_H

#include <SDL2/SDL.h>

#include "include/engine/game_object.h"
#include "include/game/core/dsc/target.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"
#include "include/game/core/objects/target_needle.h"
#include <stdbool.h>

typedef struct game_target_t
{
    game_object_t* object;
    dsc_target_t* dscTarget;
    game_target_needle_t* needle;

    struct game_target_t* connectedTarget;
    int32_t createdAt;
    int32_t finishingAt;
    int32_t flyingTime;
    double progress;
    short finished;
    
} game_target_t;

int game_target_create(game_target_t** out, dsc_target_t* dscTarget, int32_t flyingTime);
void game_target_cycle(void* target);
void game_target_render(void* target);
void game_target_free(game_target_t* target);

#endif