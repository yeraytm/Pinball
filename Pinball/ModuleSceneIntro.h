#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> boardParts;

	PhysBody* ball;
	p2List<PhysBody*> pointBalls;
	p2List<PhysBody*> starBalls;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* spriteSheet;
	// board: x = 0 y = 83 w = 405 h = 677
	// portal: x = 416 y = 258 w = 340 h = 369

	// ball: x = 10 y = 25 w = h = 30

	// ball point blue: x = 312 y = 2 w = 75 h = 75
	// ball point orange: x = 393 y = 2 w = 75 h = 75
	// ball star idle: x = 477 y = 2 w = 75 h = 75
	// ball star hit: x = 558 y = 2 w = 75 h = 75

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
