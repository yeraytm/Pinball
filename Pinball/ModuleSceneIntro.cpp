#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFlipper.h"
#include "ModuleHUD.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	spritesheetTex = NULL;
	ray_on = false;

	boardRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	portalRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	blueBallRect = { 507, 6, 35, 35 }; // ball point blue: x = 507 y = 6 w = 35 h = 35
	orangeBallRect = { 544, 6, 35, 35 }; // ball point orange: x = 544 y = 6 w = 35 h = 35

	starBallRect = { 583, 5, 37, 37 }; // ball star idle: x = 583 y = 5 w = 37 h = 37
	starBallHitRect = { 620, 5, 37, 37 }; // ball star hit: x = 620 y = 5 w = 37 h = 37

	bigStarBallRect = { 371, 15, 52, 52 }; // big ball star idle: x = 371 y = 15 w = 52 h = 52
	bigStarBallHitRect = { 441, 14, 52, 52 }; // big ball star hit: x = 441 y = 14 w = 52 h = 52

	highScore = 0;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// Enabling necessary modules
	App->physics->Enable();
	App->fonts->Enable();
	App->hud->Enable();
	App->player->Enable();
	App->flipper->Enable();
	App->audio->Enable();

	// Loading textures
	boardTex = App->textures->Load("pinball/graphics/board.png");
	portalTex = App->textures->Load("pinball/graphics/portal.png");
	spritesheetTex = App->textures->Load("pinball/graphics/spritesheet.png");
	springTex = App->textures->Load("pinball/graphics/spring.png");

	// Loading audio
	pointsFx = App->audio->LoadFx("pinball/audio/hitBall.wav");
	bumpFx = App->audio->LoadFx("pinball/audio/hitBallStar.wav");
	rampFx = App->audio->LoadFx("pinball/audio/ramp.wav");
	oneUpFx = App->audio->LoadFx("pinball/audio/1-up.wav");
	springFx = App->audio->LoadFx("pinball/audio/firstBump.wav");

	// Setting spring animations rects
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

	// Initializing score
	score = 0;

	// Creating chains
	// Pivot 0, 0
	int boardBase[124] = {
		0, 0,
		406, 0,
		406, 677,
		365, 677,
		358, 283,
		338, 241,
		331, 245,
		320, 245,
		313, 242,
		319, 239,
		322, 231,
		322, 223,
		318, 217,
		312, 214,
		305, 213,
		299, 216,
		302, 205,
		308, 198,
		318, 193,
		330, 193,
		340, 196,
		347, 203,
		369, 227,
		369, 566,
		395, 566,
		395, 184,
		393, 167,
		389, 152,
		381, 136,
		370, 122,
		353, 107,
		354, 92,
		353, 86,
		349, 81,
		297, 66,
		290, 66,
		284, 72,
		264, 113,
		135, 113,
		114, 70,
		108, 66,
		102, 66,
		53, 80,
		47, 85,
		45, 91,
		46, 106,
		35, 115,
		24, 127,
		17, 139,
		9, 156,
		5, 176,
		5, 209,
		21, 277,
		42, 353,
		35, 354,
		31, 358,
		11, 428,
		11, 436,
		39, 511,
		37, 625,
		95, 677,
		0,677
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardBase, 124));

	// Pivot 0, 0
	int boardRamp[72] = {
		113, 374,
		100, 353,
		96, 331,
		97, 312,
		105, 295,
		119, 283,
		138, 274,
		158, 270,
		177, 270,
		195, 273,
		214, 282,
		228, 294,
		240, 312,
		246, 334,
		250, 358,
		252, 382,
		253, 404,
		253, 425,
		252, 425,
		252, 404,
		251, 382,
		249, 358,
		245, 334,
		239, 312,
		227, 294,
		213, 282,
		195, 274,
		177, 271,
		158, 271,
		137, 275,
		120, 283,
		106, 295,
		98, 312,
		97, 331,
		101, 353,
		114, 374
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardRamp, 72));

	// Pivot 0, 0
	int boardRamp2[64] = {
		80, 392,
		70, 362,
		68, 335,
		72, 308,
		85, 284,
		105, 265,
		128, 253,
		157, 248,
		186, 248,
		216, 256,
		240, 271,
		256, 289,
		268, 312,
		274, 335,
		276, 362,
		276, 425,
		277, 425,
		277, 362,
		275, 335,
		269, 312,
		257, 289,
		241, 271,
		218, 256,
		186, 247,
		157, 247,
		128, 252,
		104, 265,
		84, 284,
		71, 308,
		67, 335,
		69, 362,
		79, 392
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardRamp2, 64));

	// Pivot 0, 0
	int boardCircle[32] = {
		67, 176,
		56, 179,
		44, 178,
		34, 170,
		28, 160,
		28, 144,
		35, 133,
		48, 126,
		64, 127,
		76, 138,
		80, 150,
		73, 147,
		64, 149,
		58, 155,
		57, 165,
		61, 172
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardCircle, 32));

	// Pivot 0, 0
	int boardPlatform[12] = {
	79, 536,
	116, 602,
	102, 612,
	74, 563,
	74, 547,
	76, 540
	};

	// Has Restitution
	boardParts.add(App->physics->CreateChain(0, 0, boardPlatform, 12, 1));

	// Pivot 0, 0
	int boardPlatform2[10] = {
		310, 608,
		336, 559,
		336, 544,
		331, 537,
		297, 599,
	};

	// Has Restitution
	boardParts.add(App->physics->CreateChain(0, 0, boardPlatform2, 10, 1));

	// Pivot 0, 0
	int boardPlatform3[12] = {
		313, 462,
		360, 431,
		366, 435,
		358, 459,
		345, 469,
		317, 469
	};

	// Has Restitution
	boardParts.add(App->physics->CreateChain(0, 0, boardPlatform3, 12, 1));

	// Point Balls
	pointBall = App->physics->CreateCircleSensor(238, 160, 18);
	pointBall->listener = this;

	pointBall2 = App->physics->CreateCircleSensor(315, 335, 18);
	pointBall2->listener = this;

	pointBall3 = App->physics->CreateCircleSensor(73, 230, 18);
	pointBall3->listener = this;

	pointBall4 = App->physics->CreateCircleSensor(SCREEN_WIDTH / 2, 400, 18);
	pointBall4->listener = this;

	// Star / Bumper Balls
	starBall = App->physics->CreateStaticCircle(SCREEN_WIDTH / 2, 210, 18);
	starBall->listener = this;

	starBall2 = App->physics->CreateStaticCircle(310, 150, 18);
	starBall2->listener = this;

	starBall3 = App->physics->CreateStaticCircle(180, 490, 18);
	starBall3->listener = this;

	starBall4 = App->physics->CreateStaticCircle(65, 500, 18);
	starBall4->listener = this;

	starBall5 = App->physics->CreateStaticCircle(85, 100, 18);
	starBall5->listener = this;

	bigStarBall = App->physics->CreateStaticCircle(145, 328, 26);
	bigStarBall->listener = this;

	// Sensors
	sensor = App->physics->CreateRectangleSensor(73, 163, 20, 20);
	sensor->listener = this;

	sensor2 = App->physics->CreateRectangleSensor(307, 227, 20, 20);
	sensor2->listener = this;

	sensor3 = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 10, SCREEN_WIDTH, 20);
	sensor3->listener = this;

	sensor4 = App->physics->CreateRectangleSensor(382, 556, 1, 1);
	sensor4->listener = this;

	sensorRamp = App->physics->CreateRectangleSensor(170, 258, 8, 8);
	sensorRamp->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	// Unloading textures
	App->textures->Unload(boardTex);
	App->textures->Unload(portalTex);
	App->textures->Unload(spritesheetTex);
	App->textures->Unload(springTex);

	// Disabling modules
	App->physics->Disable();
	App->audio->Disable();
	App->fonts->Disable();
	App->hud->Disable();
	App->player->Disable();
	App->flipper->Disable();

	// Set all colliders and sensors to nullptr after being destroyed by the physics cleanup
	boardParts.clear();
	pointBall = nullptr;
	pointBall2 = nullptr;
	starBall = nullptr;
	starBall2 = nullptr;
	starBall3 = nullptr;
	bigStarBall = nullptr;

	sensor = nullptr;
	sensor2 = nullptr;
	sensor3 = nullptr;
	sensor4 = nullptr;
	sensorRamp = nullptr;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// Handling high score
	if (score > highScore) {
		highScore = score;
	}

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	App->renderer->Blit(boardTex, 0, 0, &boardRect, 0.0f);
	App->renderer->Blit(portalTex, 0, 0, &portalRect, 0.0f);

	// Handling spring
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && strechingDown == false && strechingUp == false)
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
	else if (strechingUp == true && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
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
				force.Set(0, -80);
				App->player->ball->body->ApplyForceToCenter(force, true);
			}
		}
	}

	if (strechingDown == false && strechingUp == false)
	{
		App->renderer->Blit(springTex, 370, 575, &springStrechingDown.GetFrame(0));
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		App->audio->PlayFx(springFx);
	}

	// Blit Point Balls
	if(!pointBall->hit) App->renderer->Blit(spritesheetTex, pointBall->GetPosX(), pointBall->GetPosY(), &blueBallRect, 1.0f, pointBall->GetRotation());
	else App->renderer->Blit(spritesheetTex, pointBall->GetPosX(), pointBall->GetPosY(), &orangeBallRect, 1.0f, pointBall->GetRotation());

	if (!pointBall2->hit) App->renderer->Blit(spritesheetTex, pointBall2->GetPosX(), pointBall2->GetPosY(), &blueBallRect, 1.0f, pointBall2->GetRotation());
	else App->renderer->Blit(spritesheetTex, pointBall2->GetPosX(), pointBall2->GetPosY(), &orangeBallRect, 1.0f, pointBall2->GetRotation());

	if (!pointBall3->hit) App->renderer->Blit(spritesheetTex, pointBall3->GetPosX(), pointBall3->GetPosY(), &blueBallRect, 1.0f, pointBall3->GetRotation());
	else App->renderer->Blit(spritesheetTex, pointBall3->GetPosX(), pointBall3->GetPosY(), &orangeBallRect, 1.0f, pointBall3->GetRotation());

	if (!pointBall4->hit) App->renderer->Blit(spritesheetTex, pointBall4->GetPosX(), pointBall4->GetPosY(), &blueBallRect, 1.0f, pointBall4->GetRotation());
	else App->renderer->Blit(spritesheetTex, pointBall4->GetPosX(), pointBall4->GetPosY(), &orangeBallRect, 1.0f, pointBall4->GetRotation());

	// Blit Star / Bumper Balls
	if(!starBall->hit) App->renderer->Blit(spritesheetTex, starBall->GetPosX(), starBall->GetPosY(), &starBallRect, 1.0f, starBall->GetRotation());
	else App->renderer->Blit(spritesheetTex, starBall->GetPosX(), starBall->GetPosY(), &starBallHitRect, 1.0f, starBall->GetRotation());

	if (!starBall2->hit) App->renderer->Blit(spritesheetTex, starBall2->GetPosX(), starBall2->GetPosY(), &starBallRect, 1.0f, starBall2->GetRotation());
	else App->renderer->Blit(spritesheetTex, starBall2->GetPosX(), starBall2->GetPosY(), &starBallHitRect, 1.0f, starBall2->GetRotation());
	
	if (!starBall3->hit) App->renderer->Blit(spritesheetTex, starBall3->GetPosX(), starBall3->GetPosY(), &starBallRect, 1.0f, starBall3->GetRotation());
	else App->renderer->Blit(spritesheetTex, starBall3->GetPosX(), starBall3->GetPosY(), &starBallHitRect, 1.0f, starBall3->GetRotation());

	if (!starBall4->hit) App->renderer->Blit(spritesheetTex, starBall4->GetPosX(), starBall4->GetPosY(), &starBallRect, 1.0f, starBall4->GetRotation());
	else App->renderer->Blit(spritesheetTex, starBall4->GetPosX(), starBall4->GetPosY(), &starBallHitRect, 1.0f, starBall4->GetRotation());

	if (!starBall5->hit) App->renderer->Blit(spritesheetTex, starBall5->GetPosX(), starBall5->GetPosY(), &starBallRect, 1.0f, starBall5->GetRotation());
	else App->renderer->Blit(spritesheetTex, starBall5->GetPosX(), starBall5->GetPosY(), &starBallHitRect, 1.0f, starBall5->GetRotation());

	if (!bigStarBall->hit) App->renderer->Blit(spritesheetTex, bigStarBall->GetPosX(), bigStarBall->GetPosY(), &bigStarBallRect, 1.0f, bigStarBall->GetRotation());
	else App->renderer->Blit(spritesheetTex, bigStarBall->GetPosX(), bigStarBall->GetPosY(), &bigStarBallHitRect, 1.0f, bigStarBall->GetRotation());

	// Ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Handling balls collisions
	if (bodyA == pointBall || bodyA == pointBall2 || bodyA == pointBall3 || bodyA == pointBall4)
	{
		bodyA->hit = !bodyA->hit;
		score += 10;

		if (pointBall->hit && pointBall2->hit && pointBall3->hit && pointBall4->hit)
		{
			++App->player->lifes;
			App->audio->PlayFx(oneUpFx);
		}

		App->audio->PlayFx(pointsFx);
	}

	if (bodyA == starBall || bodyA == starBall2 || bodyA == starBall3 || bodyA == starBall4 || bodyA == starBall5 || bodyA == bigStarBall)
	{
		bodyA->hit = !bodyA->hit;
		score += 10;

		b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
		force *= 2;
		bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);

		App->audio->PlayFx(bumpFx);
	}

	// Handling sensors collisions
	if (bodyA == sensor)
	{
		bodyB->pendingToDelete = true;
	}

	if (bodyA == sensor2)
	{
		bodyB->pendingToDelete2 = true;
	}

	if (bodyA == sensor3)
	{
		pointBall->hit = pointBall2->hit = pointBall3->hit = pointBall4->hit = starBall->hit = starBall2->hit = starBall3->hit = starBall4->hit = starBall5->hit = bigStarBall->hit = false;

		bodyB->pendingToDelete3 = true;
		if (App->player->lifes > 1) 
		{
			--App->player->lifes;
		}
		else 
		{
			App->fade_to_black->FadeToBlack((Module*)App->scene_intro, (Module*)App->ending_screen, 100);
		}
	}

	if (bodyA == sensor4)
	{
		alreadyKicked = false;
	}

	if (bodyA == sensorRamp)
	{
		score += 30;

		App->audio->PlayFx(rampFx);
	}
}