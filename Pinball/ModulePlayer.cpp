#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ballTex = App->textures->Load("pinball/Spritesheet.png");
	ballRec = {16,31,18,18};
	ball = App->physics->CreateCircle(SCREEN_WIDTH-23, SCREEN_HEIGHT/2, 9);
	ball->body->SetBullet(true);
	ball->listener = this;
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ballTex, x, y, &ballRec, 1.0f, ball->GetRotation());
	
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(ballTex);
	return true;
}

