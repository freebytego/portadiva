#include <stdio.h>
#include "include/game/dsc/dsc.h"
#include "include/game/objects/rhythm_controller.h"
#include "include/game/objects/target_effect_renderer.h"
#include "include/engine/engine.h"
#include "include/engine/scene_object.h"
#include "include/engine/texture_manager.h"
#include <SDL2/SDL.h>

#ifdef __PSP__

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

int exit_callback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp)
{
    int cbid = sceKernelCreateCallback("Exit Callback",
        exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void)
{
    int thid = sceKernelCreateThread("update_thread",
        callback_thread, 0x11, 0xFA0, 0, 0);

    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

#endif

int main(int argc, char** argv) {

#ifdef __PSP__
    setup_callbacks();
    pspDebugScreenInit();
#endif
    engine_t* engine;
    if (engine_create(&engine) != 0)
    {
        return -1;
    }

    texture_manager_t* textureManager;
    if (texture_manager_create(&textureManager, "textures") != 0)
    {
        engine_free(engine);
        return -1;
    }
    if (texture_manager_add_texture(textureManager, "buttons.json") != 0)
    {
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }
    if (texture_manager_add_texture(textureManager, "button_effects.json") != 0)
    {
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }

    engine_set_scene(engine, &scene_object_create, &scene_object_free);

    game_rhythm_controller_t* controller;
    if (game_rhythm_controller_create_from_path(&controller, "./test.dsc") != 0)
    {
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }
    if (game_object_add_child(engine->scene->object, controller->object) != 0)
    {
        game_rhythm_controller_free(controller);
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }

    game_target_effect_renderer_t* targetEffectRenderer;
    if (game_target_effect_renderer_create(&targetEffectRenderer) != 0)
    {
        game_rhythm_controller_free(controller);
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }
    if (game_object_add_child(engine->scene->object, targetEffectRenderer->object) != 0)
    {
        game_target_effect_renderer_free(targetEffectRenderer);
        game_rhythm_controller_free(controller);
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }

    game_rhythm_controller_set_target_effect_renderer(controller, targetEffectRenderer);

    game_target_real_renderer_t* targetRealRenderer;
    if (game_target_real_renderer_create(&targetRealRenderer) != 0)
    {
        game_rhythm_controller_free(controller);
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }
    if (game_object_add_child(engine->scene->object, targetRealRenderer->object) != 0)
    {
        game_target_real_renderer_free(targetRealRenderer);
        game_rhythm_controller_free(controller);
        texture_manager_free(textureManager);
        engine_free(engine);
        return -1;
    }
    game_rhythm_controller_set_target_real_renderer(controller, targetRealRenderer);

    // psp can't handle my crap code, limiting to 30 makes it way more consistant.
    // I need to optimize stuff so bad !!!!
    const int fps = 30;
    const int frameDelay = 1000 / fps;

    uint32_t frameStart;
    int frameTime;

    while (engine->running)
    {
        frameStart = SDL_GetTicks();
        engine_cycle(engine);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    texture_manager_free(textureManager);
    engine_free(engine); // engine frees the scene and scene frees all the game objects (their implementation)

    return 0;
}
