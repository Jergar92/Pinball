


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
	ray_on = false;
	sensed = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	score = 0;

	ball = App->textures->Load("pinball/ball.png");
	left_flip = App->textures->Load("pinball/Sprites/FlipLeft.png");
	right_flip = App->textures->Load("pinball/Sprites/FlipRight.png");
	
  	//ADD BALL
	circles.add(App->physics->CreateCircle(305, 780, 6, 1, 0, ball));
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, 870, SCREEN_WIDTH, 50);

	//ADD FLIPPERS

	//MID LEFT
	circles.add(App->physics->CreateCircle(85, 530, 6, 0));
	boxes.add(App->physics->CreateRectangle(85 + 25, 530, 50, 12, 1, left_flip, FLIP));
	boxes.getLast()->data->listener = this;
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(85 + 25, 550, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	mid_left_flip = circles.getLast()->data;

	//MID RIGHT
	circles.add(App->physics->CreateCircle(209, 529, 6, 0));
	boxes.add(App->physics->CreateRectangle(209 - 25, 529, 50, 12, 1, right_flip, FLIP));
	boxes.getLast()->data->listener = this;
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(209 - 25, 529, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	mid_right_flip = circles.getLast()->data;
	p2List_item<PhysBody*>* it = boxes.getFirst();
	


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
	for (p2List_item<PhysBody*>* it = circles.getFirst(); it != nullptr; it = it->next)
	{
		//Blit Circles
		b2Vec2 pos = it->data->body->GetPosition();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width), METERS_TO_PIXELS(pos.y - it->data->height));
	}
	//Blit 6 Flippers
	p2List_item<PhysBody*>* it = boxes.getFirst();

	b2Vec2 pos = it->data->body->GetPosition();
	float angle = RADTODEG*it->data->body->GetAngle();
	App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 7), METERS_TO_PIXELS(pos.y - it->data->height - 9), NULL, NULL, angle);

	it = it->next;
	pos = it->data->body->GetPosition();
	angle = RADTODEG*it->data->body->GetAngle();
	App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 10), NULL, NULL, angle);


	for (p2List_item<PhysBody*>* it = circles.getFirst(); it != nullptr; it = it->next)
	{
		//Blit Circles
		b2Vec2 pos = it->data->body->GetPosition();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width), METERS_TO_PIXELS(pos.y - it->data->height));
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		circles.getFirst()->data->body->ApplyForceToCenter(b2Vec2(0, -60), true);
	}

	mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
	}

	return UPDATE_CONTINUE;
}




void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if (bodyA->myBodyType == FLIP || bodyB->myBodyType == FLIP)
		LOG("Flip");
	
}