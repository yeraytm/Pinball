#pragma once
#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL) {}
	~PhysBody();

	void GetPosition(int& x, int& y) const;
	int GetPosX() const;
	int GetPosY() const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;

	bool pendingToDelete;
	bool pendingToDelete2;
	bool pendingToDelete3;

	bool hit = false;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateStaticCircle(int x, int y, int radius);
	PhysBody* CreateCircleSensor(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, float32 restitution = 0);
	PhysBody* CreatePolygon(int x, int y, int* points, int size);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* rotationPivot, PhysBody* pBody, int anchorBx, int anchorBy, int upperAngleLimit, int lowerAngleLimit);
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	void DestroyJoints();

private:
	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

	b2Body* body_clicked;
	b2Vec2 mouse_position;

	b2DistanceJoint* distance_joint;
	bool distanceJoint = false;
};

#endif // __MODULE_PHYSICS_H__