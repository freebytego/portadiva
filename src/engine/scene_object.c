#include "include/engine/scene_object.h"

int scene_object_create(scene_object_t** out)
{
    scene_object_t* scene = (scene_object_t*)malloc(sizeof(scene_object_t));
    if (NULL == scene)
    {
        perror("failed to allocate a scene object");
        return -1;
    }

    SDL_Point position;
    render_properties_t renderProperties;

    if (game_object_create(&scene->object, "scene", position, renderProperties) != 0)
    {
        perror("failed to create a game object for scene object");
        free(scene);
        return -1;
    }
    game_object_set_implementation(scene->object, scene);
    game_object_set_render(scene->object, &scene_object_render);

    *out = scene;

    return 0;
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