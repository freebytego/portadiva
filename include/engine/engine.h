#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/engine/game_object.h"
#include "include/engine/scene_object.h"
#include <GL/gl.h>

typedef struct engine_t
{
    SDL_GLContext* context;
    SDL_Window* window;
    scene_object_t* scene;
    void (*free_scene)(scene_object_t*);
    short running;
} engine_t;

extern engine_t* GLOBAL_ENGINE;

int engine_create(engine_t** out);

int engine_set_scene(
    engine_t* engine, 
    int (*create_scene_game_object)(scene_object_t**), 
    void (*free_scene_game_object)(scene_object_t*)
);

void engine_cycle(engine_t* engine);

void engine_handle_input(engine_t* engine);
void engine_render(engine_t* engine);

void engine_free_scene(engine_t* engine);
void engine_free(engine_t* engine);

#endif