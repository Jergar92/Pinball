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
	background = NULL;
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

	
	background = App->textures->Load("pinball/pinball_back.png");
	ball = App->textures->Load("pinball/ball.png");
	left_flip= App->textures->Load("pinball/Sprites/FlipLeft.png");
	right_flip = App->textures->Load("pinball/Sprites/FlipRight.png");

	//ADD BALL
	circles.add(App->physics->CreateCircle(305, 750, 6, 1, 0, ball));

	//ADD GRAVES
	circles.add(App->physics->CreateCircle(83, 355, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "4", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(78, 408, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "4", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(146, 385, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "3", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(202, 408, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "2", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(235, 390, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "2", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	//////
	circles.add(App->physics->CreateCircle(27, 610, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "4", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(98, 640, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "3", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(240, 607, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "2", circles.getLast()->data));
	circles.getLast()->data->listener = this;
	
	circles.add(App->physics->CreateCircle(45, 684, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "4", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(207, 653, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "2", circles.getLast()->data));
	circles.getLast()->data->listener = this;


	circles.add(App->physics->CreateCircle(258, 680, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, "1", circles.getLast()->data));
	circles.getLast()->data->listener = this;

	//
	bonus.add(App->physics->CreateCircleSensor(95, 380, 10, 0, BONUS));
	listBonus.add(new Bonus(this,"3", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(185, 380, 10, 0, BONUS));
	listBonus.add(new Bonus(this,"3", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(140, 425, 10, 0, BONUS));
	listBonus.add(new Bonus(this,"5", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(140, 485, 10, 0, BONUS));
	listBonus.add(new Bonus(this,"2", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(90, 585, 10, 0, BONUS));
	listBonus.add(new Bonus(this, "3", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(190, 585,10, 0, BONUS));
	listBonus.add(new Bonus(this, "3", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(115, 610, 10, 0, BONUS));
	listBonus.add(new Bonus(this, "2", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(170, 610, 10, 0, BONUS));
	listBonus.add(new Bonus(this, "2", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(140, 655	, 10, 0, BONUS));
	listBonus.add(new Bonus(this, "4", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(140, 685, 10, 0, BONUS));
	listBonus.add(new Bonus(this, "3", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	bonus.add(App->physics->CreateCircleSensor(140, 725,10, 0, BONUS));
	listBonus.add(new Bonus(this, "2", bonus.getLast()->data));
	bonus.getLast()->data->listener = this;

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, 870, SCREEN_WIDTH, 50);

	// Pivot 0, 0
	int pinball_exterior[184] = {
		162, 33,
		191, 36,
		233, 47,
		276, 70,
		293, 91,
		301, 99,
		312, 119,
		315, 145,
		310, 170,
		297, 195,
		277, 213,
		249, 226,
		220, 233,
		235, 254,
		246, 267,
		231, 283,
		228, 296,
		236, 300,
		255, 282,
		273, 277,
		291, 276,
		303, 282,
		312, 293,
		315, 306,
		315, 848,
		296, 850,
		296, 306,
		287, 301,
		265, 315,
		265, 464,
		276, 474,
		282, 483,
		283, 521,
		277, 543,
		265, 557,
		242, 583,
		257, 597,
		269, 615,
		278, 636,
		284, 660,
		282, 715,
		277, 725,
		258, 735,
		258, 748,
		276, 764,
		276, 797,
		272, 812,
		258, 824,
		230, 838,
		230, 847,
		230, 884,
		58, 883,
		55, 839,
		38, 828,
		20, 813,
		12, 795,
		11, 771,
		23, 753,
		36, 748,
		36, 728,
		22, 714,
		22, 655,
		8, 643,
		4, 629,
		20, 612,
		25, 604,
		35, 592,
		52, 566,
		25, 535,
		12, 504,
		11, 448,
		4, 434,
		1, 355,
		11, 348,
		59, 313,
		64, 302,
		44, 281,
		26, 252,
		24, 218,
		24, 195,
		35, 177,
		48, 160,
		47, 148,
		10, 143,
		13, 112,
		24, 87,
		42, 72,
		64, 57,
		82, 48,
		102, 42,
		130, 35,
		159, 33
	};

	int flip_1[18] = { 63, 193,
		75, 220,
		97, 232,
		101, 239,
		91, 248,
		72, 239,
		54, 221,
		51, 199,
		57, 191 };

	int flip_2[28] = { 44, 446,
		47, 449,
		49, 495,
		66, 513,
		81, 519,
		84, 524,
		81, 530,
		74, 536,
		62, 530,
		46, 516,
		35, 504,
		33, 482,
		33, 448,
		38, 445 };

	int flip_3[20] = { 216, 538,
		245, 521,
		257, 491,
		254, 480,
		242, 483,
		242, 492,
		237, 501,
		225, 515,
		211, 520,
		215, 526 };

	int flip_4[24] = { 49, 769,
		56, 786,
		71, 798,
		83, 804,
		90, 811,
		84, 821,
		69, 820,
		50, 809,
		40, 797,
		33, 780,
		33, 768,
		44, 763 };

	int flip_5[24] = { 229, 816,
		248, 798,
		256, 774,
		252, 764,
		243, 765,
		239, 768,
		238, 774,
		230, 789,
		217, 799,
		203, 805,
		203, 818,
		217, 820 };


	chains.add(App->physics->CreateChain(0, 0, pinball_exterior, 184, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_1, 18, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_2, 28, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_3, 20, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_4, 24, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_5, 24, 0));

	

	//ADD FLIPPERS

		//MID LEFT
	circles.add(App->physics->CreateCircle(85, 530, 6, 0));
	boxes.add(App->physics->CreateRectangle(85+25, 530, 50, 12, 1, left_flip, FLIP));
	boxes.getLast()->data->listener = this;
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(85+25, 550, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	mid_left_flip = circles.getLast()->data;

		//MID RIGHT
	circles.add(App->physics->CreateCircle(209, 529, 6, 0));
	boxes.add(App->physics->CreateRectangle(209 - 25, 529, 50, 12, 1, right_flip, FLIP));
	boxes.getLast()->data->listener = this;
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(209 -25, 529, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	mid_right_flip = circles.getLast()->data;
	
	//UP LEFT
	circles.add(App->physics->CreateCircle(100, 250, 6, 0));
	boxes.add(App->physics->CreateRectangle(100 + 25, 250, 50, 12, 1, left_flip, FLIP));
	boxes.getLast()->data->listener = this;
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(100 + 25, 270, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	up_left_flip = circles.getLast()->data;

	//UP RIGHT
	circles.add(App->physics->CreateCircle(220, 250, 6, 0));
	boxes.add(App->physics->CreateRectangle(220 - 25, 250, 50, 12, 1, right_flip, FLIP));
	boxes.getLast()->data->listener = this;
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(220 - 25, 270, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	up_right_flip = circles.getLast()->data;
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0);


	for (p2List_item<PhysBody*>* it = circles.getFirst(); it != nullptr; it = it->next)
	{

		if (it->data->body == nullptr)
			continue;

		//Blit Circles
		b2Vec2 pos = it->data->body->GetPosition();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width), METERS_TO_PIXELS(pos.y - it->data->height));
	}

	
	for (p2List_item<HeadStone*>* it = headstone.getFirst(); it != nullptr; it = it->next){
		
		if (it->data->lapidBody->body == nullptr)
			continue;
		
		b2Vec2 pos = it->data->lapidBody->body->GetPosition();
		
		if (it->data->lapidBody->life > 6) {
			App->renderer->Blit(it->data->texture[0], METERS_TO_PIXELS(pos.x - it->data->lapidBody->width), METERS_TO_PIXELS(pos.y - it->data->lapidBody->height - 5));
		}
		else if (it->data->lapidBody->life > 0)
		{
			App->renderer->Blit(it->data->texture[1], METERS_TO_PIXELS(pos.x - it->data->lapidBody->width), METERS_TO_PIXELS(pos.y - it->data->lapidBody->height - 5));
		}
		else if (it->data->lapidBody->life == 0)
		{
			App->physics->DestroyBody(it->data->lapidBody);
		}
	}
	for (p2List_item<Bonus*>* it = listBonus.getFirst(); it != nullptr; it = it->next) {

		if (it->data->bonusBody->body == nullptr)
			continue;

		b2Vec2 pos = it->data->bonusBody->body->GetPosition();

		if (it->data->bonusBody->active == true) {
			App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->bonusBody->width), METERS_TO_PIXELS(pos.y - it->data->bonusBody->height - 5));
		}
		
	}

	for (p2List_item<Bonus*>* it = listBonus.getFirst(); it != nullptr; it = it->next) {

		if (it->data->bonusBody->body == nullptr)
			continue;

		b2Vec2 pos = it->data->bonusBody->body->GetPosition();
		if (it->data->bonusBody->active == true ) {
			it->data->currentTime = SDL_GetTicks();

			if (it->data->lastTime + 10000 < it->data->currentTime)
			{
			
			it->data->lastTime = it->data->currentTime;
			it->data->bonusBody->active = false;
			}
		}

	}
	
	//Blit 6 Flippers
	p2List_item<PhysBody*>* it = boxes.getFirst();

		b2Vec2 pos = it->data->body->GetPosition();
		float angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width-7), METERS_TO_PIXELS(pos.y - it->data->height-9), NULL, NULL, angle);
	
		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width-5), METERS_TO_PIXELS(pos.y - it->data->height-10), NULL, NULL, angle);

		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 10), NULL, NULL, angle);

		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 10), NULL, NULL, angle);



	//TO DELETE
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		circles.getFirst()->data->body->ApplyForceToCenter(b2Vec2(0, -60), true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		//App->renderer->camera.y--;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->renderer->camera.y++;
	}

	//APPLY FORCES TO FLIPPERS

	mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	up_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	up_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, -90), true);
		up_left_flip->body->ApplyForceToCenter(b2Vec2(0, -90), true);

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, -90), true);
		up_right_flip->body->ApplyForceToCenter(b2Vec2(0, -90), true);

	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
	if (bodyA->myBodyType == GRAVES && bodyA->life!=0)
	{
		bodyA->life--;
	}

	if (bodyB->myBodyType == GRAVES && bodyB->life != 0)
	{
		bodyB->life--;
	}
	if (bodyA->myBodyType == BONUS)
	{
		bodyA->active= true;
	}

	if (bodyB->myBodyType == BONUS)
	{
		bodyB->active = true;
	}

	

}

HeadStone::HeadStone(ModuleSceneIntro* scene ,uint life, const char* lapidnumber, PhysBody* lapidBody): lapidBody(lapidBody)
{
	
	uint i = 0;

	lapidBody->life = life;

	p2SString tmp1("pinball/Sprites/Grave_%s_Ok.png", lapidnumber);
	p2SString tmp2("pinball/Sprites/Grave_%s_Des.png", lapidnumber);

	texture[0]= scene->App->textures->Load(tmp1.GetString());
	texture[1]= scene->App->textures->Load(tmp2.GetString());
}

Bonus::Bonus(ModuleSceneIntro * scene, const char * bonusnumber, PhysBody * lapidBody) : bonusBody(lapidBody)
{
	p2SString tmp1("pinball/Sprites/x%s.png", bonusnumber);
	texture = scene->App->textures->Load(tmp1.GetString());
	currentTime = SDL_GetTicks();
	lastTime = currentTime;

}