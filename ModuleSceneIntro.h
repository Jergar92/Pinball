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

};
struct HeadStone 
{

	HeadStone(ModuleSceneIntro* scene, uint points,uint ac, const char* lapidnumber, PhysBody* lapidBody);

	SDL_Texture* texture[2];
	PhysBody* stoneBody;
	~HeadStone(){}
};
struct Bonus {

	Bonus(ModuleSceneIntro* scene, const char* bonusnumber, PhysBody* lapidBody);

	SDL_Texture* texture;
	PhysBody* bonusBody;
	uint currentTime;
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

	p2List<PhysBody*> bonus;

	PhysBody* sensor;
	bool sensed;

	
	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* left_flip;
	SDL_Texture* right_flip;


	p2List_item<PhysBody*>* flips_start;

	PhysBody* low_left_flip;
	PhysBody* low_right_flip;
	PhysBody* mid_left_flip;
	PhysBody* mid_right_flip;
	PhysBody* up_left_flip;
	PhysBody* up_right_flip;

	Brain* brain;
	p2List<HeadStone*> headstone;
	p2List<Bonus*> listBonus;
	//Score functions
	uint ToScore(uint score);
	//Score values
	uint myScore;
	uint actualBonus=1;
	//
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
