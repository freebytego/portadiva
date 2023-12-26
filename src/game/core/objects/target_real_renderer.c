#include "include/game/core/objects/target_real_renderer.h"
#include "include/game/core/objects/target.h"
#include <time.h>

texture_part_t* connectedTargetTexturePart;
texture_position_t connectedTargetTexturePosition;

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

    connectedTargetTexturePart = texture_manager_find_texture_part_in_registered("textures", "buttons", "connected");
    connectedTargetTexturePosition = texture_manager_get_texture_position_from_texture_part(connectedTargetTexturePart);

    *out = renderer;

    return 0;
}

void game_target_real_renderer_render(game_target_real_renderer_t* renderer)
{
    // draw the trail and connected lines first
    list_node_t* child = renderer->object->children->begin;
    while (NULL != child)
    {
        game_object_t* object = (game_object_t*)child->data;
        game_target_real_t* targetReal = (game_target_real_t*)object->implementation;

        glDisable(GL_TEXTURE_2D);
        glColor4f(targetReal->trailColor[0], targetReal->trailColor[1], targetReal->trailColor[2], 0.1f);
        glBegin(GL_QUAD_STRIP);
        float thickness = 3.0f;
        for (int i = 0; i < MAX_TRAILS; ++i)
        {
            glVertex3f(targetReal->trails[0][i][0] - thickness, targetReal->trails[0][i][1], 0.0f);
            glVertex3f(targetReal->trails[0][i][0] + thickness, targetReal->trails[0][i][1], 0.0f);
            thickness -= thickness / MAX_TRAILS;
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < MAX_TRAILS; ++i)
        {
            glVertex3f(targetReal->trails[1][i][0], targetReal->trails[1][i][1], 0.0f);
        }
        glEnd();

        if (NULL != targetReal->target->connectedTarget)
        {
            SDL_FPoint normal;
            normal.x = targetReal->target->connectedTarget->real->object->position.y - targetReal->target->real->object->position.y;
            normal.y = -1 * (targetReal->target->connectedTarget->real->object->position.x - targetReal->target->real->object->position.x);
            float normalMagnitude = sqrt(normal.x * normal.x + normal.y * normal.y);
            normal.x /= normalMagnitude;
            normal.y /= normalMagnitude;
            // TODO: get rid of the random numbers
            normal.x *= 5.f;
            normal.y *= 5.f;
            glBindTexture(GL_TEXTURE_2D, targetReal->object->texturePart->texture->textureId);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
            glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
            glTexCoord2f(connectedTargetTexturePosition.x2, connectedTargetTexturePosition.y2);
            glVertex3f(targetReal->target->real->object->position.x - normal.x, targetReal->target->real->object->position.y - normal.y, 0.0f);
            glTexCoord2f(connectedTargetTexturePosition.x1, connectedTargetTexturePosition.y2);
            glVertex3f(targetReal->target->real->object->position.x + normal.x, targetReal->target->real->object->position.y + normal.y, 0.0f);
            glTexCoord2f(connectedTargetTexturePosition.x1, connectedTargetTexturePosition.y1);
            glVertex3f(targetReal->target->connectedTarget->real->object->position.x - normal.x, targetReal->target->connectedTarget->real->object->position.y - normal.y, 0.0f);
            glTexCoord2f(connectedTargetTexturePosition.x2, connectedTargetTexturePosition.y1);
            glVertex3f(targetReal->target->connectedTarget->real->object->position.x + normal.x, targetReal->target->connectedTarget->real->object->position.y + normal.y, 0.0f);
            glEnd();
        }
        child = child->next;
    }
    // then the target itself
    child = renderer->object->children->begin;
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
