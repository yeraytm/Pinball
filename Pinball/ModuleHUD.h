#pragma once
#include "Module.h"

class ModuleHUD : public Module
{
public:
	ModuleHUD(Application* app, bool start_enabled = true);
	~ModuleHUD();

	bool Start();
	update_status Update();
	bool CleanUp();
	char lookupTable[90] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890.,;:$#'! /?%&()@          "};
	int font = -1;

};