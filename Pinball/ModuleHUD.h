﻿#pragma once
#ifndef __MODULE_HUD_H__
#define __MODULE_HUD_H__

#include "Module.h"
#include "SDL/include/SDL.h"

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

	SDL_Texture* soundTex;
	SDL_Rect soundOnRect;
	SDL_Rect soundOffRect;

	SDL_Texture* logoTex;
	SDL_Rect logoRect;
	SDL_Texture* upcTex;
	SDL_Rect upcRect;
};

#endif // __MODULE_HUD_H__