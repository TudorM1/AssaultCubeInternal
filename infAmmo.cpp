#include "pch.h"
#include "infAmmo.h"
#include "setupHooksAndNops.h"

void __declspec(naked)InfAmmo()
{
	__asm
	{
		mov esi, [esi + 0x14] // the only stolen bytes we wanna process
		inc[esi] // incrementing our ammo
		jmp infAmmoDetour.returnJumpDetour //using our instance's return jump to jump after after the hook
	}
}