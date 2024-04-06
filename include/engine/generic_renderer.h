#ifndef ENGINE_GENERIC_RENDERER_H
#define ENGINE_GENERIC_RENDERER_H

#include <SDL2/SDL.h>
#include "include/engine/game_object.h"
#include "include/engine/texture_manager.h"
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif


typedef struct generic_renderer_t 
{
    list_t* queue;
} generic_renderer_t;

typedef enum generic_renderable_type
{
    GAME_OBJECT_RENDER = 0,
    FUNCTION_RENDER
} generic_renderable_type;

typedef struct generic_renderable_t
{
    generic_renderable_type type;
    game_object_t* gameObject;
    void (*render_function)(void*);
    void* renderFunctionTarget;
    uint16_t order;
} generic_renderable_t;

int engine_generic_renderer_create(generic_renderer_t** out);
void engine_generic_renderer_render(generic_renderer_t* renderer);
void engine_generic_renderer_free(generic_renderer_t* renderer);
void engine_generic_renderer_clear_queue(generic_renderer_t* renderer);
void engine_generic_renderer_add_to_queue(generic_renderer_t* renderer, generic_renderable_t* renderable);
int engine_generic_renderer_create_renderable(generic_renderable_t** out);

#endif