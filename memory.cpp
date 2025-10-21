#include "pch.h"
#include "memory.h"


//reach inside Hook struct to acces our Hook constructor and define that function
Hook::Hook(BYTE* hookPosition, BYTE* desiredFunction, int lenghtOfHook)
{
	//use arguments to define our Hook's variables
	this->hookPosition = hookPosition;
	this->desiredFunction = desiredFunction;
	this->lenghtOfHook = lenghtOfHook;

	//copy stolen bytes
	memcpy(stolenBytes, hookPosition, lenghtOfHook);

	//allocate room for our trampoline
	trampoline = (BYTE*)VirtualAlloc(nullptr, 30, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//setup tramp jumps
	hookToTrampJump = (DWORD)(trampoline - hookPosition) - 5;
	returnJump = (DWORD)(hookPosition + lenghtOfHook) - (DWORD)(trampoline + lenghtOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack)) - 5;

	//setup detour jumps
	hookToDetourJump = (DWORD)(desiredFunction - hookPosition) - 5;
	returnJumpDetour = (DWORD)(hookPosition + lenghtOfHook);
}

Hook::Hook()
{
	//do nothing
}

void Hook::ToggleTrampSBF()
{
	//toggle hook
	bActive = !bActive;

	if (bActive)
	{
		//making it so that we can execute/read/write bytes at hook location
		DWORD oldProtection;
		VirtualProtect(hookPosition, lenghtOfHook, PAGE_EXECUTE_READWRITE, &oldProtection);

		//is the trmapoline already built
		if (bTrampBuilt)goto TrampAlreadyBuilt;
		
		//in our trampoline, jump to our space in the DLL for specified function
		trampToFuncCall = (DWORD)desiredFunction - (DWORD)(trampoline + lenghtOfHook + sizeof(preserveStack)) - 5;

		//stolen bytes placed, stack preserved, call our desired function
		memcpy(trampoline, (void*)hookPosition, lenghtOfHook);
		memcpy(trampoline + lenghtOfHook, preserveStack, sizeof(preserveStack));
		memset(trampoline + lenghtOfHook + sizeof(preserveStack), 0xE8, 1);
		*(DWORD*)(trampoline + lenghtOfHook + sizeof(preserveStack) + 1) = trampToFuncCall;

		//release stack, jump back to after hook
		memcpy(trampoline + lenghtOfHook + sizeof(preserveStack) + 5, releaseStack, sizeof(releaseStack));
		memset(trampoline + lenghtOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack), 0xE9, 1);
		*(DWORD*)(trampoline + lenghtOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack) + 1) = returnJump;

		//trampoline is built
		bTrampBuilt = true;

	TrampAlreadyBuilt:
		//set hook inside game's memory
		memset(hookPosition, 0x90, lenghtOfHook);
		*(BYTE*)hookPosition = 0xE9;
		*(DWORD*)(hookPosition + 1) = hookToTrampJump;

		//restore protections
		VirtualProtect(hookPosition, lenghtOfHook, oldProtection, nullptr);
	}

	if (!bActive)
	{
		DWORD oldProtection;
		VirtualProtect(hookPosition, lenghtOfHook, PAGE_EXECUTE_READWRITE, &oldProtection);
		memcpy(hookPosition, stolenBytes, lenghtOfHook);
		VirtualProtect(hookPosition, lenghtOfHook, oldProtection, nullptr);
	}
}

void Hook::ToggleDetour()
{
	//toggle hook
	bActive = !bActive;

	if (bActive) {
		//making it so that we can read/write bytes at hook location
		DWORD oldProtection;
		VirtualProtect(hookPosition, lenghtOfHook, PAGE_EXECUTE_READWRITE, &oldProtection);

		//set detour hook inside game's memory. the detour jumps back for us
		memset(hookPosition, 0x90, lenghtOfHook);
		*(BYTE*)hookPosition = 0xE9;
		*(DWORD*)(hookPosition + 1) = hookToDetourJump;

		//resotre protections
		VirtualProtect(hookPosition, lenghtOfHook, oldProtection, nullptr);
	}

	if (!bActive)
	{
		DWORD oldProtection;
		VirtualProtect(hookPosition, lenghtOfHook, PAGE_EXECUTE_READWRITE, &oldProtection);
		memcpy(hookPosition, stolenBytes, lenghtOfHook);
		VirtualProtect(hookPosition, lenghtOfHook, oldProtection, nullptr);
	}
}

Nop::Nop(BYTE* nopPosition, int lenght)
{
	this->nopPosition = nopPosition;
	this->lenght = lenght;
	memcpy(stolenBytes, nopPosition, lenght);
}

Nop::Nop()
{
	//do nothing 
}

void Nop::ToggleNop()
{
	//toggle NOP
	bActive = !bActive;

	if (bActive)
	{
		DWORD oldProtection;
		VirtualProtect(nopPosition, lenght, PAGE_EXECUTE_READWRITE, &oldProtection);
		memset(nopPosition, 0x90, lenght);
		VirtualProtect(nopPosition, lenght, oldProtection, &oldProtection);
	}

	if (!bActive)
	{
		DWORD oldProtection;
		VirtualProtect(nopPosition, lenght, PAGE_EXECUTE_READWRITE, &oldProtection);
		memcpy(nopPosition, stolenBytes, lenght);
		VirtualProtect(nopPosition, lenght, oldProtection, &oldProtection);
	}
}