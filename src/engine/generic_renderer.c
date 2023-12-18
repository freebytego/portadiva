#include "include/engine/generic_renderer.h"

void engine_generic_renderer_render(game_object_t* object)
{
    SDL_FRect rect;
    rect.w = object->renderProperties.width;
    rect.h = object->renderProperties.height;
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
    printf("%f %f %f %f\n", rect.x, rect.y, rect.w, rect.h);
    SDL_SetRenderDrawColor(GLOBAL_ENGINE->renderer, 255, 0, 0, 255);
    SDL_RenderFillRectF(GLOBAL_ENGINE->renderer, &rect);
}