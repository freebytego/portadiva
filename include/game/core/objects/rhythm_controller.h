#ifndef GAME_RHYTHM_CONTROLLER_H
#define GAME_RHYTHM_CONTROLLER_H

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "include/engine/scene_object.h"
#include "include/engine/generic_renderer.h"

#include "include/game/core/dsc/dsc.h"
#include "include/game/core/objects/target.h"
#include "include/core/list.h"

typedef struct {
    game_object_t* object;
    dsc_script_t* script;
    uint32_t startedAt;
    uint32_t ticks;
} game_rhythm_controller_t;

int game_rhythm_controller_create_from_path(game_rhythm_controller_t** out, const char* filePath);
void game_rhythm_controller_cycle(game_rhythm_controller_t* controller);
void game_rhythm_controller_render(game_rhythm_controller_t* controller);
void game_rhythm_controller_free(game_rhythm_controller_t* controller);

#endif