#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "include/engine/game_object.h"

typedef struct engine_t
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    game_object_t* scene;
    short running;
} engine_t;

int engine_create(engine_t** out);

int engine_set_scene(engine_t* engine, int (*create_scene_game_object)(game_object_t**, const char*), const char* sceneName);

void engine_cycle(engine_t* engine);

void engine_handle_input(engine_t* engine);
void engine_render(engine_t* engine);

void engine_free(engine_t* engine);

#endif