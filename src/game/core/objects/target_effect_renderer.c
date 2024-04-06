#include "include/game/core/objects/target_effect_renderer.h"
#include "include/game/core/objects/target.h"
#include "include/game/core/objects/target_effect_appear.h"

int game_target_effect_renderer_create(game_target_effect_renderer_t** out)
{
    game_target_effect_renderer_t* renderer = (game_target_effect_renderer_t*)malloc(sizeof(game_target_effect_renderer_t));
    if (NULL == renderer)
    {
        fprintf(stderr, "failed to allocate a target effect renderer");
        return -1;
    }

    render_properties_t renderProperties;
    SDL_FPoint position;
    if (game_object_create(&renderer->object, "target effect", position, renderProperties, NULL) != 0)
    {
        fprintf(stderr, "failed to create a game object for target effect renderer");
        free(renderer);
        return -1;
    }

    game_object_set_implementation(renderer->object, renderer);
    game_object_set_render(renderer->object, (void (*)(void*))&game_target_effect_renderer_render);
    game_object_set_cycle(renderer->object, (void (*)(void*))&game_target_effect_renderer_cycle);
    game_object_set_free(renderer->object, (void (*)(void*))&game_target_effect_renderer_free);

    renderer->spawnEffectTexturePart = texture_manager_find_texture_part_in_registered("textures", "button_effects", "effect_appear_1");
    if (node_list_create(&renderer->spawnEffectTargets) != 0)
    {
        fprintf(stderr, "failed to create a target effect renderer");
        game_object_free(renderer->object);
        free(renderer);
        return -1;
    }

    *out = renderer;

    return 0;
}

void game_target_effect_renderer_spawn_create_effect(game_target_effect_renderer_t* renderer, game_target_t* target)
{
    game_target_effect_appear_t* effect;
    if (game_target_effect_appear_create(&effect, renderer->spawnEffectTexturePart, target->object->position) != 0)
    {
        fprintf(stderr, "failed to create a target appear effect");
        return;
    }
    if (node_list_add_node(renderer->spawnEffectTargets, effect) != 0)
    {
        fprintf(stderr, "failed to append a target appear effect");
        game_target_effect_appear_free(effect);
    }
}

void game_target_effect_renderer_spawn_delete_effect(game_target_effect_renderer_t* renderer, game_target_t* target)
{

}

void game_target_effect_renderer_cycle(game_target_effect_renderer_t* renderer)
{
    list_node_t* child = renderer->spawnEffectTargets->begin;
    while (NULL != child)
    {
        game_target_effect_appear_cycle((game_target_effect_appear_t*)child->data);
        child = child->next;
    }
}

void game_target_effect_renderer_render(game_target_effect_renderer_t* renderer)
{
    list_node_t* child = renderer->spawnEffectTargets->begin;
    while (NULL != child)
    {
        game_target_effect_appear_render((game_target_effect_appear_t*)child->data);
        child = child->next;
    }
}

void game_target_effect_renderer_free(game_target_effect_renderer_t* renderer)
{
    // todo: clear spawnEffectTargets game objects if present
    node_list_free(renderer->spawnEffectTargets);
    game_object_free(renderer->object);
    free(renderer);
}