#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2SString.h"
#include "Globals.h"
#include "p2DynArray.h"
#define BONUS_TIME 10000
class PhysBody;
class ModuleSceneIntro;
struct Brain {
	Brain(ModuleSceneIntro* scene, uint points, uint fx, PhysBody* brainBody);
	PhysBody* brainBody;
	uint points;
	uint fx;


};
struct HeadStone 
{

	HeadStone(ModuleSceneIntro* scene, uint points,uint ac, uint fx, int lapidnumber, PhysBody* stoneBody);
	
	int life = 0;
	int number;
	PhysBody* stoneBody;
	uint points;
	uint fx;
	~HeadStone(){}
};
struct Squeleton
{

	Squeleton(ModuleSceneIntro* scene, uint points, uint fx, int squeletonNumber, PhysBody* squeletonBody);

	int number;
	PhysBody* squeletonBody;
	uint points;
	uint fx;
};
struct Bumper
{

	Bumper(ModuleSceneIntro* scene, uint points, uint fx, int bumpernumber, PhysBody* bumperBody);

	int number;
	PhysBody* bumperBody;
	uint points;
	uint fx;
};
struct Bonus {

	Bonus(ModuleSceneIntro* scene,uint fx, int bonusnumber, PhysBody* bonusBody);

	PhysBody* bonusBody;
	bool active = false;
	uint fx;
	uint currentTime = NULL;
	uint lastTime = NULL;
	uint bonusValue = NULL;
	~Bonus() {}
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> chains;

	
	SDL_Texture* background;
	SDL_Texture* ball_texture;
	SDL_Texture* left_flip;
	SDL_Texture* right_flip;
	SDL_Texture*  grave_ok[4];
	SDL_Texture* grave_des[4];
	SDL_Texture* bonus_tex[4];
	SDL_Texture* brain_text;
	SDL_Texture* bell_text;
	SDL_Texture* squeleton_tex[5];
	p2List_item<PhysBody*>* flips_start;
	SDL_Texture* bumper_text[2];

	PhysBody* low_left_flip;
	PhysBody* low_right_flip;
	PhysBody* mid_left_flip;
	PhysBody* mid_right_flip;
	PhysBody* up_left_flip;
	PhysBody* up_right_flip;
	PhysBody* bell;
	PhysBody* bell_sensor;
	PhysBody* ball;


	Brain* brain;
	p2List<HeadStone*> headstone;
	p2List<Bumper*> bumpers;
	p2List<Bonus*> listBonus;

	p2List<Squeleton*> squeletons;

	//Score functions
	uint ToScore(uint score);
	
	//Score and life values
	uint myScore;
	int myLife;
	uint actualBonus=1;
	 
	
	//Audios
	uint gravesFx[4];
	uint bonusFx;
	uint brainFx;
	uint BellFx;
	uint FlipFx;
	uint bumperFx;
	uint EvilLaugh;
	uint Game_Over_Laugh;

	uint squeletonFx;

	p2Point<int> ray;
	bool ray_on;
	bool change_sensor;
};
