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
	Brain(ModuleSceneIntro* scene, uint points, PhysBody* lapidBody);
	PhysBody* brainBody;
	uint points;


};
struct HeadStone 
{

	HeadStone(ModuleSceneIntro* scene, uint points,uint ac, int lapidnumber, PhysBody* lapidBody);
	
	int life = 0;
	int number;
	PhysBody* stoneBody;
	uint points;
	uint fx;
	~HeadStone(){}
};
struct Bonus {

	Bonus(ModuleSceneIntro* scene, int bonusnumber, PhysBody* lapidBody);

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

	p2List_item<PhysBody*>* flips_start;

	PhysBody* low_left_flip;
	PhysBody* low_right_flip;
	PhysBody* mid_left_flip;
	PhysBody* mid_right_flip;
	PhysBody* up_left_flip;
	PhysBody* up_right_flip;

	PhysBody* ball;

	Brain* brain;
	p2List<HeadStone*> headstone;
	p2List<Bonus*> listBonus;
	
	//Score functions
	uint ToScore(uint score);
	
	//Score and life values
	uint myScore;
	uint myLife;
	uint actualBonus=1;
	bool game_over;
	
	//Audios
	uint gravesFx[4];
	uint bonusFx;

	p2Point<int> ray;
	bool ray_on;
};
