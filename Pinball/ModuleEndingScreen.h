#pragma once
#ifndef __MODULE_ENDING_SCREEN_H__
#define __MODULE_ENDING_SCREEN_H__

#include "Module.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class ModuleEndingScreen : public Module
{
public:
	ModuleEndingScreen(Application* app, bool startEnabled = true);
	virtual ~ModuleEndingScreen();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* tex = nullptr;
	SDL_Rect screen;
	char lookupTable[99] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;:$#'! /?%&()@          " };
	int font = -1;

};
#endif // __MODULE_ENDING_SCREEN_H__