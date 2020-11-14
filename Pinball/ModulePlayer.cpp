#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	ballTex = App->textures->Load("pinball/Spritesheet.png");
	ballRec = { 16, 31, 18, 18 };

	ball = App->physics->CreateCircle(SCREEN_WIDTH-23, SCREEN_HEIGHT/2, 9);
	ball->body->SetBullet(true);
	ball->listener = this;

	springTex = App->textures->Load("pinball/spring.png");

	springFx = App->audio->LoadFx("pinball/firstBump.wav");

	for (int i = 0; i < 7; i++)
	{
		springStrechingDown.PushBack({ 20 * i, 0, 20, 64 });
	}
	springStrechingDown.speed = 0.5f;

	for (int i = 7; i < 13; i++)
	{
		springStrechingUp.PushBack({ 20 * i, 0, 20, 64 });
	}
	springStrechingUp.speed = 0.5f;

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (ball != nullptr && ball->pendingToDelete == true)
	{
		ball->pendingToDelete = false;
		delete ball;
		ball = nullptr;

		ball = App->physics->CreateCircle(65, 455, 9);
		ball->body->SetBullet(true);
		ball->listener = this;

		b2Vec2 force;
		force.Set(5, -5);
		ball->body->ApplyForceToCenter(force, true);
	}

	if (ball != nullptr && ball->pendingToDelete2 == true)
	{
		ball->pendingToDelete2 = false;
		delete ball;
		ball = nullptr;

		ball = App->physics->CreateCircle(310, 520, 9);
		ball->body->SetBullet(true);
		ball->listener = this;

		b2Vec2 force;
		force.Set(-5, -5);
		ball->body->ApplyForceToCenter(force, true);
	}

	if (ball != nullptr && ball->pendingToDelete3 == true)
	{
		ball->pendingToDelete3 = false;
		delete ball;
		ball = nullptr;

		ball = App->physics->CreateCircle(SCREEN_WIDTH - 23, SCREEN_HEIGHT / 2, 9);
		ball->body->SetBullet(true);
		ball->listener = this;
	}

	int x, y;
	ball->GetPosition(x, y);
	App->renderer->Blit(ballTex, x, y, &ballRec, 1.0f, ball->GetRotation());

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && strechingDown == false && strechingUp == false)
	{
		strechingDown = true;
	}

	if (strechingDown == true)
	{
		App->renderer->Blit(springTex, 370, 575, &springStrechingDown.GetCurrentFrame());
		if (&springStrechingDown.GetCurrentFrame() == &springStrechingDown.GetFrame(6))
		{
			springStrechingDown.Reset();
			strechingDown = false;
			strechingUp = true;
		}
	}
	else if (strechingUp == true && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->renderer->Blit(springTex, 370, 575, &springStrechingDown.GetFrame(6));
	}
	else if (strechingUp == true)
	{
		App->renderer->Blit(springTex, 370, 575, &springStrechingUp.GetCurrentFrame());
		if (&springStrechingUp.GetCurrentFrame() == &springStrechingUp.GetFrame(5))
		{
			strechingUp = false;
			if (alreadyKicked == false)
			{
				alreadyKicked = true;
				b2Vec2 force;
				force.Set(0, -70);
				ball->body->ApplyForceToCenter(force, true);
			}
		}
	}

	if (strechingDown == false && strechingUp == false)
	{
		App->renderer->Blit(springTex, 370, 575, &springStrechingDown.GetFrame(0));
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		App->audio->PlayFx(springFx);
	}
	
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ballTex);
	App->textures->Unload(springTex);

	return true;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == App->scene_intro->sensor)
	{
		bodyA->pendingToDelete = true;
	}

	if (bodyB == App->scene_intro->sensor2)
	{
		bodyA->pendingToDelete2 = true;
	}

	if (bodyB == App->scene_intro->sensor3)
	{
		bodyA->pendingToDelete3 = true;
	}

	if (bodyB == App->scene_intro->sensor4)
	{
		alreadyKicked = false;
	}
}