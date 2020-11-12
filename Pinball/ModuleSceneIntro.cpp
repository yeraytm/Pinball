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
	circle = box = rick = NULL;
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

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	pinball = App->textures->Load("pinball/pinball_stage.png");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	// Pivot 0, 0
	int pinball_stage[220] = {
		0, 0,
		440, 0,
		440, 800,
		247, 800,
		271, 779,
		265, 771,
		356, 693,
		360, 697,
		360, 725,
		363, 731,
		370, 733,
		377, 731,
		380, 725,
		380, 546,
		378, 538,
		371, 527,
		362, 517,
		350, 507,
		336, 496,
		335, 488,
		340, 481,
		372, 428,
		370, 422,
		366, 417,
		359, 416,
		352, 418,
		339, 438,
		327, 437,
		380, 288,
		386, 286,
		392, 289,
		397, 299,
		397, 673,
		401, 669,
		407, 667,
		413, 669,
		417, 673,
		417, 299,
		421, 289,
		421, 256,
		419, 226,
		413, 198,
		400, 172,
		380, 146,
		357, 129,
		329, 116,
		296, 109,
		264, 107,
		228, 109,
		196, 118,
		166, 135,
		140, 158,
		119, 188,
		107, 216,
		103, 247,
		104, 272,
		109, 293,
		116, 310,
		117, 317,
		110, 321,
		104, 317,
		94, 295,
		88, 273,
		86, 240,
		91, 206,
		101, 178,
		114, 156,
		120, 147,
		118, 139,
		113, 134,
		104, 134,
		97, 138,
		86, 150,
		75, 168,
		65, 195,
		57, 231,
		55, 258,
		58, 291,
		66, 317,
		81, 346,
		36, 432,
		35, 440,
		37, 448,
		45, 462,
		20, 483,
		18, 491,
		21, 497,
		27, 500,
		36, 500,
		60, 491,
		64, 494,
		64, 501,
		60, 507,
		48, 514,
		36, 526,
		27, 538,
		22, 553,
		22, 692,
		24, 700,
		24, 727,
		28, 731,
		34, 733,
		40, 731,
		44, 727,
		44, 697,
		49, 693,
		139, 771,
		133, 778,
		157, 800,
		0, 800
	};

	pinballBodies.add(App->physics->CreateChain(0, 0, pinball_stage, 220));

	p2List_item<PhysBody*>* pinballBody;
	for (pinballBody = pinballBodies.getFirst(); pinballBody; pinballBody = pinballBody->next)
	{
		pinballBody->data->body->SetType(b2_staticBody);
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(circle);
	App->textures->Unload(box);
	App->textures->Unload(rick);
	App->textures->Unload(pinball);
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
	App->renderer->Blit(pinball, 0, 0, NULL, 1.0f);

	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

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
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

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
