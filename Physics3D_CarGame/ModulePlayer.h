#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 500.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void ResetPlayer();
	void SetInitialPos();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	Cube sensore_cube;
	bool timer_started = false;

	uint engine_normal_fx;
	uint engine_accel_fx;
	bool playing_normal = false;
	bool playing_accel = false;
};