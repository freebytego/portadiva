#ifndef ENGINE_TEXTURE_MANAGER_H
#define ENGINE_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include "include/core/list.h"
#include "include/engine/engine.h"

typedef struct
{
    const char* name;
    list_t* textures;
} texture_manager_t;

typedef struct
{
    const char* name;
    const char* filePath;
    SDL_Texture* texture;
    list_t* parts; // texture_part_t
} texture_t;

typedef struct 
{
    const char* name;
    texture_t* texture;
    SDL_Rect source;
} texture_part_t;

int texture_manager_create(texture_manager_t** out, const char* name);
void texture_manager_free(texture_manager_t* manager);
int texture_manager_add_texture(texture_manager_t* manager, const char* textureConfigPath);
void texture_manager_free_texture(texture_manager_t* manager, texture_t* texture);

static list_t* registeredTextureManagers = NULL;
texture_manager_t* texture_manager_find_registered(const char* name);

#endif