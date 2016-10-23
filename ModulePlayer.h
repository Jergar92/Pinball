#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
struct SDL_Texture;
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* bounc;

	PhysBody* bouncer;
	PhysBody* bouncerWheel;
	unsigned int score;

};