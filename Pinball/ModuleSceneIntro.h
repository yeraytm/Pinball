#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "SDL/include/SDL_Rect.h"

#include "Animation.h"

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

	uint GetScore() { return score; }
	int GetHighScore() { return highScore; }

public:
	// Scene bodies
	p2List<PhysBody*> boardParts;

	PhysBody* pointBall1;
	PhysBody* pointBall2;

	PhysBody* starBall1;
	PhysBody* starBall2;
	PhysBody* starBall3;
	
	PhysBody* bigStarBall1;

	PhysBody* sensor;
	PhysBody* sensor2;
	PhysBody* sensor3;
	PhysBody* sensor4;

	PhysBody* sensorRamp;

	SDL_Texture* board;
	SDL_Texture* boardPortal;
	SDL_Texture* spriteSheet;

	SDL_Texture* springTex;

	// Rects to blit
	SDL_Rect boardRect; // board: x = 0 y = 83 w = 405 h = 677
	SDL_Rect boardPortalRect; // portal: x = 416 y = 258 w = 340 h = 369

	SDL_Rect blueBallRect; // ball point blue: x = 645 y = 4 w = 37 h = 37
	SDL_Rect orangeBallRect; // ball point orange: x = 682 y = 4 w = 37 h = 37

	SDL_Rect starBallRect; // ball star idle: x = 722 y = 4 w = 37 h = 37
	SDL_Rect starBallHitRect; // ball star hit: x = 759 y = 4 w = 37 h = 37

	SDL_Rect bigStarBallRect; // big ball star idle: x = 477 y = 2 w = 77 h = 77
	SDL_Rect bigStarBallHitRect; // big ball star hit: x = 557 y = 2 w = 77 h = 77

	SDL_Rect springRec;
	Animation springStrechingDown;
	Animation springStrechingUp;

	uint pointsFx;
	uint bumpFx;
	uint rampFx;
	uint oneUpFx;

	uint springFx;
	uint flipperFx;

	uint score, highScore;

	p2Point<int> ray;
	bool ray_on;

	bool strechingDown = false;
	bool strechingUp = false;
	bool alreadyKicked = true;
};
