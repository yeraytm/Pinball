#include "ModuleHUD.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include <string>

ModuleHUD::ModuleHUD(Application* app, bool start_enabled) : Module(app, start_enabled) {}
ModuleHUD::~ModuleHUD() {}

bool ModuleHUD::Start()
{
	bool ret = true;

	font = App->fonts->Load("pinball/graphics/font.png", lookupTable, 8);

	return ret;
}

update_status ModuleHUD::Update()
{
	update_status ret = UPDATE_CONTINUE;

	App->fonts->BlitText(90,12,font,"Score:");
	App->fonts->BlitText(100,30,font,std::to_string(App->scene_intro->GetScore()).c_str());	
	App->fonts->BlitText(250,14,font,"Lifes:");
	App->fonts->BlitText(265, 30, font, std::to_string(App->player->GetLifes()).c_str());

	return ret;
}
bool ModuleHUD::CleanUp() 
{
	bool ret = true;

	App->fonts->UnLoad(font);

	return ret;
}
