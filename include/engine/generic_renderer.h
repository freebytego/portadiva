#ifndef ENGINE_GENERIC_RENDERER_H
#define ENGINE_GENERIC_RENDERER_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/engine.h"
#include "include/engine/texture_manager.h"

void engine_generic_renderer_render(game_object_t* object);

#endif