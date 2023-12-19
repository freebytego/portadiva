#include "include/engine/texture_manager.h"

int texture_manager_create(texture_manager_t** out, const char* name)
{
    texture_manager_t* manager = (texture_manager_t*)malloc(sizeof(texture_manager_t));
    if (NULL == manager)
    {
        fprintf(stderr, "failed to allocate a texture manager");
        return -1;
    }
    if (node_list_create(&manager->textures) != 0)
    {
        free(manager);
        return -1;
    }
    manager->name = name;

    if (NULL == registeredTextureManagers)
    {
        if (node_list_create(&registeredTextureManagers) != 0)
        {
            free(manager);
            return -1;
        }
    }
    node_list_add_node(registeredTextureManagers, manager);

    *out = manager;
    return 0;
}

void texture_manager_free(texture_manager_t* manager)
{
    list_node_t* textureNode = manager->textures->begin;
    while (NULL != textureNode)
    {
        texture_t* texture = (texture_t*)textureNode->data;
        list_node_t* nextTextureNode = textureNode->next;
        texture_manager_free_texture(manager, texture);
        textureNode = nextTextureNode;
    }
    node_list_free(manager->textures);
    list_node_t* managerNode = registeredTextureManagers->begin;
    while (NULL != managerNode)
    {
        if (managerNode->data == manager)
        {
            node_list_remove_node(registeredTextureManagers, managerNode);
            break;
        }
        managerNode = managerNode->next;
    }
    if (registeredTextureManagers->length == 0)
    {
        node_list_free(registeredTextureManagers);
    }
    free(manager);
}

