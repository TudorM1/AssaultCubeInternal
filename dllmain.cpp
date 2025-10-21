// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>//for console
#include <iostream>//for console printing
#include "mainHackLoop.h"
#include "setupHooksAndNops.h"

void WINAPI HackThread(HMODULE hModule)
{
    //create console 
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    //setup our hoosk and NOPs
    SetupHooksAndNops();

    //toggle the hooks we want
    mainHackLoopTramp.ToggleTrampSBF();

    std::cout << "Press DELETE for infAmmoDetour" << std::endl;
    std::cout << "Press PG_UP for infAmmoNOP" << std::endl;
    std::cout << "Press INSERT to uninject" << std::endl;


    while (!bBreakHackThreadWhileLoop)
    {
        MainHackLoop();
    }

    if (infAmmoDetour.bActive)infAmmoDetour.ToggleDetour();
    if (infAmmoNOP.bActive)infAmmoNOP.ToggleNop();

    //release console
    fclose(f);
    FreeConsole();

    //unload library, close thread safely
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

