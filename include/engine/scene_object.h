#ifndef ENGINE_SCENE_OBJECT_H
#define ENGINE_SCENE_OBJECT_H

#include "include/engine/game_object.h"
#include "include/core/list.h"

typedef struct scene_object_t
{
    game_object_t* object;
} scene_object_t;

int scene_object_create(scene_object_t** out);
void scene_object_render(void* target);
void scene_object_free(scene_object_t* scene);

#endif