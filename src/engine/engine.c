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
        SDL_WINDOW_OPENGL
    );
    if (NULL == engine->window)
    {
        free(engine);
        return -1;
    }

    engine->context = SDL_GL_CreateContext(engine->window);
    if (NULL == engine->context)
    {
        SDL_DestroyWindow(engine->window);
        free(engine);
        return -1;
    }

    SDL_SetWindowFullscreen(engine->window, SDL_FALSE);

    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 480, 272, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, 480, 272);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_GL_DeleteContext(engine->context);
        SDL_DestroyWindow(engine->window);
        free(engine);
        return -1;
    }

    populateLookupTables();

    if (engine_generic_renderer_create(&engine->renderer) != 0)
    {
        SDL_GL_DeleteContext(engine->context);
        SDL_DestroyWindow(engine->window);
        free(engine);
        return -1;
    }

    engine->running = 1;
    engine->scene = NULL;

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
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    engine_generic_renderer_clear_queue(engine->renderer);

    scene_object_render(engine->scene);

    engine_generic_renderer_render(engine->renderer);

    SDL_GL_SwapWindow(engine->window);
}

void engine_free_scene(engine_t* engine)
{
    if (NULL != engine->scene)
        engine->free_scene(engine->scene);
}

void engine_free(engine_t* engine)
{
    engine_generic_renderer_free(engine->renderer);
    if (NULL != engine->scene)
        engine->free_scene(engine->scene);
    SDL_DestroyWindow(engine->window);
    free(engine);
}