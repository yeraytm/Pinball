#include "ModuleFlipper.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"

ModuleFlipper::ModuleFlipper(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}
ModuleFlipper::~ModuleFlipper()
{

}

bool ModuleFlipper::Start()
{
	LOG("Loading Flipper");
	bool ret = true;
	flipperTex = App->textures->Load("pinball/Spritesheet.png");
	flipperLeftRect = { 55,27,83,32 };
	flipperRightRect = { 193,18,109,44 };
	CreateLeftFlipper();
	return ret;
}

void ModuleFlipper::CreateLeftFlipper() {
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
	leftFlipper.rotationPivot = App->physics->CreateStaticCircle(initialPosLeft.x-6, initialPosLeft.y-14, 5);
	leftFlipper.revoluteJoint = App->physics->CreateRevoluteJoint(leftFlipper.rotationPivot, leftFlipper.pBody, 50,30, 30, -50);
}


update_status ModuleFlipper::PreUpdate() {

	return UPDATE_CONTINUE;
}

update_status ModuleFlipper::Update() {
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		//leftFlipper.pBody->body->ApplyAngularImpulse(-9.0f, true);
		leftFlipper.revoluteJoint->SetMotorSpeed(-400);
	}
	else
	{
		//leftFlipper.pBody->body->ApplyAngularImpulse(0.2f, true);
		leftFlipper.revoluteJoint->SetMotorSpeed(10);
	}

	
	leftFlipper.pBody->GetPosition(actualPositionL.x, actualPositionL.y);
	actualPositionL.x += 55;
	actualPositionL.y += 25;

	App->renderer->Blit(flipperTex, actualPositionL.x,actualPositionL.y , &flipperLeftRect, 1.0f, leftFlipper.pBody->GetRotation(),-55,-25);
	//App->renderer->Blit(flipperTex, positionRightFlipper.x, positionRightFlipper.y, &flipperRightRect, 1.0f, rightFlipper.pBody->GetRotation());
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleFlipper::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(flipperTex);
	return true;
}

void ModuleFlipper::LeftMovement() {
	leftFlipper.revoluteJoint->SetMotorSpeed(-20);
}
