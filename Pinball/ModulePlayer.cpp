#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled) {}
ModulePlayer::~ModulePlayer() {}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	ballRect = { 16, 31, 18, 18 };

	ball = App->physics->CreateCircle(SCREEN_WIDTH-23, SCREEN_HEIGHT/2+215, 9);
	ball->body->SetBullet(true);
	ball->listener = this;

	lifes = 5;

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (ball != nullptr && ball->pendingToDelete == true)
	{
		ball->pendingToDelete = false;

		b2Vec2 position;
		position.Set(PIXEL_TO_METERS(65.0f), PIXEL_TO_METERS(455.0f));
		ball->body->SetTransform(position, ball->GetRotation());

		b2Vec2 velocity;
		velocity.Set(1.0f, 1.0f);
		ball->body->SetLinearVelocity(velocity);
	}

	if (ball != nullptr && ball->pendingToDelete2 == true)
	{
		ball->pendingToDelete2 = false;

		b2Vec2 position;
		position.Set(PIXEL_TO_METERS(310.0f), PIXEL_TO_METERS(520.0f));
		ball->body->SetTransform(position, ball->GetRotation());

		b2Vec2 velocity;
		velocity.Set(-1.0f, 1.0f);
		ball->body->SetLinearVelocity(velocity);
	}

	if (ball != nullptr && ball->pendingToDelete3 == true)
	{
		ball->pendingToDelete3 = false;

		b2Vec2 position;
		position.Set(PIXEL_TO_METERS(float(SCREEN_WIDTH - 23)), PIXEL_TO_METERS(float(SCREEN_HEIGHT / 2+215)));
		ball->body->SetTransform(position, ball->GetRotation());

		b2Vec2 velocity;
		velocity.Set(1.0f, 1.0f);
		ball->body->SetLinearVelocity(velocity);
	}

	int x, y;
	ball->GetPosition(x, y);
	App->renderer->Blit(App->scene_intro->spritesheetTex, x, y, &ballRect, 1.0f, ball->GetRotation());
	
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	ball = nullptr;
	App->textures->Unload(App->scene_intro->spritesheetTex);

	return true;
}