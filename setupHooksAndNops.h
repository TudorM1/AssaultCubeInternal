#pragma once
#include "memory.h"

extern Hook mainHackLoopTramp;
extern Hook infAmmoDetour;
extern Nop infAmmoNOP;

void SetupHooksAndNops();

