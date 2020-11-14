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
	// Scene bodies
	p2List<PhysBody*> boardParts;

	PhysBody* ball;
	PhysBody* pointBall;
	PhysBody* starBall;

	PhysBody* sensor;
	PhysBody* sensor2;
	PhysBody* sensor3;

	p2List<PhysBody*> pointBalls;
	p2List<PhysBody*> starBalls;

	SDL_Texture* spriteSheet;

	// Rects to blit
	SDL_Rect boardRect; // board: x = 0 y = 83 w = 405 h = 677
	SDL_Rect boardPortalRect; // portal: x = 416 y = 258 w = 340 h = 369

	SDL_Rect blueBallRect; // ball point blue: x = 645 y = 4 w = 37 h = 37
	SDL_Rect orangeBallRect; // ball point orange: x = 682 y = 4 w = 37 h = 37

	SDL_Rect starBallRect; // ball star idle: x = 722 y = 4 w = 37 h = 37
	SDL_Rect starBallHitRect; // ball star hit: x = 759 y = 4 w = 37 h = 37

	SDL_Rect bigStarBallRect; // big ball star idle: x = 477 y = 2 w = 77 h = 77
	SDL_Rect bigStarBallHitRect; // big ball star hit: x = 557 y = 2 w = 77 h = 77

	uint pointsFx;
	uint bumpFx;
	uint flipperFx;

	p2Point<int> ray;
	bool ray_on;
};
