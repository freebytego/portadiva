#include "include/game/core/objects/target_real_renderer.h"

int game_target_real_renderer_create(game_target_real_renderer_t** out)
{
    game_target_real_renderer_t* renderer = (game_target_real_renderer_t*)malloc(sizeof(game_target_real_renderer_t));
    if (NULL == renderer)
    {
        fprintf(stderr, "failed to allocate a target real renderer\n");
        return -1;
    }
    SDL_FPoint position;
    render_properties_t renderProperties;
    if (game_object_create(&renderer->object, "target real renderer", position, renderProperties, NULL) != 0)
    {
        fprintf(stderr, "failed to create a game object for target real renderer\n");
        free(renderer);
        return -1;
    }
    game_object_set_implementation(renderer->object, renderer);
    game_object_set_render(renderer->object, (void (*)(void*))&game_target_real_renderer_render);
    game_object_set_free(renderer->object, (void (*)(void*))&game_target_real_renderer_free);

    *out = renderer;

    return 0;
}

void game_target_real_renderer_render(game_target_real_renderer_t* renderer)
{
    list_node_t* child = renderer->object->children->begin;
    while (NULL != child)
    {
        engine_generic_renderer_render((game_object_t*)child->data);
        child = child->next;
    }
}

void game_target_real_renderer_free(game_target_real_renderer_t* renderer)
{
    // game_object_free will free all its children, so it will kill all of the real targets before the target object can do so.
    // to prevent this we must unparent all of children
    list_node_t* childNode = renderer->object->children->begin;
    while (NULL != childNode)
    {
        game_object_remove_child(renderer->object, (game_object_t*)childNode->data);
        childNode = childNode->next;
    }
    game_object_free(renderer->object);
    free(renderer);
}