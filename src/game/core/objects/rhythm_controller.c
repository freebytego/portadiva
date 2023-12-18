#include "include/game/core/objects/rhythm_controller.h"

int game_rhythm_controller_create_from_path(game_rhythm_controller_t** out, const char* filePath)
{
    game_rhythm_controller_t* controller = (game_rhythm_controller_t*)malloc(sizeof(game_rhythm_controller_t));
    if (NULL == controller)
    {
        perror("failed to allocate a game rhythm controller");
        return -1;
    }

    SDL_FPoint position;
    render_properties_t renderProperties;
    if (game_object_create(&controller->object, "rhythm controller", position, renderProperties) != 0)
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
    
    list_node_t* nextTimeElementNode = gameController->script->timeElements->begin;
    if (NULL != nextTimeElementNode)
    {
        dsc_time_element_t* nextTimeElement = (dsc_time_element_t*)nextTimeElementNode->data;
        printf("ticks: %d | next arrives at: %d | len: %d\n", gameController->ticks, (dsc_time_element_t*)gameController->script->timeElements->length, nextTimeElement->time);
        if (gameController->ticks > nextTimeElement->time)
        {
            list_node_t* targetNode = nextTimeElement->targets->begin;
            while (targetNode != NULL)
            {
                dsc_target_t* dscTarget = (dsc_target_t*)targetNode->data;
                printf("target at time %d (created at %d)\n", nextTimeElement->time, gameController->ticks);
                game_target_t* gameTarget;
                SDL_FPoint position;
                position.x = dscTarget->x * 480 / 480000.0f;
                position.y = dscTarget->y * 272 / 272000.0f + 50; // TODO: figure out the positioning
                game_target_create(&gameTarget, position);
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