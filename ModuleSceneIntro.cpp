#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFadeToBlack.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	background = NULL;
	ray_on = false;	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	actualBonus = 1;

	myScore = 0;
	myLife = 0;
	game_over = false;


	//LoadMusic

	App->audio->PlayMusic("pinball/sounds/game_music.ogg", 0.0f);
	int i = 0;
	gravesFx[i++] = App->audio->LoadFx("pinball/sounds/grave1.wav");
	gravesFx[i++] = App->audio->LoadFx("pinball/sounds/grave2.wav");
	gravesFx[i++] = App->audio->LoadFx("pinball/sounds/grave3.wav");
	gravesFx[i++] = App->audio->LoadFx("pinball/sounds/grave4.wav");
	bonusFx= App->audio->LoadFx("pinball/sounds/ding_snd.wav");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Load Images
	background = App->textures->Load("pinball/pinball_back.png");
	ball_texture = App->textures->Load("pinball/ball.png");
	left_flip= App->textures->Load("pinball/Sprites/FlipLeft.png");
	right_flip = App->textures->Load("pinball/Sprites/FlipRight.png");
	brain_text = App->textures->Load("pinball/Sprites/brain.png");
	for (int i = 0; i < 4; i++)
	{
		p2SString tmp1("pinball/Sprites/Grave_%i_Ok.png", i+1);
		p2SString tmp2("pinball/Sprites/Grave_%i_Des.png", i+1);

		grave_ok[i] = App->textures->Load(tmp1.GetString());
		grave_des[i] = App->textures->Load(tmp2.GetString());
	}

	for (int i = 0; i < 4; i++)
	{

		p2SString tmp("pinball/Sprites/x%i.png", i + 2);
		bonus_tex[i] = App->textures->Load(tmp.GetString());
	}

	//ADD BALL
	ball = App->physics->CreateCircle(305, 750, 6, 1, 0, ball_texture);
	ball->body->IsBullet();

	//ADD BRAIN
	circles.add(App->physics->CreateCircle(0, 390, 35, 0, 1, brain_text, HIT_OBJECT));
	brain = new Brain(this, 100, circles.getLast()->data);
	circles.getLast()->data->listener = this;

	//ADD GRAVES
	circles.add(App->physics->CreateCircle(83, 355, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[3], 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(78, 408, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[3], 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(146, 385, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[2], 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(202, 408, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[1], 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(235, 390, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[1], 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	//////
	circles.add(App->physics->CreateCircle(27, 610, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[3], 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(98, 640, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[2], 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(240, 607, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[1], 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(40, 684, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[3], 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;


	circles.add(App->physics->CreateCircle(207, 653, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[1], 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(250, 680, 15, 0, 1, NULL, GRAVES));
	headstone.add(new HeadStone(this, 10, 100, gravesFx[0], 1, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	//
	circles.add(App->physics->CreateCircleSensor(95, 380, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(185, 380, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(140, 425, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 5, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(140, 485, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(90, 585, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(190, 585, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(115, 610, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(170, 610, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(140, 655, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(140, 685, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(140, 725, 10, 0, BONUS));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

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
		202, 805,
		204, 814,
		214, 821 };


	chains.add(App->physics->CreateChain(0, 0, pinball_exterior, 184, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_1, 18, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_2, 28, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_3, 20, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_4, 24, 0));
	chains.add(App->physics->CreateChain(0, 0, flip_5, 24, 0));

	

	//ADD FLIPPERS

		//BOTTOM LEFT
	circles.add(App->physics->CreateCircle(88, 816, 6, 0));
	boxes.add(App->physics->CreateRectangle(88 + 25, 816, 50, 12, 1, left_flip, FLIP));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(88 + 25, 836, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	low_left_flip = circles.getLast()->data;

     	//BOTTOM RIGHT
	circles.add(App->physics->CreateCircle(205, 814, 6, 0));
	boxes.add(App->physics->CreateRectangle(205 - 25, 814, 50, 12, 1, right_flip, FLIP));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(205 - 25, 834, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	low_right_flip = circles.getLast()->data;

		//MID LEFT
	circles.add(App->physics->CreateCircle(85, 530, 6, 0));
	boxes.add(App->physics->CreateRectangle(85+25, 530, 50, 12, 1, left_flip, FLIP));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(85+25, 550, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	mid_left_flip = circles.getLast()->data;

		//MID RIGHT
	circles.add(App->physics->CreateCircle(209, 529, 6, 0));
	boxes.add(App->physics->CreateRectangle(209 - 25, 529, 50, 12, 1, right_flip, FLIP));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(209 -25, 529, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	mid_right_flip = circles.getLast()->data;
	
	//UP LEFT
	circles.add(App->physics->CreateCircle(100, 250, 6, 0));
	boxes.add(App->physics->CreateRectangle(100 + 25, 250, 50, 12, 1, left_flip, FLIP));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(100 + 25, 270, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	up_left_flip = circles.getLast()->data;

	//UP RIGHT
	circles.add(App->physics->CreateCircle(220, 250, 6, 0));
	boxes.add(App->physics->CreateRectangle(220 - 25, 250, 50, 12, 1, right_flip, FLIP));
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
	App->textures->Unload(background);
	App->textures->Unload(ball_texture);
	App->textures->Unload(left_flip);
	App->textures->Unload(right_flip);

	
	for (int i = 0; i < 4; i++)
	{
		App->textures->Unload(grave_ok[i]);
		App->textures->Unload(grave_des[i]);
		App->textures->Unload(bonus_tex[i]);
	}





	for (p2List_item<PhysBody*>* it = circles.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data);
	}
	circles.clear();

	for (p2List_item<PhysBody*>* it = boxes.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data);
	}
	boxes.clear();

	for (p2List_item<PhysBody*>* it = chains.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data);
	}
	chains.clear();

	for (p2List_item<HeadStone*>* it = headstone.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data->stoneBody);
	}
	headstone.clear();

	for (p2List_item<Bonus*>* it = listBonus.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data->bonusBody);
	}
	listBonus.clear();


	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0);
	LOG("myScore=%i, myBonus=%i", myScore,actualBonus);



	for (p2List_item<PhysBody*>* it = circles.getFirst(); it != nullptr; it = it->next)
	{

		if (it->data->body == nullptr)
			continue;

		//Blit Circles
		b2Vec2 pos = it->data->body->GetPosition();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width), METERS_TO_PIXELS(pos.y - it->data->height));
	}


	//Blit Bonus
	for (p2List_item<Bonus*>* it = listBonus.getFirst(); it != nullptr; it = it->next) {

		if (it->data->bonusBody->body == nullptr)
			continue;

		b2Vec2 pos = it->data->bonusBody->body->GetPosition();

		if (it->data->active == true) {
			App->renderer->Blit(bonus_tex[it->data->bonusValue-2], METERS_TO_PIXELS(pos.x - it->data->bonusBody->width), METERS_TO_PIXELS(pos.y - it->data->bonusBody->height));
		}

	}


	//Blit Ball
	{		b2Vec2 pos =ball->body->GetPosition();
	
	App->renderer->Blit(ball->texture, METERS_TO_PIXELS(pos.x - ball->width), METERS_TO_PIXELS(pos.y - ball->height)); }



	//Blit Graves
	for (p2List_item<HeadStone*>* it = headstone.getFirst(); it != nullptr; it = it->next){
		
		if (it->data->stoneBody->body == nullptr)
			continue;
		
		b2Vec2 pos = it->data->stoneBody->body->GetPosition();
		
		if (it->data->life > 6) {
			App->renderer->Blit(grave_ok[it->data->number-1], METERS_TO_PIXELS(pos.x - it->data->stoneBody->width), METERS_TO_PIXELS(pos.y - it->data->stoneBody->height - 5));
		}
		else if (it->data->life > 0)
		{
			App->renderer->Blit(grave_des[it->data->number-1], METERS_TO_PIXELS(pos.x - it->data->stoneBody->width), METERS_TO_PIXELS(pos.y - it->data->stoneBody->height - 5));
		}
		else if (it->data->life == 0)
		{
			App->physics->DestroyBody(it->data->stoneBody);
		}
	}


	//Check Bonus Times
	for (p2List_item<Bonus*>* it = listBonus.getFirst(); it != nullptr; it = it->next) {

		if (it->data->bonusBody->body == nullptr)
			continue;

		b2Vec2 pos = it->data->bonusBody->body->GetPosition();
		if (it->data->active == true ) {
			it->data->currentTime = SDL_GetTicks();

			if (it->data->lastTime + BONUS_TIME < it->data->currentTime)
			{
			it->data->lastTime = it->data->currentTime;
			it->data->active = false;
			actualBonus-=it->data->bonusValue;
			}
		}

	}
	
	//Blit 6 Flippers
	p2List_item<PhysBody*>* it = boxes.getFirst();

		b2Vec2 pos = it->data->body->GetPosition();
		float angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width-7), METERS_TO_PIXELS(pos.y - it->data->height-12), NULL, NULL, angle);
	
		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width-5), METERS_TO_PIXELS(pos.y - it->data->height-14), NULL, NULL, angle);

		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 12), NULL, NULL, angle);

		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 14), NULL, NULL, angle);

		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 12), NULL, NULL, angle);

		it = it->next;
		pos = it->data->body->GetPosition();
		angle = RADTODEG*it->data->body->GetAngle();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width - 5), METERS_TO_PIXELS(pos.y - it->data->height - 14), NULL, NULL, angle);



	//APPLY FORCES TO FLIPPERS

	low_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	low_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	up_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	up_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
		up_left_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
		low_left_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
		up_right_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);
		low_right_flip->body->ApplyForceToCenter(b2Vec2(0, -60), true);

	}




	//CHECK IF BALL IS UNDER THRESHOLD
	b2Vec2 ballpos = ball->body->GetPosition();
	if (METERS_TO_PIXELS(ballpos.y) > SCREEN_HEIGHT+20 && game_over == false)
	{
		myLife--;
		
		if (myLife == -1)
		{
			game_over = true;

			App->fade->FadeToBlack(this, this);

		}
		else
		{
			App->physics->DestroyBody(ball);
			delete ball;
			ball = App->physics->CreateCircle(305, 750, 6, 1, 0, ball_texture);
		}
			
	}


	//BLIT SCORE AND LIFES
	p2SString title("Zomball Score: %i Balls: %i", myScore, myLife);

	if (game_over)
		title="GAME OVER- Score: %i", myScore;

	App->window->SetTitle(title.GetString());



	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;



	for (uint i = 0; i < headstone.count(); ++i) {
		p2List_item<HeadStone*>* item = headstone.getFirst();
		for (; item != NULL; item = item->next) {
			if (item->data->stoneBody == bodyA) {
				item->data->life--;
				App->audio->PlayFx(item->data->fx);
				myScore += ToScore(item->data->points);
				return;
			}
		}
	}
	for (uint i = 0; i < listBonus.count(); ++i) {
		p2List_item<Bonus*>* item = listBonus.getFirst();
		for (; item != NULL; item = item->next) {
			if (item->data->bonusBody == bodyA) {
				if (item->data->active == false) {
					App->audio->PlayFx(item->data->fx);
					actualBonus += item->data->bonusValue;
				}
				item->data->active = true;
				item->data->lastTime = SDL_GetTicks();
				return;
			}
		}
	}

}

uint ModuleSceneIntro::ToScore(uint score)
{
	uint ret;
	if (actualBonus <= 0) 
		actualBonus = 1;
	
	return ret = actualBonus*score;
}

HeadStone::HeadStone(ModuleSceneIntro* scene, uint life, uint points, uint fx, int number, PhysBody* stoneBody) :
	life(life), points(points), fx(fx), number(number), stoneBody(stoneBody)
{

}

Bonus::Bonus(ModuleSceneIntro * scene, uint fx, int bonusValue, PhysBody * bonusBody) :
	fx(fx), bonusValue(bonusValue), bonusBody(bonusBody)
{
	currentTime = SDL_GetTicks();
	lastTime = currentTime;
}

Brain::Brain(ModuleSceneIntro * scene, uint points, PhysBody * brainBody) :brainBody(brainBody), points(points)
{

	brainBody->width += brainBody->width;

}


