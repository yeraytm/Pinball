#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFlipper.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	spriteSheet = NULL;
	ray_on = false;

	boardRect = { 0, 83, 405, 677 };
	boardPortalRect = { 416, 258, 340, 369 };

	blueBallRect = { 646, 5, 37, 37 }; // ball point blue: x = 645 y = 4 w = 37 h = 37
	orangeBallRect = { 683, 5, 37, 37 }; // ball point orange: x = 682 y = 4 w = 37 h = 37

	starBallRect = { 723, 5, 37, 37 }; // ball star idle: x = 722 y = 4 w = 37 h = 37
	starBallHitRect = { 760, 5, 37, 37 }; // ball star hit: x = 759 y = 4 w = 37 h = 37

	bigStarBallRect = { 477, 2, 77, 77 }; // big ball star idle: x = 477 y = 2 w = 77 h = 77
	bigStarBallHitRect = { 557, 2, 77, 77 }; // big ball star hit: x = 557 y = 2 w = 77 h = 77
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	spriteSheet = App->textures->Load("pinball/Spritesheet.png");

	pointsFx = App->audio->LoadFx("pinball/hitBall.wav");
	bumpFx = App->audio->LoadFx("pinball/hitBallStar.wav");
	flipperFx = App->audio->LoadFx("pinball/flipper.wav");

	//sensor = App->physics->CreateRectangleSensor(150, SCREEN_HEIGHT / 2, 30, 30);

	// Pivot 0, 0
	int boardPoints[124] = {
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

	boardParts.add(App->physics->CreateChain(0, 0, boardPoints, 124));

	// Pivot 0, 0
	int boardRamp1[68] = {
		110, 376,
		97, 354,
		93, 333,
		93, 310,
		101, 294,
		116, 281,
		142, 270,
		166, 266,
		189, 269,
		211, 277,
		228, 289,
		240, 307,
		250, 333,
		254, 360,
		256, 383,
		256, 400,
		255, 424,
		254, 424,
		255, 400,
		255, 383,
		253, 360,
		249, 333,
		239, 307,
		227, 289,
		209, 277,
		189, 270,
		166, 267,
		142, 271,
		116, 282,
		102, 294,
		94, 310,
		94, 333,
		98, 354,
		111, 376
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardRamp1, 68));

	// Pivot 0, 0
	int boardRamp2[60] = {
		82, 389,
		73, 360,
		71, 330,
		78, 301,
		97, 277,
		124, 260,
		154, 252,
		188, 252,
		220, 263,
		244, 281,
		256, 298,
		265, 315,
		270, 334,
		272, 360,
		276, 423,
		275, 423,
		271, 360,
		269, 334,
		264, 315,
		255, 298,
		243, 281,
		220, 264,
		189, 253,
		154, 253,
		125, 260,
		98, 277,
		79, 301,
		72, 330,
		74, 360,
		83, 389
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardRamp2, 60));

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
	int boardPlatform1[12] = {
	79, 536,
	116, 602,
	102, 612,
	74, 563,
	74, 547,
	76, 540
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardPlatform1, 12));

	// Pivot 0, 0
	int boardPlatform2[10] = {
		310, 608,
		336, 559,
		336, 544,
		331, 537,
		297, 599,

	};

	boardParts.add(App->physics->CreateChain(0, 0, boardPlatform2, 10));

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

	p2List_item<PhysBody*>* pinballBody;
	for (pinballBody = boardParts.getFirst(); pinballBody; pinballBody = pinballBody->next)
	{
		pinballBody->data->body->SetType(b2_staticBody);
	}

	pointBall = App->physics->CreateStaticCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 18);
	pointBall->listener = this;

	starBall = App->physics->CreateStaticCircle(SCREEN_WIDTH / 2, 400, 18);
	starBall->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	App->textures->Unload(spriteSheet);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	App->renderer->Blit(spriteSheet, 0, 0, &boardRect, 0.0f);

	/*p2List_item<PhysBody*>* c = pointBalls.getFirst();
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		SDL_Rect pointsBallRect = { 312, 2, 75, 75 };
		App->renderer->Blit(spriteSheet, x, y, &pointsBallRect, 1.0f, c->data->GetRotation());
		c = c->next;
	}*/

	int x1, y1;
	pointBall->GetPosition(x1, y1);
	App->renderer->Blit(spriteSheet, x1, y1, &blueBallRect, 1.0f, pointBall->GetRotation());

	int x2, y2;
	starBall->GetPosition(x2, y2);
	App->renderer->Blit(spriteSheet, x2, y2, &starBallRect, 1.0f, starBall->GetRotation());

	/*c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}*/

	// ray -----------------
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
	if (bodyA == pointBall) App->audio->PlayFx(pointsFx);

	if (bodyA == starBall)
	{
		b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
		force *= 7;
		bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);
		App->audio->PlayFx(bumpFx);
	}

	// TODO: Check flipper audio
	//if (bodyA == App->flipper->leftFlipper.pBody || bodyA == App->flipper->rightFlipper.pBody)
	//{
	//	App->audio->PlayFx(flipperFx);
	//}
}
