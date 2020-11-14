#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFlipper.h"
#include "ModulePlayer.h"

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
	int boardRamp1[72] = {
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

	boardParts.add(App->physics->CreateChain(0, 0, boardRamp1, 72));

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

	sensor = App->physics->CreateRectangleSensor(73, 163, 20, 20);
	sensor->listener = this;

	sensor2 = App->physics->CreateRectangleSensor(307, 227, 20, 20);
	sensor2->listener = this;

	sensor3 = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, SCREEN_HEIGHT + 10, SCREEN_WIDTH, 20);
	sensor3->listener = this;

	sensor4 = App->physics->CreateRectangleSensor(382, 552, 20, 20);
	sensor4->listener = this;

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
	App->renderer->Blit(spriteSheet, 20, 173, &boardPortalRect, 0.0f);

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
