#include "include/engine/engine.h"

engine_t* GLOBAL_ENGINE;

int engine_create(engine_t** out)
{
    engine_t* engine = (engine_t*)malloc(sizeof(engine_t));
    if (NULL == engine)
    {
        fprintf(stderr, "failed allocating engine");
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    engine->window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        480,
        272,
        0
    );
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);

    // TODO: validations here

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!( IMG_Init( imgFlags ) & imgFlags ))
    {
        // uh oh
    }

    engine->running = 1;

    *out = engine;
    GLOBAL_ENGINE = engine;
    return 0;
}

int engine_set_scene(
    engine_t* engine, 
    int (*create_scene_game_object)(scene_object_t**), 
    void (*free_scene_game_object)(scene_object_t*)
)
{
    if (NULL != engine->scene)
    {
        engine->free_scene(engine->scene);
    }
    if (create_scene_game_object(&engine->scene) != 0)
    {
        return -1;
    }
    engine->free_scene = free_scene_game_object;
    return 0;
}

void engine_cycle(engine_t* engine)
{
    scene_object_cycle(engine->scene);
    engine_handle_input(engine);
    engine_render(engine);
}

void engine_handle_input(engine_t* engine)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                engine->running = 0;
                break;
        }
    }
}

void engine_render(engine_t* engine)
{
    SDL_RenderClear(engine->renderer);

    scene_object_render(engine->scene);

    SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 255);
    SDL_RenderPresent(engine->renderer);
}

void engine_free_scene(engine_t* engine)
{
    if (NULL != engine->scene)
        engine->free_scene(engine->scene);
}

void engine_free(engine_t* engine)
{
    if (NULL != engine->scene)
        engine->free_scene(engine->scene);
    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
    free(engine);
}