int texture_manager_add_texture(texture_manager_t* manager, const char* textureConfigPath)
{
/*
{
    "name": "something"
    "textureFilePath": "something.png",
    "parts": [
        {
            "name": "target",
            "x": 0,
            "y": 0,
            "width": 32
            "height": 32,
        }
    ]
}
*/
    cJSON* textureName = NULL;
    cJSON* textureFilePath = NULL;
    cJSON* textureParts = NULL;
    cJSON* texturePart = NULL;
    cJSON* textureDefinitionJson = NULL;
    list_node_t* texturePartNode = NULL;
    int retval = 0;

    texture_t* texture = (texture_t*)malloc(sizeof(texture_t));
    if (NULL == texture)
    {
        fprintf(stderr, "failed to allocate a texture\n");
        retval = -1;
        goto end;
    }
    texture->texture = NULL;

    FILE* textureDefinitionJsonFile = fopen(textureConfigPath, "r");
    size_t textureDefinitionJsonFileSize = 0;
    fseek(textureDefinitionJsonFile, 0L, SEEK_END);
    textureDefinitionJsonFileSize = ftell(textureDefinitionJsonFile);
    fseek(textureDefinitionJsonFile, 0L, SEEK_SET);
    char* textureDefinitionJsonFileContent = (char*)malloc(textureDefinitionJsonFileSize + 1);
    size_t readBytes = fread(textureDefinitionJsonFileContent, 1, textureDefinitionJsonFileSize, textureDefinitionJsonFile);
    textureDefinitionJsonFileContent[readBytes] = 0;
    fclose(textureDefinitionJsonFile);

    printf("read: %s\n", textureDefinitionJsonFileContent);

    textureDefinitionJson = cJSON_Parse(textureDefinitionJsonFileContent);
    if (NULL == textureDefinitionJson)
    {
        fprintf(stderr, "failed to read a texture definition json at '%s'\n", textureConfigPath);
        retval = -1;
        goto free_texture;
    }

    textureName = cJSON_GetObjectItem(textureDefinitionJson, "name");
    if (cJSON_IsString(textureName) && NULL != textureName->valuestring)
    {
        texture->name = textureName->valuestring;
    }
    else
    {
        fprintf(stderr, "invalid format for 'name' attribute in the texture definition json at '%s'\n", textureConfigPath);
        retval = -1;
        goto free_texture;
    }
    textureFilePath = cJSON_GetObjectItem(textureDefinitionJson, "textureFilePath");
    if (cJSON_IsString(textureFilePath) && NULL != textureFilePath->valuestring)
    {
        texture->filePath = textureFilePath->valuestring;
        SDL_Surface* surface = IMG_Load(texture->filePath);
        if (NULL == surface)
        {
            fprintf(stderr, "failed to create a surface\n");
            retval = -1;
            goto free_texture;
        }
        texture->texture = SDL_CreateTextureFromSurface(GLOBAL_ENGINE->renderer, surface);
        SDL_FreeSurface(surface);
        if (NULL == texture->texture)
        {
            fprintf(stderr, "failed to create a texture from surface: %s\n", SDL_GetError());
            retval = -1;
            goto free_texture;
        }
    }
    else
    {
        fprintf(stderr, "invalid format for 'textureFilePath' attribute in the texture definition json at '%s'\n", textureConfigPath);
        retval = -1;
        goto free_sdl_texture;
    }
    textureParts = cJSON_GetObjectItem(textureDefinitionJson, "parts");
    if (cJSON_IsArray(textureParts))
    {
        if (node_list_create(&texture->parts) != 0)
        {
            fprintf(stderr, "failed to allocate a texture\n");
            retval = -1;
            goto free_sdl_texture;
        }
        int index = 0;
        cJSON_ArrayForEach(texturePart, textureParts)
        {
            cJSON* name = cJSON_GetObjectItem(texturePart, "name");
            cJSON* x = cJSON_GetObjectItem(texturePart, "x");
            cJSON* y = cJSON_GetObjectItem(texturePart, "y");
            cJSON* width = cJSON_GetObjectItem(texturePart, "width");
            cJSON* height = cJSON_GetObjectItem(texturePart, "height");
            if (cJSON_IsString(name) && cJSON_IsNumber(x) && cJSON_IsNumber(y) && cJSON_IsNumber(width) && cJSON_IsNumber(height))
            {
                texture_part_t* part = (texture_part_t*)malloc(sizeof(texture_part_t));
                if (NULL == part)
                {
                    fprintf(stderr, "failed to allocate a texture part\n");
                    retval = -1;
                    goto free_texture_parts;
                }
                part->texture = texture;
                part->name = name->valuestring;
                part->source.x = x->valueint;
                part->source.y = y->valueint;
                part->source.w = width->valuedouble;
                part->source.h = width->valuedouble;
                if (node_list_add_node(texture->parts, (void*)part) != 0)
                {
                    free(part);
                    retval = -1;
                    goto free_texture_parts;
                }
            }
            else
            {
                printf("texture part at index %d has incorrect data, skipped\n", index);
            }
            ++index;
        }
    }
    else
    {
        fprintf(stderr, "invalid format for 'parts' attribute in the texture definition json at '%s'\n", textureConfigPath);
        retval = -1;
        goto free_sdl_texture;
    }
    // all good
    node_list_add_node(manager->textures, (void*)texture);
    goto free_json;

free_texture_parts:
    texturePartNode = texture->parts->begin;
    while (NULL != texturePartNode)
    {
        free(texturePartNode->data);
        texturePartNode = texturePartNode->next;
    }
    node_list_free(texture->parts);
free_sdl_texture:
    SDL_DestroyTexture(texture->texture);
free_texture:
    free(texture);
free_json:
    cJSON_Delete(textureDefinitionJson);
    free(textureDefinitionJsonFileContent);
end:
    return retval;
}

void texture_manager_free_texture(texture_manager_t* manager, texture_t* texture)
{
    list_node_t* partIterNode = texture->parts->begin;
    while (NULL != partIterNode)
    {
        free(partIterNode->data);
        partIterNode = partIterNode->next;
    }
    node_list_free(texture->parts);
    if (NULL != texture->texture) SDL_DestroyTexture(texture->texture);
    node_list_remove_node(manager->textures, *(list_node_t**)&texture);
    free(texture);
}