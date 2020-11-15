#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	uint GetLifes() { return lifes; }

public:
	PhysBody* ball;

	SDL_Rect ballRect;
	
	uint numLives = 5;
	bool gameOver = false;

	uint lifes;
};