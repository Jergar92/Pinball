#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
class PhysBody;
struct SDL_Texture;
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;

	PhysBody* sensor;
	bool sensed;

	

	SDL_Texture* ball;
	SDL_Texture* left_flip;
	SDL_Texture* right_flip;
	
	p2List_item<PhysBody*>* flips_start;


	unsigned int score;


	PhysBody* mid_left_flip;
	PhysBody* mid_right_flip;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

};