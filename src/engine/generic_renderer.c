#include "include/engine/generic_renderer.h"

void engine_generic_renderer_render(game_object_t* object)
{
    SDL_FRect rect;
    rect.w = object->renderProperties.width;
    rect.h = object->renderProperties.height;

    printf("%s -> %f x %f\n", object->name, rect.w, rect.h);
    switch (object->renderProperties.offsetType)
    {
    case RENDER_OFFSET_TOP_LEFT:
    {
        rect.x = object->position.x;
        rect.y = object->position.y;
        break;
    }
    case RENDER_OFFSET_CENTER:
    {
        rect.x = object->position.x - rect.w / 2.0f;
        rect.y = object->position.y - rect.h / 2.0f;
    }
    }
    if (NULL == object->texturePart)
    {
        SDL_SetRenderDrawColor(GLOBAL_ENGINE->renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(GLOBAL_ENGINE->renderer, &rect);
        return;
    }
    rect.x += object->renderProperties.offset.x;
    rect.y += object->renderProperties.offset.y;
    SDL_RenderCopyExF(
        GLOBAL_ENGINE->renderer, 
        object->texturePart->texture->texture, 
        &object->texturePart->source,
        &rect,
        object->renderProperties.angle,
        &object->renderProperties.center,
        SDL_FLIP_NONE
    );
}