#include "Application.h"
#include "ModuleFlipper.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

ModuleFlipper::ModuleFlipper(Application* app, bool start_enabled) : Module(app, start_enabled) {}
ModuleFlipper::~ModuleFlipper() {}

bool ModuleFlipper::Start()
{
	LOG("Loading Flipper");
	bool ret = true;

	flipperLeftRect = { 55,27,83,32 };
	flipperRightRect = { 156,28,83,32 };
	CreateLeftFlipper();
	CreateRightFlipper();

	flipperFx = App->audio->LoadFx("pinball/audio/flipper.wav");

	return ret;
}

void ModuleFlipper::CreateLeftFlipper()
{
	// Pivot 0, 0
	int flipperLeftChain[16] = {
		56, 39,
		62, 29,
		124, 29,
		131, 33,
		130, 45,
		123, 50,
		65, 51,
		57, 44
	};

	leftFlipper.pBody = App->physics->CreatePolygon(initialPosLeft.x, initialPosLeft.y, flipperLeftChain, 16);
	leftFlipper.rotationPivot = App->physics->CreateStaticCircle(initialPosLeft.x, initialPosLeft.y - 14, 5);
	leftFlipper.revoluteJoint = App->physics->CreateRevoluteJoint(leftFlipper.rotationPivot, leftFlipper.pBody, 50,30, 30, -30);
	leftFlipper.pBody->listener = this;
}

void ModuleFlipper::CreateRightFlipper()
{
	// Pivot 0, 0
	int flipperRightChain[12] = {
		79, 15,
		73, 26,
		6, 24,
		4, 7,
		68, 3,
		78, 9
	};

	rightFlipper.pBody = App->physics->CreatePolygon(initialPosRight.x, initialPosRight.y, flipperRightChain, 12);
	rightFlipper.rotationPivot = App->physics->CreateStaticCircle(initialPosRight.x, initialPosRight.y - 14, 5);
	rightFlipper.revoluteJoint = App->physics->CreateRevoluteJoint(rightFlipper.rotationPivot, rightFlipper.pBody, 82, 5, 30, -30);
	rightFlipper.pBody->listener = this;
}

update_status ModuleFlipper::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleFlipper::Update()
{
	(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)? leftFlipper.revoluteJoint->SetMotorSpeed(-400) : leftFlipper.revoluteJoint->SetMotorSpeed(10);
	(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)?rightFlipper.revoluteJoint->SetMotorSpeed(400) : rightFlipper.revoluteJoint->SetMotorSpeed(-10);

	leftFlipper.pBody->GetPosition(actualPositionL.x, actualPositionL.y);
	actualPositionL.x += 55;
	actualPositionL.y += 25;
	App->renderer->Blit(App->scene_intro->spritesheetTex, actualPositionL.x,actualPositionL.y , &flipperLeftRect, 1.0f, leftFlipper.pBody->GetRotation(),-55,-25);

	rightFlipper.pBody->GetPosition(actualPositionR.x, actualPositionR.y);
	actualPositionR.x += 3;
	actualPositionR.y += 3;
	App->renderer->Blit(App->scene_intro->spritesheetTex, actualPositionR.x, actualPositionR.y, &flipperRightRect, 1.0f, rightFlipper.pBody->GetRotation(), -3,-3);

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleFlipper::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(App->scene_intro->spritesheetTex);

	return true;
}

void ModuleFlipper::LeftMovement()
{
	leftFlipper.revoluteJoint->SetMotorSpeed(-20);
}

void ModuleFlipper::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == App->player->ball)
	{
		App->audio->PlayFx(flipperFx);
	}
}