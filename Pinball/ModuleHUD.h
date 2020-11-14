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
	char lookupTable[99] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;:$#'! /?%&()@          "};
	int font = -1;

};