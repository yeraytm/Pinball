#include "Application.h"
#include "ModuleEndingScreen.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleFonts.h"
#include "ModuleSceneIntro.h"
#include <string>

ModuleEndingScreen::ModuleEndingScreen(Application* app, bool startEnabled) : Module(app, startEnabled) {}
ModuleEndingScreen::~ModuleEndingScreen() {}

bool ModuleEndingScreen::Start()
{
	bool ret = true;

	// Screen rect
	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	tex = App->textures->Load("pinball/graphics/ending_screen.png");
	font = App->fonts->Load("pinball/graphics/font.png", lookupTable, 8);

	if (tex == nullptr)
	{
		ret = false;
	}

	return ret;
}

update_status ModuleEndingScreen::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(this, (Module*)App->title_screen, 100);
	}

	// Blit 
	if (!App->renderer->Blit(tex, 0, 0, &screen, 0.0f))
	{
		ret = UPDATE_STOP;
	}

	App->fonts->BlitText(70, SCREEN_HEIGHT/ 2, font, "Score:................");
	App->fonts->BlitText(300, SCREEN_HEIGHT / 2, font, std::to_string(App->scene_intro->GetScore()).c_str());
	App->fonts->BlitText(70, SCREEN_HEIGHT / 2+40, font, "PreviousScore:........");
	App->fonts->BlitText(300, SCREEN_HEIGHT / 2+40, font, std::to_string(App->scene_intro->GetPrevScore()).c_str());
	App->fonts->BlitText(70, SCREEN_HEIGHT / 2+80, font, "HighestScore:.........");
	App->fonts->BlitText(300, SCREEN_HEIGHT / 2+80, font, std::to_string(App->scene_intro->GetHighScore()).c_str());

	return ret;
}

bool ModuleEndingScreen::CleanUp()
{
	bool ret = true;

	App->textures->Unload(tex);

	return ret;
}