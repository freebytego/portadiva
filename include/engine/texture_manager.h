#ifndef ENGINE_TEXTURE_MANAGER_H
#define ENGINE_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include "include/core/list.h"
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

typedef struct
{
    char* name;
    list_t* textures;
} texture_manager_t;

typedef struct
{
    char* name;
    char* filePath;
    list_t* parts; // texture_part_t
    GLuint textureId;
    int width;
    int height;
} texture_t;

typedef struct 
{
    char* name;
    texture_t* texture;
    SDL_Rect source;
} texture_part_t;

typedef struct
{
    float x1;
    float x2;
    float y1;
    float y2;
} texture_position_t; // i can just use SDL_FRect for this, but this is slightly more readable

int texture_manager_create(texture_manager_t** out, const char* name);
void texture_manager_free(texture_manager_t* manager);
int texture_manager_add_texture(texture_manager_t* manager, const char* textureConfigPath);
void texture_manager_free_texture(texture_manager_t* manager, texture_t* texture);

static list_t* registeredTextureManagers = NULL;
texture_manager_t* texture_manager_find_registered(const char* name);
texture_t* texture_manager_find_texture_in_registered(const char* managerName, const char* textureName);
texture_part_t* texture_manager_find_texture_part_in_registered(const char* managerName, const char* textureName, const char* texturePartName);
texture_position_t texture_manager_get_texture_position_from_texture_part(texture_part_t* texturePart);

#endif