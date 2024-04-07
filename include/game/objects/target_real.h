#ifndef GAME_TARGET_REAL_H
#define GAME_TARGET_REAL_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/generic_renderer.h"
#include "include/engine/texture_manager.h"
#include "include/engine/trig_lookup.h"

#define MAX_TRAILS 3

typedef struct game_target_t game_target_t;
typedef struct game_target_real_t
{
    game_object_t* object;
    game_target_t* target;
    float angle;
    float distance;
    float amplitude;
    float frequency;
    float trails[2][MAX_TRAILS][2];
    int trailsSize;
    float trailColor[3];
} game_target_real_t;

int game_target_real_create(game_target_real_t** out, game_target_t* target);
void game_target_real_cycle(game_target_real_t* target);
void game_target_real_free(game_target_real_t* target);


#endif