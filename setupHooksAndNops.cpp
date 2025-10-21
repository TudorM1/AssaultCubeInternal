#include "pch.h"
#include "setupHooksAndNops.h"
#include "mainHackLoop.h"
#include "infAmmo.h"

//declase all hooks and NOPs
Hook mainHackLoopTramp;
Hook infAmmoDetour;
Nop infAmmoNOP;

void SetupHooksAndNops()
{
	//make sure we have our module bases
	BYTE* moduleBaseAssaultCube = (BYTE*)GetModuleHandle(L"ac_client.exe");
	BYTE* moduleBaseOpenGl = (BYTE*)GetModuleHandle(L"opengl32.dll");


	//make temporary hooks that will feed their information into the blank declarations of our global hooks
	//main hack loop hook
	BYTE* mainHackLoopHookDst = moduleBaseOpenGl + 0x47ADE;
	Hook mainHackLoopTrampTemp(mainHackLoopHookDst, (BYTE*)MainHackLoop, 5);
	mainHackLoopTramp = mainHackLoopTrampTemp;

	//infinite ammo detour
	BYTE* infAmmoDetourDst = moduleBaseAssaultCube + 0x637E6;
	Hook infAmmoDetourTemp(infAmmoDetourDst, (BYTE*)InfAmmo, 5);
	infAmmoDetour = infAmmoDetourTemp; 

	//infinite ammo Nop
	BYTE* infAmmoNopDst = moduleBaseAssaultCube + 0x637E9;
	Nop infAmmoNopTemp(infAmmoNopDst, 2);
	infAmmoNOP = infAmmoNopTemp;
}
