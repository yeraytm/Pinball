#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	spriteSheet = NULL;
	ray_on = false;
	sensed = false;
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

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 30, 30);

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
		0, 677
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardPoints, 124));

	//// Pivot 0, 0
	/*int boardRampPoints1[48] = {
		110, 375,
		83, 390,
		73, 363,
		70, 335,
		76, 305,
		92, 281,
		113, 264,
		145, 252,
		185, 251,
		222, 264,
		253, 290,
		270, 332,
		275, 425,
		257, 425,
		252, 352,
		246, 321,
		236, 299,
		216, 280,
		187, 269,
		149, 269,
		116, 282,
		97, 303,
		93, 329,
		96, 351
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardRampPoints1, 48));*/

	// Pivot 0, 0
	/*int boardRampPoints2[60] = {
		111, 376,
		97, 351,
		94, 326,
		97, 303,
		114, 284,
		133, 273,
		158, 268,
		181, 268,
		205, 275,
		223, 286,
		236, 301,
		245, 321,
		251, 345,
		254, 371,
		256, 426,
		256, 426,
		254, 371,
		251, 345,
		245, 321,
		236, 301,
		223, 286,
		205, 275,
		181, 268,
		158, 268,
		133, 273,
		114, 284,
		97, 303,
		94, 326,
		97, 351,
		111, 376
	};

	boardParts.add(App->physics->CreateChain(0, 0, boardRampPoints2, 65));*/

	p2List_item<PhysBody*>* pinballBody;
	for (pinballBody = boardParts.getFirst(); pinballBody; pinballBody = pinballBody->next)
	{
		pinballBody->data->body->SetType(b2_staticBody);
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	App->textures->Unload(spriteSheet);
	//App->textures->Unload(circle);

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

	SDL_Rect boardRect = { 0, 83, 405, 677 };
	App->renderer->Blit(spriteSheet, 0, 0, &boardRect, 0.0f);

	/*p2List_item<PhysBody*>* c = pointBalls.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
			App->renderer->Blit(spriteSheet, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}*/

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
	int x, y;

	App->audio->PlayFx(bonus_fx);
}
