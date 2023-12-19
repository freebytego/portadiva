#include "include/engine/scene_object.h"

int scene_object_create(scene_object_t** out)
{
    scene_object_t* scene = (scene_object_t*)malloc(sizeof(scene_object_t));
    if (NULL == scene)
    {
        fprintf(stderr, "failed to allocate a scene object");
        return -1;
    }

    SDL_FPoint position;
    render_properties_t renderProperties;

    if (game_object_create(&scene->object, "scene", position, renderProperties) != 0)
    {
        fprintf(stderr, "failed to create a game object for scene object");
        free(scene);
        return -1;
    }
    game_object_set_implementation(scene->object, scene);
    game_object_set_render(scene->object, &scene_object_render);

    scene->cycle = NULL;

    *out = scene;

    return 0;
}

void scene_object_cycle(void* scene)
{
    scene_object_t* sceneObject = (scene_object_t*)scene;
    if (NULL != sceneObject->cycle)
    {
        sceneObject->cycle(sceneObject->implementation);
    }
    list_node_t* child = sceneObject->object->children->begin;
    while (NULL != child)
    {
        game_object_cycle((game_object_t*)child->data);
        child = child->next;
    }
}

void scene_object_render(void* scene)
{
    scene_object_t* sceneObject = (scene_object_t*)scene;
    list_node_t* child = sceneObject->object->children->begin;
    while (NULL != child)
    {
        game_object_render((game_object_t*)child->data);
        child = child->next;
    }
}

void scene_object_free(scene_object_t* scene)
{
    game_object_free(scene->object);
    free(scene);
}

void scene_object_set_implementation(scene_object_t* scene, void* implementation)
{
    scene->implementation = implementation;
}

void scene_object_set_cycle(scene_object_t* scene, void (*cycle)(void*))
{
    scene->cycle = cycle;
}