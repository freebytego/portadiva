#include "include/game/objects/target_effect_destroy.h"

int game_target_effect_destroy_create(game_target_effect_destroy_t** out, texture_part_t** textureParts, SDL_FPoint position)
{
    game_target_effect_destroy_t* effect = (game_target_effect_destroy_t*)malloc(sizeof(game_target_effect_destroy_t));
    if (NULL == effect)
    {
        fprintf(stderr, "failed to allocate a target destroy effect");
        return -1;
    }

    render_properties_t renderProperties;
    if (game_object_create(&effect->object, "base target destroy effect", position, renderProperties, NULL) != 0)
    {
        fprintf(stderr, "failed to create a game object for target destroy effect");
        free(effect);
        return -1;
    }

    game_object_set_implementation(effect->object, effect);
    game_object_set_render(effect->object, (void (*)(void*))&game_target_effect_destroy_render);
    game_object_set_cycle(effect->object, (void (*)(void*))&game_target_effect_destroy_cycle);
    game_object_set_free(effect->object, (void (*)(void*))&game_target_effect_destroy_free);

    render_properties_t layersRenderProperties[3];
    layersRenderProperties[0] = render_properties_create(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RENDER_OFFSET_CENTER, 1.0f, 100);
    layersRenderProperties[1] = render_properties_create(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RENDER_OFFSET_CENTER, 1.0f, 100);
    layersRenderProperties[2] = render_properties_create(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RENDER_OFFSET_CENTER, 1.0f, 100);
    if (game_object_create(&effect->objectLayers[0], "layer 1 target destroy effect", position, layersRenderProperties[0], textureParts[0]) != 0)
    {
        fprintf(stderr, "failed to create a game object for layer 1 target destroy effect");
        free(effect);
        return -1;
    }
    if (game_object_create(&effect->objectLayers[1], "layer 1 target destroy effect", position, layersRenderProperties[1], textureParts[1]) != 0)
    {
        fprintf(stderr, "failed to create a game object for layer 2 target destroy effect");
        free(effect);
        return -1;
    }
    if (game_object_create(&effect->objectLayers[2], "layer 1 target destroy effect", position, layersRenderProperties[2], textureParts[2]) != 0)
    {
        fprintf(stderr, "failed to create a game object for layer 3 target destroy effect");
        free(effect);
        return -1;
    }

    effect->animationStartedAt[0] = SDL_GetTicks() * 100;
    effect->desiredSize[0].x = 100.0f;
    effect->desiredSize[0].y = 100.0f;
    effect->doingAnimation[0] = 1;
    effect->animationTime[0] = 0.2f * 1000 * 100;
    effect->animationEndAt[0] = effect->animationStartedAt[0] + effect->animationTime[0];
    effect->desiredSize[1].x = 120.0f;
    effect->desiredSize[1].y = 120.0f;
    effect->doingAnimation[1] = 0;
    effect->animationTime[1] = 0.3f * 1000 * 100;
    effect->desiredSize[2].x = 100.0f;
    effect->desiredSize[2].y = 100.0f;
    effect->doingAnimation[2] = 0;
    effect->animationTime[2] = 0.6f * 1000 * 100;

    *out = effect;

    return 0;
}

