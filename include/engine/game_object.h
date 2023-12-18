#ifndef ENGINE_GAME_OBJECT_H
#define ENGINE_GAME_OBJECT_H

#include "include/core/list.h"
#include <stdio.h>
#include <SDL2/SDL.h>

typedef enum {
    RENDER_OFFSET_TOP_LEFT,
    RENDER_OFFSET_CENTER
} render_offset_type_t;

typedef struct render_properties_t
{
    float width;
    float height;
    render_offset_type_t offsetType;
} render_properties_t;

typedef struct game_object_t
{
    int id;
    const char* name;
    SDL_Point position;
    render_properties_t renderProperties;
    struct game_object_t* parent;
    list_node_t* parentChildNode;
    list_t* children;

    void* implementation; // pseudo polymorphysm
    void (*cycle)(void*);
    void (*handle_input)(void*);
    void (*render)(void*); // not every object needs to be rendered, maybe this should be a component-like thing
} game_object_t;

int game_object_create(game_object_t** out, const char* name, SDL_Point position, render_properties_t renderProperties);
void game_object_set_parent(game_object_t* object, game_object_t* parent);
int game_object_add_child(game_object_t* object, game_object_t* child);
void game_object_remove_child(game_object_t* object, game_object_t* child);
void game_object_free(game_object_t* object);
void game_object_set_implementation(game_object_t* object, void* implementation);
void game_object_set_cycle(game_object_t* object, void (*cycle)(void*));
void game_object_set_handle_input(game_object_t* object, void (*handle_input)(void*));
void game_object_set_render(game_object_t* object, void (*render)(void*));
void game_object_cycle(game_object_t* object);
void game_object_render(game_object_t* object);

#endif