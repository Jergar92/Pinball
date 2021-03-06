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
	myLife = 3;
	change_sensor = false;


	//LoadMusic

	App->audio->PlayMusic("Game/sounds/game_music.ogg", 0.0f);
	int i = 0;
	gravesFx[i++] = App->audio->LoadFx("Game/sounds/grave1.wav");
	gravesFx[i++] = App->audio->LoadFx("Game/sounds/grave2.wav");
	gravesFx[i++] = App->audio->LoadFx("Game/sounds/grave3.wav");
	gravesFx[i++] = App->audio->LoadFx("Game/sounds/grave4.wav");
	bonusFx = App->audio->LoadFx("Game/sounds/ding_snd.wav");
	brainFx = App->audio->LoadFx("Game/sounds/squish.wav");
	BellFx = App->audio->LoadFx("Game/sounds/bell_snd.wav");
	FlipFx = App->audio->LoadFx("Game/sounds/impact_shovel.wav");
	bumperFx = App->audio->LoadFx("Game/sounds/brainHit.wav");
	squeletonFx = App->audio->LoadFx("Game/sounds/squeleton_hit.wav");
	Game_Over_Laugh = App->audio->LoadFx("Game/sounds/laugh.wav");
	EvilLaugh = App->audio->LoadFx("Game/sounds/evillaugh.wav");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Load Images
	background = App->textures->Load("Game/Sprites/pinball_back.png");
	ball_texture = App->textures->Load("Game/Sprites/ball.png");
	bell_text = App->textures->Load("Game/Sprites/Bell.png");
	left_flip = App->textures->Load("Game/Sprites/FlipLeft.png");
	right_flip = App->textures->Load("Game/Sprites/FlipRight.png");
	brain_text = App->textures->Load("Game/Sprites/brain.png");
	game_over_text= App->textures->Load("Game/Sprites/game_over.png");

	for (int i = 0; i < 4; i++)
	{
		p2SString tmp1("Game/Sprites/Grave_%i_Ok.png", i+1);
		p2SString tmp2("Game/Sprites/Grave_%i_Des.png", i+1);

		grave_ok[i] = App->textures->Load(tmp1.GetString());
		grave_des[i] = App->textures->Load(tmp2.GetString());
	}

	for (int i = 0; i < 4; i++)
	{

		p2SString tmp("Game/Sprites/x%i.png", i + 2);
		bonus_tex[i] = App->textures->Load(tmp.GetString());
	}
	for (int i = 0; i < 5; i++)
	{
		//0 look east 1 look south-east 2 look south 3 look south-west 4 look west
		p2SString tmp("Game/Sprites/Squeleton_%i.png", i);
		squeleton_tex[i] = App->textures->Load(tmp.GetString());
	}
	i = 0;
	bumper_text[i++]= App->textures->Load("Game/Sprites/brainbot_l.png");
	bumper_text[i++] = App->textures->Load("Game/Sprites/brainbot_r.png");
	

	//ADD BALL
	ball = App->physics->CreateCircle(305, 750, 6, 1, 0, ball_texture);
	ball->body->SetBullet(true);

	int brain_cords[26] = {
		78, 63,
		92, 59,
		104, 45,
		101, 23,
		82, 9,
		57, 0,
		28, 1,
		8, 14,
		0, 29,
		0, 44,
		15, 60,
		45, 72,
		71, 69
	};
	//ADD BRAIN
	chains.add(App->physics->CreateChain(-70, 360, brain_cords, 26, 0, 1.0f));
	brain = new Brain(this, 10, brainFx, chains.getLast()->data);
	chains.getLast()->data->listener = this;

	//ADD SQUELETONS
	circles.add(App->physics->CreateCircle(20, 90, 15, 0, 1, NULL));
	squeletons.add(new Squeleton(this, 8, squeletonFx,0, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(60, 55, 15, 0, 1, NULL));
	squeletons.add(new Squeleton(this, 8, squeletonFx, 1, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(155, 25, 15, 0, 1, NULL));
	squeletons.add(new Squeleton(this, 8, squeletonFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(250, 50, 15, 0, 1, NULL));
	squeletons.add(new Squeleton(this, 8, squeletonFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircle(300, 90, 15, 0, 1, NULL));
	squeletons.add(new Squeleton(this, 8, squeletonFx, 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	//ADD GRAVES


	int grave_1_chain[10] = { 28, 27,
		28, 17,
		4, 14,
		2, 25,
		23, 26 };

	int grave_2_chain[10] = {
		1, 27,
		9, 20,
		33, 33,
		24, 40,
		4, 29 };

	int grave_3_chain[10] = { 1, 17,
		24, 16,
		29, 24,
		0, 27,
		-1, 20 };

	int grave_4_chain[12] = {-1, 36,
		4, 42,
		31, 32,
		29, 26,
		22, 24,
		0, 31};

	chains.add(App->physics->CreateChain(100,5, grave_4_chain, 12, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[3], 4, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	chains.add(App->physics->CreateChain(190, 10, grave_2_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[3], 2, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	chains.add(App->physics->CreateChain(65, 335, grave_4_chain, 12, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[3], 4, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	chains.add(App->physics->CreateChain(60, 400, grave_4_chain, 12, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[3], 4, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	chains.add(App->physics->CreateChain(135, 375, grave_3_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[2], 3, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	chains.add(App->physics->CreateChain(185, 390, grave_2_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[1], 2, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	chains.add(App->physics->CreateChain(218, 360, grave_2_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[1], 2, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	//////
	circles.add(App->physics->CreateChain(24, 600, grave_4_chain, 12, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[3], 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateChain(90, 640, grave_3_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[2], 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateChain(230, 607, grave_2_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[1], 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateChain(42, 670, grave_4_chain, 12, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[3], 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;


	circles.add(App->physics->CreateChain(190, 653, grave_2_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[1], 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;


	chains.add(App->physics->CreateChain(240, 680, grave_1_chain, 10, 0, 1.0f));
	headstone.add(new HeadStone(this, 10, 25, gravesFx[0], 1, chains.getLast()->data));
	chains.getLast()->data->listener = this;

	//
	circles.add(App->physics->CreateCircleSensor(103, 382, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(187, 382, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(143, 430, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 5, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(144, 486, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(94, 587, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(200, 587, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(119, 613, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(174, 613, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(144, 653, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 4, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(144, 693, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 3, circles.getLast()->data));
	circles.getLast()->data->listener = this;

	circles.add(App->physics->CreateCircleSensor(144, 729, 10, 0));
	listBonus.add(new Bonus(this, bonusFx, 2, circles.getLast()->data));
	circles.getLast()->data->listener = this;
	//

	//SET Bouncers

	int bumper_left[18] = {
		12, 32,
		21, 38,
		29, 38,
		38, 34,
		44, 25,
		42, 15,
		33, 10,
		22, 13,
		17, 21
	};
	int bumper_right[18] = {
		47, 31,
		40, 39,
		38, 38,
		25, 37,
		15, 28,
		14, 18,
		22, 11,
		34, 12,
		41, 19
	};

	
	chains.add(App->physics->CreateChain(170, 740, bumper_left,18,0, 1.3f));
	bumpers.add(new Bumper(this,5, bumperFx,0,chains.getLast()->data));
	chains.getLast()->data->listener = this; 
	
	chains.add(App->physics->CreateChain(60, 740, bumper_right, 18, 0, 1.3f));
	bumpers.add(new Bumper(this, 5, bumperFx, 1, chains.getLast()->data));
	chains.getLast()->data->listener = this;
	// Pivot 0, 0
	int pinball_exterior[184] = {
		162, 23,
		191, 26,
		233, 37,
		276, 60,
		293, 81,
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
		42, 62,
		64, 47,
		82, 38,
		102, 32,
		130, 25,
		159, 23
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
		80, 524,
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
		80, 804,
		85, 805,
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
		210, 805,
		214, 814,
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
	boxes.add(App->physics->CreateRectangle(88 + 25, 816, 50, 12, 1, left_flip));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(88 + 25, 836, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	low_left_flip = circles.getLast()->data;

     	//BOTTOM RIGHT
	circles.add(App->physics->CreateCircle(205, 814, 6, 0));
	boxes.add(App->physics->CreateRectangle(205 - 25, 814, 50, 12, 1, right_flip));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(205 - 25, 834, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	low_right_flip = circles.getLast()->data;

		//MID LEFT
	circles.add(App->physics->CreateCircle(85, 530, 6, 0));
	boxes.add(App->physics->CreateRectangle(85+25, 530, 50, 12, 1, left_flip));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(85+25, 550, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	mid_left_flip = circles.getLast()->data;

		//MID RIGHT
	circles.add(App->physics->CreateCircle(209, 529, 6, 0));
	boxes.add(App->physics->CreateRectangle(209 - 25, 529, 50, 12, 1, right_flip));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(209 -25, 529, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	mid_right_flip = circles.getLast()->data;
	
	//UP LEFT
	circles.add(App->physics->CreateCircle(100, 250, 6, 0));
	boxes.add(App->physics->CreateRectangle(100 + 25, 250, 50, 12, 1, left_flip));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(100 + 25, 270, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0));
	up_left_flip = circles.getLast()->data;

	//UP RIGHT
	circles.add(App->physics->CreateCircle(220, 250, 6, 0));
	boxes.add(App->physics->CreateRectangle(220 - 25, 250, 50, 12, 1, right_flip));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(0.5, 0), p2Point<float>(0, 0), 0, 25, -20);
	circles.add(App->physics->CreateCircle(220 - 25, 270, 6, 1));
	App->physics->CreateRevolutionJoint(boxes.getLast()->data->body, circles.getLast()->data->body, p2Point<float>(-0.5, 0), p2Point<float>(0, 0));
	up_right_flip = circles.getLast()->data;
	



	//ADD BELL AND SENSOR
	boxes.add(App->physics->CreateRectangle(255, 305, 20, 10, 0));
	bell = boxes.getLast()->data;
	bell->body->SetTransform(b2Vec2(PIXEL_TO_METERS(255), PIXEL_TO_METERS(305)), DEGTORAD * 45);
	bell->listener = this;
	bell->body->SetActive(false);

	boxes.add(App->physics->CreateRectangleSensor(248, 310, 30, 10));
	bell_sensor = boxes.getLast()->data;
	bell_sensor->body->SetTransform(b2Vec2(PIXEL_TO_METERS(248), PIXEL_TO_METERS(310)), DEGTORAD * 45);
	bell_sensor->listener = this;


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
	App->textures->Unload(brain_text);
	App->textures->Unload(bell_text);
	App->textures->Unload(game_over_text);

	for (int i = 0; i < 4; i++)
	{
		App->textures->Unload(grave_ok[i]);
		App->textures->Unload(grave_des[i]);
		App->textures->Unload(bonus_tex[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		App->textures->Unload(squeleton_tex[i]);
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

	for (p2List_item<Squeleton*>* it = squeletons.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data->squeletonBody);
	}
	squeletons.clear();
	for (p2List_item<Bumper*>* it = bumpers.getFirst(); it != nullptr; it = it->next)
	{
		App->physics->DestroyBody(it->data->bumperBody);
	}
	bumpers.clear();
	App->physics->DestroyBody(brain->brainBody);



	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0);
//	LOG("myScore=%i, myBonus=%i", myScore,actualBonus);

	if (change_sensor)
	{
		change_sensor = false;
		bell->body->SetActive(true);
		bell_sensor->body->SetActive(false);
	}

	for (p2List_item<PhysBody*>* it = circles.getFirst(); it != nullptr; it = it->next)
	{

		if (it->data->body == nullptr)
			continue;

		//Blit Circles
		b2Vec2 pos = it->data->body->GetPosition();
		App->renderer->Blit(it->data->texture, METERS_TO_PIXELS(pos.x - it->data->width), METERS_TO_PIXELS(pos.y - it->data->height));
	}
	//Blit brain
	b2Vec2 brainpos = brain->brainBody->body->GetPosition();
	App->renderer->Blit(brain_text, METERS_TO_PIXELS(brainpos.x- brain->brainBody->width), METERS_TO_PIXELS(brainpos.y - brain->brainBody->height));
	//Blit Bonus
	for (p2List_item<Bonus*>* it = listBonus.getFirst(); it != nullptr; it = it->next) {

		if (it->data->bonusBody->body == nullptr)
			continue;

		b2Vec2 pos = it->data->bonusBody->body->GetPosition();

		if (it->data->active == true) {
			App->renderer->Blit(bonus_tex[it->data->bonusValue-2], METERS_TO_PIXELS(pos.x - it->data->bonusBody->width), METERS_TO_PIXELS(pos.y - it->data->bonusBody->height));
		}

	}
	//Blit Bumpers
	for (p2List_item<Bumper*>* it = bumpers.getFirst(); it != nullptr; it = it->next) {

		if (it->data->bumperBody->body == nullptr)
			continue;
		b2Vec2 pos = it->data->bumperBody->body->GetPosition();
		App->renderer->Blit(bumper_text[it->data->number], METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y));

	}


	//Blit Ball
	if(ball!=nullptr)
	{	
		b2Vec2 pos =ball->body->GetPosition();
		App->renderer->Blit(ball->texture, METERS_TO_PIXELS(pos.x - ball->width), METERS_TO_PIXELS(pos.y - ball->height));
	}

	//Blit Bell
		App->renderer->Blit(bell_text, 225, 255);


	//Blit Graves
	for (p2List_item<HeadStone*>* it = headstone.getFirst(); it != nullptr; it = it->next){
		
		if (it->data->stoneBody->body == nullptr)
			continue;
		
		b2Vec2 pos = it->data->stoneBody->body->GetPosition();
		
		if (it->data->life > 6) {
			App->renderer->Blit(grave_ok[it->data->number-1], METERS_TO_PIXELS(pos.x - it->data->stoneBody->width), METERS_TO_PIXELS(pos.y - it->data->stoneBody->height));
		}
		else if (it->data->life > 0)
		{
			App->renderer->Blit(grave_des[it->data->number-1], METERS_TO_PIXELS(pos.x - it->data->stoneBody->width), METERS_TO_PIXELS(pos.y - it->data->stoneBody->height));
		}
		else if (it->data->life <= 0)
		{
			App->physics->DestroyBody(it->data->stoneBody);
		}
	}
	// Blit Squeletons
	for (p2List_item<Squeleton*>* it = squeletons.getFirst(); it != nullptr; it = it->next) {
		if (it->data->squeletonBody->body == nullptr)
			continue;
		b2Vec2 pos = it->data->squeletonBody->body->GetPosition();

		App->renderer->Blit(squeleton_tex[it->data->number], METERS_TO_PIXELS(pos.x - it->data->squeletonBody->width), METERS_TO_PIXELS(pos.y - it->data->squeletonBody->height));
		
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
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(FlipFx);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, -70), true);
		up_left_flip->body->ApplyForceToCenter(b2Vec2(0, -70), true);
		low_left_flip->body->ApplyForceToCenter(b2Vec2(0, -70), true);
	
	}
	else
	{
		low_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
		mid_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
		up_left_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, -70), true);
		up_right_flip->body->ApplyForceToCenter(b2Vec2(0, -70), true);
		low_right_flip->body->ApplyForceToCenter(b2Vec2(0, -70), true);

	}	
	else
	{
		low_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
		mid_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
		up_right_flip->body->ApplyForceToCenter(b2Vec2(0, 10), true);
	}



	//CHECK IF BALL IS UNDER THRESHOLD
	if (ball != nullptr)
	{
		b2Vec2 ballpos = ball->body->GetPosition();
		if (METERS_TO_PIXELS(ballpos.y) > SCREEN_HEIGHT + 20)
		{
			bell->body->SetActive(false);
			bell_sensor->body->SetActive(true);			

			if (myLife > 0)
				myLife--;

			if (myLife == 0)
			{
				App->audio->PlayFx(Game_Over_Laugh);
				App->fade->FadeToBlack(this, this, 6.0);
				App->physics->DestroyBody(ball);
				ball = nullptr;
			}

			else if (myLife > 0)
			{
				App->audio->PlayFx(EvilLaugh);
				App->physics->DestroyBody(ball);
				ball = App->physics->CreateCircle(305, 750, 6, 1, 0, ball_texture);
			}

		}
	}



	//BLIT SCORE,LIFES AND GAME_OVER SCREEN;
	p2SString title("Zomball Score: %i Balls: %i", myScore, myLife);

	App->window->SetTitle(title.GetString());

	if (myLife == 0)
	{
		App->renderer->Blit(game_over_text, 15, 325);
	}


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (brain->brainBody == bodyA) {
		App->audio->PlayFx(brain->fx);
		myScore += ToScore(brain->points);
		return;
	}
	        

	if (bell_sensor == bodyA)
	{
		change_sensor = true;
		App->audio->PlayFx(BellFx);
		return;
	}

	if (bell == bodyA)
	{
		App->audio->PlayFx(BellFx);
		return;
	}
	for (uint i = 0; i < bumpers.count(); ++i) {
		p2List_item<Bumper*>* item = bumpers.getFirst();
		for (; item != NULL; item = item->next) {
			if (item->data->bumperBody == bodyA) {
				App->audio->PlayFx(item->data->fx);
				myScore += ToScore(item->data->points);
				return;
			}
		}
	}

	for (uint i = 0; i < squeletons.count(); ++i) {
		p2List_item<Squeleton*>* item = squeletons.getFirst();
		for (; item != NULL; item = item->next) {
			if (item->data->squeletonBody == bodyA) {
				App->audio->PlayFx(item->data->fx);
				myScore += ToScore(item->data->points);
				return;
			}
		}
	}
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

Brain::Brain(ModuleSceneIntro * scene, uint points,uint fx, PhysBody * brainBody) :
	 points(points),fx(fx),brainBody(brainBody)
{
}
Squeleton::Squeleton(ModuleSceneIntro * scene, uint points, uint fx, int number, PhysBody * squeletonBody) :
	points(points), fx(fx),number(number), squeletonBody(squeletonBody)
{
}
Bumper::Bumper(ModuleSceneIntro * scene, uint points, uint fx, int number, PhysBody * bumperBody) :
	points(points), fx(fx), number(number),bumperBody(bumperBody)
{
}



