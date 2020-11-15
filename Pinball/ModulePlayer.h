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

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	PhysBody* ball;

	SDL_Texture* ballTex;
	SDL_Rect ballRec;
	
	uint numLives = 5;
	bool gameOver = false;

	SDL_Texture* springTex;
	SDL_Rect springRec;

	Animation springStrechingDown;
	Animation springStrechingUp;

	bool strechingDown = false;
	bool strechingUp = false;
	bool alreadyKicked = true;

	uint lifes;
	uint springFx;
	uint flipperFx;
};