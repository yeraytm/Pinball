#include "ModuleEndingScreen.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"

ModuleEndingScreen::ModuleEndingScreen(Application* app, bool startEnabled) : Module(app, startEnabled)
{}

ModuleEndingScreen::~ModuleEndingScreen()
{}

bool ModuleEndingScreen::Start()
{
	bool ret = true;

	// Screen rect
	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	tex = App->textures->Load("pinball/ending_screen.png");

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

	return ret;
}

bool ModuleEndingScreen::CleanUp()
{
	bool ret = true;

	App->textures->Unload(tex);

	return ret;
}