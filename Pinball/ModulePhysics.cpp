#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/DebugX/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

PhysBody::~PhysBody()
{
	body->GetWorld()->DestroyBody(body);
}

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics() {}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.8f;
	fixture.restitution = 0.1f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateStaticCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2BodyType::b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, float32 restitution)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	delete[] p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* rotationPivot, PhysBody* pBody, int anchorBx, int anchorBy, int upperAngleLimit, int lowerAngleLimit)
{
	b2RevoluteJointDef revolutionDef;
	revolutionDef.bodyA = rotationPivot->body;
	revolutionDef.bodyB = pBody->body;
	revolutionDef.collideConnected = false;
	revolutionDef.localAnchorA.Set(0, 0);
	revolutionDef.localAnchorB.Set(PIXEL_TO_METERS(anchorBx), PIXEL_TO_METERS(anchorBy));
	revolutionDef.enableLimit = true;
	revolutionDef.upperAngle = upperAngleLimit * DEGTORAD;
	revolutionDef.lowerAngle = lowerAngleLimit * DEGTORAD;
	revolutionDef.enableMotor = true;
	revolutionDef.motorSpeed = 2;
	revolutionDef.maxMotorTorque = 400;

	return (b2RevoluteJoint*)App->physics->world->CreateJoint(&revolutionDef); // Save joint in a var included in struct of flipper
}

update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		distanceJoint = !distanceJoint;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				PhysBody* body = (PhysBody*)b->GetUserData();
				if (body != NULL)
				{
					if (body->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
					{
						mouse_position = { PIXEL_TO_METERS((float32)App->input->GetMouseX()) , PIXEL_TO_METERS((float32)App->input->GetMouseY()) };
						body_clicked = body->body;
						break;
					}
				}
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (distanceJoint == false)
	{
		if (body_clicked != NULL && mouse_joint == NULL)
		{
			b2MouseJointDef def;
			def.bodyA = ground;
			def.bodyB = body_clicked;
			def.target = mouse_position;
			def.dampingRatio = 0.5f;
			def.frequencyHz = 2.0f;
			def.maxForce = 100.0f * body_clicked->GetMass();

			mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
		}
	}
	else
	{
		if (body_clicked != NULL && distance_joint == NULL)
		{
			b2DistanceJointDef def;

			b2BodyDef body;
			body.type = b2_staticBody;
			body.position.Set(body_clicked->GetPosition().x, body_clicked->GetPosition().y);
			b2Body* b = world->CreateBody(&body);
			def.length = PIXEL_TO_METERS(50.0f);

			def.bodyA = b;
			def.bodyB = body_clicked;
			/*def.target = mouse_position;*/
			def.dampingRatio = 1.0f;
			def.frequencyHz = 0.0f;
			/*def.maxForce = 100.0f * body_clicked->GetMass();*/

			distance_joint = (b2DistanceJoint*)world->CreateJoint(&def);
		}
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != NULL)
	{
		b2Vec2 mousePos = { PIXEL_TO_METERS((float32)App->input->GetMouseX()), PIXEL_TO_METERS((float32)App->input->GetMouseY()) };
		mouse_joint->SetTarget(mousePos);

		App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetAnchorA().x), METERS_TO_PIXELS(mouse_joint->GetAnchorA().y), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 0, 0);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && distance_joint != NULL)
	{
		b2Vec2 mousePos = { PIXEL_TO_METERS((float32)App->input->GetMouseX()), PIXEL_TO_METERS((float32)App->input->GetMouseY()) };
		distance_joint->GetBodyA()->SetTransform(mousePos, distance_joint->GetBodyA()->GetAngle());
		distance_joint->SetLength(3);

		App->renderer->DrawLine(METERS_TO_PIXELS(distance_joint->GetAnchorA().x), METERS_TO_PIXELS(distance_joint->GetAnchorA().y), METERS_TO_PIXELS(distance_joint->GetAnchorB().x), METERS_TO_PIXELS(distance_joint->GetAnchorB().y), 255, 0, 0);
	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != NULL)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = NULL;
		body_clicked = NULL;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && distance_joint != NULL)
	{
		world->DestroyJoint(distance_joint);
		distance_joint = NULL;
		body_clicked = NULL;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

int PhysBody::GetPosX() const
{
	int x = METERS_TO_PIXELS(body->GetPosition().x) - (width);
	return x;
}

int PhysBody::GetPosY() const
{
	int y = METERS_TO_PIXELS(body->GetPosition().y) - (height);
	return y;
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}

		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::DestroyJoints()
{
	world->DestroyJoint(distance_joint);
	distance_joint = NULL;

	world->DestroyJoint(mouse_joint);
	mouse_joint = NULL;

	body_clicked = NULL;
}