void game_target_effect_destroy_cycle(game_target_effect_destroy_t* effect)
{
    uint32_t currentTicks = SDL_GetTicks() * 100;
    if (effect->doingAnimation[0])
    {
        if (currentTicks >= effect->animationEndAt[0])
        {
            effect->doingAnimation[0] = 0;
            effect->doingAnimation[1] = 1;
            effect->doingAnimation[2] = 1;
            effect->objectLayers[0]->renderProperties.width = effect->desiredSize[0].x;
            effect->objectLayers[0]->renderProperties.height = effect->desiredSize[0].y;
            effect->animationStartedAt[1] = currentTicks;
            effect->animationEndAt[1] = currentTicks + effect->animationTime[1];
            effect->animationStartedAt[2] = currentTicks;
            effect->animationEndAt[2] = currentTicks + effect->animationTime[2];
        }
        else
        {
            double animationProgress = ((double)(currentTicks - effect->animationStartedAt[0]) / (double)(effect->animationEndAt[0] - effect->animationStartedAt[0]));
            effect->objectLayers[0]->renderProperties.width = bezier(animationProgress, 0.0, 0.6, 0.8, 1.0) * effect->desiredSize[0].x;
            effect->objectLayers[0]->renderProperties.height = bezier(animationProgress, 0.0, 0.6, 0.8, 1.0) * effect->desiredSize[0].y;
            effect->objectLayers[0]->renderProperties.opacity = bezier(animationProgress, 1.0, 0.9, 0.6, 0.0) * 0.6;
        }
    }
    if (effect->doingAnimation[1])
    {
        if (currentTicks >= effect->animationEndAt[1])
        {
            effect->doingAnimation[1] = 0;
            effect->objectLayers[1]->renderProperties.width = effect->desiredSize[1].x;
            effect->objectLayers[1]->renderProperties.height = effect->desiredSize[1].y;
        }
        else
        {
            double animationProgress = ((double)(currentTicks - effect->animationStartedAt[1]) / (double)(effect->animationEndAt[1] - effect->animationStartedAt[1]));
            effect->objectLayers[1]->renderProperties.width = bezier(animationProgress, 0.0, 0.6, 0.8, 1.0) * effect->desiredSize[1].x;
            effect->objectLayers[1]->renderProperties.height = bezier(animationProgress, 0.0, 0.6, 0.8, 1.0) * effect->desiredSize[1].y;
            effect->objectLayers[1]->renderProperties.opacity = bezier(animationProgress, 1.0, 0.9, 0.6, 0.0) * 0.6;
        }
    }
    if (effect->doingAnimation[2])
    {
        if (currentTicks >= effect->animationEndAt[2])
        {
            effect->doingAnimation[2] = 0;
            effect->objectLayers[2]->renderProperties.width = effect->desiredSize[2].x;
            effect->objectLayers[2]->renderProperties.height = effect->desiredSize[2].y;
        }
        else
        {
            double animationProgress = ((double)(currentTicks - effect->animationStartedAt[2]) / (double)(effect->animationEndAt[2] - effect->animationStartedAt[2]));
            effect->objectLayers[2]->renderProperties.width = bezier(animationProgress, 0.0, 0.8, 1.2, 1.0) * effect->desiredSize[2].x;
            effect->objectLayers[2]->renderProperties.height = bezier(animationProgress, 0.0, 0.8, 1.2, 1.0) * effect->desiredSize[2].y;
            effect->objectLayers[2]->renderProperties.opacity = bezier(animationProgress, 1.0, 0.9, 0.7, 0.0) * 0.4;
        }
    }
}

void game_target_effect_destroy_render(game_target_effect_destroy_t* effect)
{
    generic_renderable_t* renderables[3];
    if (engine_generic_renderer_create_renderable(&renderables[0]) != 0)
    {
        return;
    }
    renderables[0]->type = GAME_OBJECT_RENDER;
    renderables[0]->gameObject = effect->objectLayers[0];
    renderables[0]->order = renderables[0]->gameObject->renderProperties.order;
    engine_generic_renderer_add_to_queue(GLOBAL_ENGINE->renderer, renderables[0]);
    if (engine_generic_renderer_create_renderable(&renderables[1]) != 0)
    {
        return;
    }
    renderables[1]->type = GAME_OBJECT_RENDER;
    renderables[1]->gameObject = effect->objectLayers[1];
    renderables[1]->order = renderables[1]->gameObject->renderProperties.order;
    engine_generic_renderer_add_to_queue(GLOBAL_ENGINE->renderer, renderables[1]);
    if (engine_generic_renderer_create_renderable(&renderables[2]) != 0)
    {
        return;
    }
    renderables[2]->type = GAME_OBJECT_RENDER;
    renderables[2]->gameObject = effect->objectLayers[2];
    renderables[2]->order = renderables[2]->gameObject->renderProperties.order;
    engine_generic_renderer_add_to_queue(GLOBAL_ENGINE->renderer, renderables[2]);
}

void game_target_effect_destroy_free(game_target_effect_destroy_t* effect)
{
    game_object_free(effect->object);
    game_object_free(effect->objectLayers[0]);
    game_object_free(effect->objectLayers[1]);
    game_object_free(effect->objectLayers[2]);
    free(effect);
}
