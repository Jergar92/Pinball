


#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	bouncer = App->physics->CreateRectangle(300, 750, 20, 20, 1);
	bouncerWheel = App->physics->CreateRectangle(305, 840, 10, 0, 0);
	App->physics->CreateLineJoint(bouncer->body, bouncerWheel->body, p2Point<float>(0, 0), p2Point<float>(0, 0), 20.0f, 1.0f);
	score = 0;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	static float powerPush = 0.0f;
	LOG("%f", powerPush);
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		powerPush += 100.0f;

		bouncer->body->ApplyForceToCenter(b2Vec2(0, powerPush), true);
	}
	else

		powerPush = 0.0f;

	return UPDATE_CONTINUE;
}



