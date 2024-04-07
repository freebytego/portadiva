#include "include/engine/generic_renderer.h"
#include "include/engine/engine.h"

int engine_generic_renderer_create(generic_renderer_t** out)
{
    generic_renderer_t* renderer = (generic_renderer_t*)malloc(sizeof(generic_renderer_t));
    if (NULL == renderer)
    {
        fprintf(stderr, "failed to allocate a generic renderer");
        return -1;
    }
    if (node_list_create(&renderer->queue) != 0)
    {
        free(renderer);
        fprintf(stderr, "failed to create a generic renderer queue");
        return -1;
    }

    *out = renderer;

    return 0;
}

int engine_generic_renderer_create_renderable(generic_renderable_t** out)
{
    generic_renderable_t* renderable = (generic_renderable_t*)malloc(sizeof(generic_renderable_t));
    if (NULL == renderable)
    {
        fprintf(stderr, "failed to allocate a generic renderable");
        return -1;
    }
    *out = renderable;
    return 0;
}

void engine_generic_renderer_add_to_queue(generic_renderer_t* renderer, generic_renderable_t* renderable) 
{
    if (node_list_add_node(renderer->queue, renderable) != 0)
    {
        fprintf(stderr, "failed to append a renderable (%p) to the generic renderer queue", renderable);
        return;
    }
}

int sort_renderables(const void* a, const void* b)
{
    generic_renderable_t* renderableA = *(generic_renderable_t**)a;
    generic_renderable_t* renderableB = *(generic_renderable_t**)b;
    return (renderableA->order) - (renderableB->order);
}

void engine_generic_renderer_render(generic_renderer_t* renderer)
{
    if (renderer->queue->length > 0)
    {
        generic_renderable_t** queue = (generic_renderable_t**)malloc(renderer->queue->length * sizeof(generic_renderable_t*));
        if (NULL == queue)
        {
            fprintf(stderr, "failed to create generic renderable queue\n");
            return;
        }
        list_node_t* head = renderer->queue->begin;
        int index = 0;
        while (NULL != head)
        {
            generic_renderable_t* renderable = (generic_renderable_t*)head->data;
            queue[index] = renderable;
            head = head->next;
            index++;
        }
        qsort(queue, renderer->queue->length, sizeof(generic_renderable_t*), &sort_renderables);
        for (int i = 0; i < renderer->queue->length; ++i)
        {
            generic_renderable_t* renderable = queue[i];
            switch (renderable->type)
            {
                case GAME_OBJECT_RENDER:
                {
                    SDL_FRect rect;
                    rect.w = renderable->gameObject->renderProperties.width;
                    rect.h = renderable->gameObject->renderProperties.height;

                    switch (renderable->gameObject->renderProperties.offsetType)
                    {
                        case RENDER_OFFSET_TOP_LEFT:
                        {
                            rect.x = renderable->gameObject->position.x;
                            rect.y = renderable->gameObject->position.y;
                            break;
                        }
                        case RENDER_OFFSET_CENTER:
                        {
                            rect.x = renderable->gameObject->position.x - rect.w / 2.0f;
                            rect.y = renderable->gameObject->position.y - rect.h / 2.0f;
                        }
                    }
                    rect.x += renderable->gameObject->renderProperties.offset.x;
                    rect.y += renderable->gameObject->renderProperties.offset.y;
                    if (NULL == renderable->gameObject->texturePart)
                    {
                        glDisable(GL_TEXTURE_2D);
                        glBegin(GL_QUADS);

                        glColor4f(1.0f, 1.0f, 0.0f, 1.0f); glVertex2f(rect.x, rect.y);
                        glColor4f(1.0f, 1.0f, 0.0f, 1.0f); glVertex2f(rect.x + rect.w, rect.y);
                        glColor4f(1.0f, 1.0f, 0.0f, 1.0f); glVertex2f(rect.x + rect.w, rect.y + rect.h);
                        glColor4f(1.0f, 1.0f, 0.0f, 1.0f); glVertex2f(rect.x, rect.y + rect.h);

                        glEnd();
                        glEnable(GL_TEXTURE_2D);
                        continue;
                    }

                    texture_position_t position = texture_manager_get_texture_position_from_texture_part(renderable->gameObject->texturePart);

                    glEnable(GL_TEXTURE_2D);
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glBindTexture(GL_TEXTURE_2D, renderable->gameObject->texturePart->texture->textureId);

                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    glPushMatrix();

                    glTranslatef(renderable->gameObject->position.x, renderable->gameObject->position.y, 0);
                    glRotatef(renderable->gameObject->renderProperties.angle, 0, 0, 1.0);
                    glTranslatef(-renderable->gameObject->position.x, -renderable->gameObject->position.y, 0);

                    glBegin(GL_TRIANGLE_FAN);

                    glColor4f(1.0f, 1.0f, 1.0f, renderable->gameObject->renderProperties.opacity);
                    glTexCoord2f(position.x1, position.y1); glVertex2f(rect.x, rect.y);
                    glTexCoord2f(position.x2, position.y1); glVertex2f(rect.x + rect.w, rect.y);
                    glTexCoord2f(position.x2, position.y2); glVertex2f(rect.x + rect.w, rect.y + rect.h);
                    glTexCoord2f(position.x1, position.y2); glVertex2f(rect.x, rect.y + rect.h);

                    glEnd();
                    
                    glPopMatrix();
                    break;
                }
                case FUNCTION_RENDER: {
                    renderable->render_function(renderable->renderFunctionTarget);
                    break;
                }
                default: {
                    fprintf(stderr, "invalid render type for the generic renderer has been passed : %d\n", (int)renderable->type);
                    break;
                }
            }
        }
        free(queue);
    }
}

void engine_generic_renderer_clear_queue(generic_renderer_t* renderer)
{
    list_node_t* head = renderer->queue->begin;
    while (NULL != head)
    {
        free(head->data);
        head = head->next;
    }
    node_list_reset(renderer->queue);
}

void engine_generic_renderer_free(generic_renderer_t* renderer)
{
    node_list_free(renderer->queue);
    free(renderer);
}