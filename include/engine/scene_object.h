#ifndef ENGINE_SCENE_OBJECT_H
#define ENGINE_SCENE_OBJECT_H

#include "include/engine/game_object.h"
#include "include/core/list.h"

typedef struct scene_object_t
{
    game_object_t* object;
    void* implementation; // pseudo polymorphysm
    void (*cycle)(void*);
} scene_object_t;

int scene_object_create(scene_object_t** out);
void scene_object_cycle(void* scene);
void scene_object_render(void* target);
void scene_object_free(scene_object_t* scene);

#endif