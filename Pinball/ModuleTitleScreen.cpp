#include "ModuleTitleScreen.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"

ModuleTitleScreen::ModuleTitleScreen(Application* app, bool startEnabled) : Module(app, startEnabled)
{}

ModuleTitleScreen::~ModuleTitleScreen()
{}

bool ModuleTitleScreen::Start()
{
	bool ret = true;

	// Screen rect
	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	tex = App->textures->Load("pinball/graphics/title_screen.png");

	if (tex == nullptr)
	{
		ret = false;
	}

	return ret;
}

update_status ModuleTitleScreen::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(this, (Module*)App->scene_intro, 100);
	}

	// Blit 
	if (!App->renderer->Blit(tex, 0, 0, &screen, 0.0f))
	{
		ret = UPDATE_STOP;
	}

	return ret;
}

bool ModuleTitleScreen::CleanUp()
{
	bool ret = true;

	App->textures->Unload(tex);

	return ret;
}