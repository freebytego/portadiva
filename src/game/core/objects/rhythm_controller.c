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
    game_object_set_cycle(controller->object, &game_rhythm_controller_cycle);
    game_object_set_render(controller->object, &game_rhythm_controller_render);
    controller->startedAt = SDL_GetTicks();

    *out = controller;

    return 0;
}

void game_rhythm_controller_cycle(void* controller)
{
    game_rhythm_controller_t* gameController = (game_rhythm_controller_t*)controller;
    gameController->ticks = (SDL_GetTicks() - gameController->startedAt) * 100;
    
    list_node_t* childNode = gameController->object->children->begin;
    while (NULL != childNode)
    {
        game_target_t* child = (game_target_t*)((game_object_t*)childNode->data)->implementation; // lmao
        childNode = childNode->next;
        if (child->finished == 1)
        {
            game_target_free(child);
        }
    }

    list_node_t* nextTimeElementNode = gameController->script->timeElements->begin;
    if (NULL != nextTimeElementNode)
    {
        dsc_time_element_t* nextTimeElement = (dsc_time_element_t*)nextTimeElementNode->data;
        if (gameController->ticks > nextTimeElement->time)
        {
            list_node_t* targetNode = nextTimeElement->targets->begin;
            while (targetNode != NULL)
            {
                dsc_target_t* dscTarget = (dsc_target_t*)targetNode->data;
                game_target_t* gameTarget;
                game_target_create(&gameTarget, dscTarget, nextTimeElement->flyingTime);
                game_object_add_child(gameController->object, gameTarget->object);
                targetNode = targetNode->next;
            }
            node_list_remove_node(gameController->script->timeElements, gameController->script->timeElements->begin);
            dsc_time_element_free(nextTimeElement);
        }
    }
}

void game_rhythm_controller_free(game_rhythm_controller_t* controller)
{
    dsc_script_free(controller->script);
    game_object_free(controller->object);
    free(controller);
}

void game_rhythm_controller_render(void* controller)
{
    game_rhythm_controller_t* gameController = (game_rhythm_controller_t*)controller;
    list_node_t* child = gameController->object->children->begin;
    while (NULL != child)
    {
        game_object_render((game_object_t*)child->data);
        child = child->next;
    }
}