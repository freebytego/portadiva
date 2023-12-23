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
    if (NULL == object->texturePart)
    {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        glColor3f(1.0f, 1.0f, 0.0f); glVertex2f(rect.x, rect.y);
        glColor3f(1.0f, 1.0f, 0.0f); glVertex2f(rect.x + rect.w, rect.y);
        glColor3f(1.0f, 1.0f, 0.0f); glVertex2f(rect.x + rect.w, rect.y + rect.h);
        glColor3f(1.0f, 1.0f, 0.0f); glVertex2f(rect.x, rect.y + rect.h);

        glEnd();
        glEnable(GL_TEXTURE_2D);
        return;
    }
    rect.x += object->renderProperties.offset.x;
    rect.y += object->renderProperties.offset.y;

    float texCoordX1 = (object->texturePart->source.x / (float)object->texturePart->texture->width);
    float texCoordY1 = (object->texturePart->source.y / (float)object->texturePart->texture->height);
    float texCoordX2 = ((object->texturePart->source.x + object->texturePart->source.w) / (float)object->texturePart->texture->width);
    float texCoordY2 = ((object->texturePart->source.y + object->texturePart->source.h) / (float)object->texturePart->texture->height);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, object->texturePart->texture->textureId);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    glTranslatef(object->position.x, object->position.y, 0);
    glRotatef(object->renderProperties.angle, 0, 0, 1.0);
    glTranslatef(-object->position.x, -object->position.y, 0);

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(1, 0, 0); glTexCoord2f(texCoordX1, texCoordY1); glVertex2f(rect.x, rect.y);
    glColor3f(0, 1, 0); glTexCoord2f(texCoordX2, texCoordY1); glVertex2f(rect.x + rect.w, rect.y);
    glColor3f(0, 0, 1); glTexCoord2f(texCoordX2, texCoordY2); glVertex2f(rect.x + rect.w, rect.y + rect.h);
    glColor3f(1, 1, 1); glTexCoord2f(texCoordX1, texCoordY2); glVertex2f(rect.x, rect.y + rect.h);

    glEnd();
    
    glPopMatrix();
}