#pragma once


struct Hook
{
	//get to our hook
	BYTE* hookPosition{ nullptr };
	//store stolen bytes
	BYTE stolenBytes[20]{ 0 };
	int lenghtOfHook;
	//is hook active?
	bool bActive = false;


	//tramp hook setup
	BYTE* desiredFunction{ nullptr };
	DWORD hookToTrampJump;//jump at the hook to our trampoline
	DWORD trampToFuncCall;//call our function from the trampoline
	DWORD returnJump;//jump at the end of our trampoline back to the flow of execution
	//trampoline placement
	BYTE* trampoline{ nullptr };
	//dont corrupt stack
	BYTE preserveStack[2]{ 0x60,0x9C };//pushad,pushfd
	BYTE releaseStack[2]{ 0x9D, 0x61 };//popfd,popad
	bool bTrampBuilt = false;

	//detour hook setup
	DWORD hookToDetourJump;
	DWORD returnJumpDetour;

	//functionality
	void ToggleTrampSBF();
	void ToggleTrampSBL();
	void ToggleDetour();

	//constructors
	Hook(BYTE* hookPosition, BYTE* desiredFunction, int lenghtOfHook);
	Hook();
};

struct Nop
{
	//where at and for how long?
	BYTE* nopPosition{ nullptr };
	int lenght{ 0 };
	//save for toggling off
	BYTE stolenBytes[30]{ 0 };
	//used by function to know wheter to store original bytes, NOP, or replace nopPosition wwith stolenBytes
	bool bActive = false;
	//functionality
	void ToggleNop();
	//construcotr
	Nop(BYTE* nopPosition, int lenght);
	Nop();
};

