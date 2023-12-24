#ifndef GAME_TARGET_H
#define GAME_TARGET_H

#include <SDL2/SDL.h>

#include "include/engine/engine.h"
#include "include/engine/game_object.h"
#include "include/game/core/dsc/target.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"
#include "include/game/core/objects/target_needle.h"
#include "include/game/core/objects/rhythm_controller.h"
#include "include/game/core/objects/target_real.h"
#include <stdbool.h>
#include <math.h>

typedef struct game_target_t
{
    game_object_t* object;
    dsc_target_t* dscTarget;
    game_target_needle_t* needle;
    game_target_real_t* real;

    struct game_target_t* connectedTarget;
    int32_t createdAt;
    int32_t finishingAt;
    int32_t flyingTime;
    double progress;
    short finished;

    short doingAnimation;
    uint32_t animationTime;
    uint32_t animationEndAt;
    SDL_FPoint desiredSize;
} game_target_t;

int game_target_create(game_target_t** out, dsc_target_t* dscTarget, int32_t flyingTime);
void game_target_cycle(game_target_t* target);
void game_target_render(game_target_t* target);
void game_target_free(game_target_t* target);
int game_target_create_target_real(game_target_t* target);

#endif