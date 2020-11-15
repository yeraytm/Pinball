#include "Application.h"
#include "ModuleHUD.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>

ModuleHUD::ModuleHUD(Application* app, bool start_enabled) : Module(app, start_enabled) {}
ModuleHUD::~ModuleHUD() {}

bool ModuleHUD::Start()
{
	bool ret = true;

	font = App->fonts->Load("pinball/graphics/font.png", lookupTable, 8);

	soundTex = App->textures->Load("pinball/graphics/music.png");
	soundOffRect = { 0, 0, 21, 22 };
	soundOnRect = { 21, 0, 21, 22 };

	logoTex = App->textures->Load("pinball/graphics/Logo.png");
	logoRect = { 0, 0, 35, 18 };
	upcTex = App->textures->Load("pinball/graphics/UPC.png");
	upcRect = { 0, 0, 27, 27 };

	return ret;
}

update_status ModuleHUD::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (App->audio->volume == 0 || App->audio->MusicIsPaused()) {
		App->renderer->Blit(soundTex, 190, 75, &soundOffRect, 0.0f);

	}else{
					App->renderer->Blit(soundTex, 190, 75, &soundOnRect, 0.0f);

	}

	App->fonts->BlitText(90,12,font,"Score:");
	App->fonts->BlitText(100,30,font,std::to_string(App->scene_intro->GetScore()).c_str());	
	App->fonts->BlitText(250,14,font,"Lifes:");
	App->fonts->BlitText(265, 30, font, std::to_string(App->player->GetLifes()).c_str());

	App->renderer->Blit(logoTex, 18, 36, &logoRect, 0.0f);
	App->renderer->Blit(upcTex, 362, 28, &upcRect, 0.0f);

	return ret;
}
bool ModuleHUD::CleanUp() 
{
	bool ret = true;

	App->fonts->UnLoad(font);

	App->textures->Unload(soundTex);
	App->textures->Unload(logoTex);
	App->textures->Unload(upcTex);

	return ret;
}
