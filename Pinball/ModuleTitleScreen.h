#pragma once
#ifndef __MODULETITLESCREEN_H__
#define __MODULETITLESCREEN_H__

#include "Module.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class ModuleTitleScreen : public Module
{
public:

	ModuleTitleScreen(Application* app, bool startEnabled = true);

	virtual ~ModuleTitleScreen();
	
	bool Start();

	update_status Update();

	bool CleanUp();

private:

	SDL_Texture* tex = nullptr;
	SDL_Rect screen;
};
#endif