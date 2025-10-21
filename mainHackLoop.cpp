#include "pch.h"
#include "mainHackLoop.h"
#include "setupHooksAndNops.h"
#include <thread>

bool bBreakHackThreadWhileLoop = false;

void MainHackLoop()
{
	

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		mainHackLoopTramp.ToggleTrampSBF();
		bBreakHackThreadWhileLoop = true;
	}

	if (GetAsyncKeyState(VK_DELETE) & 1)
	{
		infAmmoDetour.ToggleDetour();
		std::cout << "Infinite Ammo Detour: " << (infAmmoDetour.bActive) << std::endl;
	}

	if (GetAsyncKeyState(VK_PRIOR) & 1)
	{
		infAmmoNOP.ToggleNop();
		std::cout << "Infinite Ammo NOP: " << (infAmmoNOP.bActive) << std::endl;
	}
}
