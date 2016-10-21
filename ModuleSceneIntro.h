#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

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

	PhysBody* sensor;
	bool sensed;


	
	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* left_flip;
	SDL_Texture* right_flip;
	SDL_Texture* lapid4;
	SDL_Texture* lapid3;
	SDL_Texture* lapid2;
	SDL_Texture* lapid1;

	p2List_item<PhysBody*>* flips_start;

	PhysBody* mid_left_flip;
	PhysBody* mid_right_flip;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
