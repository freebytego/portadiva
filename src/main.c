#include <stdio.h>
#include "include/game/core/dsc/dsc.h"

#ifdef __PSP__

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

#define printf pspDebugScreenPrintf

PSP_MODULE_INFO("portadiva", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

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
    dsc_script_t* script;
    if (create_dsc_script_from_file(&script, "./test.dsc") != 0)
    {
        perror("fail");
        return -1;
    }

    list_node_t* timeElementNode = script->timeElements->begin;
    int y = 0;
    while (timeElementNode != NULL)
    {
        #ifdef __PSP__
            pspDebugScreenSetXY(0, y++);
            sceDisplayWaitVblankStart();
        #endif
        dsc_time_element_t* timeElement = ((dsc_time_element_t*)timeElementNode->data);
        printf("time: %d\n", timeElement->time);

        list_node_t* targetNode = timeElement->targets->begin;
        while (targetNode != NULL)
        {
            #ifdef __PSP__
                pspDebugScreenSetXY(0, y++);
                sceDisplayWaitVblankStart();
            #endif
            dsc_target_t* target = ((dsc_target_t*)targetNode->data);
            printf("target at %d : %d, %d, %d, %d, %d, %d, %d\n", target->time, target->type, target->x, target->y, target->angle, target->distance, target->amplitude, target->frequency);
            targetNode = targetNode->next;
        }

        timeElementNode = timeElementNode->next;
    }

    while (1)
    {
        
    }

    free_dsc_script(script);

    return 0;
}