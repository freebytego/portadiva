#include <stdio.h>
#include "include/game/core/dsc/dsc.h"
#include "include/game/core/objects/rhythm_controller.h"
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
        return -1;
    }
    if (texture_manager_add_texture(textureManager, "buttons.json") != 0)
    {
        texture_manager_free(textureManager);
        return -1;
    }

    engine_set_scene(engine, &scene_object_create, &scene_object_free);

    game_rhythm_controller_t* controller;
    game_rhythm_controller_create_from_path(&controller, "./test.dsc");

    game_object_add_child(engine->scene->object, controller->object);

    // while (engine->running)
    // {
    //     engine_cycle(engine);
    // }

    texture_manager_free(textureManager);
    engine_free(engine);

    return 0;
}