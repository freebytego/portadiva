#include "include/game/core/objects/rhythm_controller.h"

int game_rhythm_controller_create_from_path(game_rhythm_controller_t** out, const char* filePath)
{
    game_rhythm_controller_t* controller = (game_rhythm_controller_t*)malloc(sizeof(game_rhythm_controller_t));
    if (NULL == controller)
    {
        fprintf(stderr, "failed to allocate a game rhythm controller");
        return -1;
    }

    SDL_FPoint position;
    render_properties_t renderProperties;
    if (game_object_create(&controller->object, "rhythm controller", position, renderProperties, NULL) != 0)
    {
        free(controller);
        return -1;
    }

    if (dsc_script_create_from_file(&controller->script, filePath) != 0)
    {
        free(controller);
        return -1;
    }

    game_object_set_implementation(controller->object, controller);
    game_object_set_cycle(controller->object, (void (*)(void*))&game_rhythm_controller_cycle);
    game_object_set_render(controller->object, (void (*)(void*))&game_rhythm_controller_render);
    game_object_set_free(controller->object, (void (*)(void*))&game_rhythm_controller_free);

    controller->startedAt = SDL_GetTicks();

    *out = controller;

    return 0;
}

SDL_FPoint center;
// Thanks Project Heartbeat! <3
int sort_game_targets(const void* a, const void* b)
{
    const game_target_t* targetA = *(const game_target_t**)a;
    const game_target_t* targetB = *(const game_target_t**)b;
    if (targetA->object->position.x - center.x >= 0 && targetB->object->position.x - center.x < 0) return -1;
    if (targetA->object->position.x - center.x < 0 && targetB->object->position.x - center.x >= 0) return 1;
    if (targetA->object->position.x - center.x == 0 && targetB->object->position.x - center.x == 0) 
    {
        if (targetA->object->position.y - center.y >= 0 || targetB->object->position.y - center.y >= 0) return targetA->object->position.y > targetB->object->position.y ? 1 : -1;
        return targetA->object->position.y < targetB->object->position.y ? 1 : -1;
    }
    double cross = (targetA->object->position.x - center.x) * (targetB->object->position.y - center.y) - (targetB->object->position.x - center.x) * (targetA->object->position.y - center.y);
    if (cross < 0) return 1;
    if (cross > 0) return -1;

    float distanceA = (targetA->object->position.x - center.x) * (targetA->object->position.x - center.x) + (targetA->object->position.y - center.y) * (targetA->object->position.y - center.y);
    float distanceB = (targetB->object->position.x - center.x) * (targetB->object->position.x - center.x) + (targetB->object->position.y - center.y) * (targetB->object->position.y - center.y);
    return distanceA > distanceB ? 1 : -1;
}

void game_rhythm_controller_cycle(game_rhythm_controller_t* controller)
{
    controller->ticks = (SDL_GetTicks() - controller->startedAt) * 100;
    
    list_node_t* childNode = controller->object->children->begin;
    while (NULL != childNode)
    {
        game_target_t* child = (game_target_t*)((game_object_t*)childNode->data)->implementation; // lmao
        childNode = childNode->next;
        if (child->finished == 1)
        {
            game_target_free(child);
            // todo: create splash thingy
        }
    }

    list_node_t* nextTimeElementNode = controller->script->timeElements->begin;
    if (NULL != nextTimeElementNode)
    {
        dsc_time_element_t* nextTimeElement = (dsc_time_element_t*)nextTimeElementNode->data;
        if (controller->ticks > nextTimeElement->time)
        {
            list_node_t* targetNode = nextTimeElement->targets->begin;
            size_t currentTargetsCount = 0;
            while (targetNode != NULL)
            {
                dsc_target_t* dscTarget = (dsc_target_t*)targetNode->data;
                game_target_t* gameTarget;
                game_target_create(&gameTarget, dscTarget, nextTimeElement->flyingTime, nextTimeElement->targets->length > 1);
                game_object_add_child(controller->object, gameTarget->object);
                game_target_effect_renderer_spawn_create_effect(controller->targetEffectRenderer, gameTarget);
                game_target_create_target_real(gameTarget);
                if (currentTargetsCount < 4) controller->currentTargets[currentTargetsCount] = gameTarget;
                ++currentTargetsCount;
                targetNode = targetNode->next;
            }
            switch (currentTargetsCount)
            {
            case 2:
                controller->currentTargets[0]->connectedTarget = controller->currentTargets[1];
                goto switchDone;
            case 3:
            case 4:
            {
                center.x = 0.0f;
                center.y = 0.0f;
                for (int i = 0; i < currentTargetsCount; ++i)
                {
                    center.x += controller->currentTargets[i]->object->position.x;
                    center.y += controller->currentTargets[i]->object->position.y;
                }
                center.x /= currentTargetsCount;
                center.y /= currentTargetsCount;
                qsort(controller->currentTargets, currentTargetsCount, sizeof(game_target_t*), &sort_game_targets);
                for (int i = 0; i < currentTargetsCount - 1; ++i)
                {
                    controller->currentTargets[i]->connectedTarget = controller->currentTargets[i + 1];
                }
                controller->currentTargets[currentTargetsCount - 1]->connectedTarget = controller->currentTargets[0];
                goto switchDone;
            }
            default:
                goto switchDone;
            }
            switchDone:
            node_list_remove_node(controller->script->timeElements, controller->script->timeElements->begin);
            dsc_time_element_free(nextTimeElement);
        }
    }
}

void game_rhythm_controller_set_target_real_renderer(game_rhythm_controller_t* controller, game_target_real_renderer_t* renderer)
{
    controller->targetRealRenderer = renderer;
}

void game_rhythm_controller_set_target_effect_renderer(game_rhythm_controller_t* controller, game_target_effect_renderer_t* renderer)
{
    controller->targetEffectRenderer = renderer;
}

void game_rhythm_controller_free(game_rhythm_controller_t* controller)
{
    game_target_real_renderer_free(controller->targetRealRenderer);
    dsc_script_free(controller->script);
    game_object_free(controller->object);
    free(controller);
}

void game_rhythm_controller_render(game_rhythm_controller_t* controller)
{
    list_node_t* child = controller->object->children->begin;
    while (NULL != child)
    {
        game_object_render((game_object_t*)child->data);
        child = child->next;
    }
}