#include "include/engine/game_object.h"

int game_object_create(game_object_t** out, const char* name, SDL_FPoint position, render_properties_t renderProperties, texture_part_t* texturePart)
{
    static int id = 0;

    game_object_t* object = (game_object_t*)malloc(sizeof(game_object_t));
    if (NULL == object)
    {
        fprintf(stderr, "failed allocating a game object");
    }

    object->id = id++;
    object->name = name;
    object->cycle = NULL;
    object->handle_input = NULL;
    object->render = NULL;
    object->position = position;
    object->parent = NULL;
    object->renderProperties = renderProperties;
    object->texturePart = texturePart;

    if (node_list_create(&object->children) != 0)
    {
        free(object);
        return -1;
    }

    *out = object;
    return 0;
}

void game_object_set_parent(game_object_t* object, game_object_t* parent)
{
    object->parent = parent;
}

int game_object_add_child(game_object_t* object, game_object_t* child)
{
    game_object_set_parent(child, object);
    if (node_list_add_node(object->children, child) != 0)
    {
        game_object_set_parent(child, NULL);
        return -1;
    }
    child->parentChildNode = object->children->end;
    return 0;
}

void game_object_remove_child(game_object_t* object, game_object_t* child)
{
    node_list_remove_node(object->children, child->parentChildNode);
    child->parent = NULL;
    child->parentChildNode = NULL;
}

void game_object_free(game_object_t* object)
{
    list_node_t* childNode = object->children->begin;
    while (childNode != NULL)
    {
        game_object_free((game_object_t*)childNode->data); // needs to call implementation destroy
        childNode = childNode->next;
    }
    node_list_free(object->children);
    if (NULL != object->parent)
    {
        game_object_remove_child(object->parent, object);
    }
    free(object);
}

void game_object_set_implementation(game_object_t* object, void* implementation)
{
    object->implementation = implementation;
}

void game_object_set_cycle(game_object_t* object, void (*cycle)(void*))
{
    object->cycle = cycle;
}

void game_object_set_handle_input(game_object_t* object, void (*handle_input)(void*))
{
    object->handle_input = handle_input;
}

void game_object_set_render(game_object_t* object, void (*render)(void*))
{
    object->render = render;
}

void game_object_cycle(game_object_t* object)
{
    if (NULL != object->cycle)
    {
        object->cycle(object->implementation);
    }
    list_node_t* child = object->children->begin;
    while (NULL != child)
    {
        game_object_cycle((game_object_t*)child->data);
        child = child->next;
    }
}

void game_object_render(game_object_t* object) // It is the object's duty to draw its children however it wants
{
    if (NULL != object->render)
    {
        object->render(object->implementation);
    }
}