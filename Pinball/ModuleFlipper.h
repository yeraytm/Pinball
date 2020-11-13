#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

class PhysBody;
class b2RevoluteJoint;
class FlipperObject {
public:
	PhysBody* pBody = nullptr;
	PhysBody* rotationPivot = nullptr;
	b2RevoluteJoint* revoluteJoint = nullptr;
	bool isMoving = false;
};

class ModuleFlipper : public Module
{
public:
	ModuleFlipper(Application* app, bool start_enabled = true);
	virtual ~ModuleFlipper();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void CreateLeftFlipper();
	void CreateRightFlipper();
	void LeftMovement();
	void RightMovement();

private:
	SDL_Texture* flipperTex = nullptr;
	SDL_Rect flipperLeftRect,flipperRightRect;
	iPoint initialPosLeft = { 114,614 }, initialPosRight = { 296,614 };
	iPoint actualPositionL = { 0,0 }, actualPositionR = { 0,0 };
	FlipperObject leftFlipper,rightFlipper;

};