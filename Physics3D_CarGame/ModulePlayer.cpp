#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	float total_length = 5.0f;

	car.collision_size.Set(1.5, 4, 5);
	car.collision_offset.Set(0, 2.5, -.4);

	// Motorbike properties ----------------------------------------
	car.chassis_size.Set(1.5, 1.5, 1.5);
	car.chassis_offset.Set(0, 1, -2);

	car.saddle_size.Set(1.75, .5, 2);
	car.saddle_offset.Set(0, 2, -1.85);

	car.downchassis_size.Set(1.5, .1, 3);
	car.downchassis_offset.Set(0, .3, -.3);

	car.frontwheelsupport_size.Set(1.5, .9, .55);
	car.frontwheelsupport_offset.Set(0, 0.75, 1);

	car.mudguard_size.Set(.6, .2, .8);
	car.mudguard_offset.Set(0, 0.5, 1.8);

	car.handlebarsupport_size.Set(1.5, .5, 1);
	car.handlebarsupport_offset.Set(0, 1.075, 1.35);

	car.handelbarpost_radius = 0.2;
	car.handelbarpost_height = 4;
	car.handelbarpost_offset.Set(0, 1.8, 1.2);

	car.panel_size.Set(1, .5, .4);
	car.panel_offset.Set(0, 3.6, 0.7);

	car.handlebarright_radius = 0.1;
	car.handlebarright_height = .6;
	car.handlebarright_offset.Set(-.35, 3.6, .3);

	car.handlebarleft_radius = 0.1;
	car.handlebarleft_height = .6;
	car.handlebarleft_offset.Set(.35, 3.6, .3);

	car.sensor_offset.Set(0, -.6, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 55.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 60000.0f;

	//Italian guy
	car.hip_size.Set(1.2, .5, .5);
	car.hip_offset.Set(0, 2.5, -2);

	car.body_size.Set(1.2, 2, .5);
	car.body_offset.Set(0, 3.75, -2);

	car.leftarm_size.Set(.5, .5, 2.5);
	car.leftarm_offset.Set(.6, 4.1, -1);

	car.rightarm_size.Set(.5, .5, 2.5);
	car.rightarm_offset.Set(-.6, 4.1, -1);

	car.leftlegup_size.Set(.5, .5, 1);
	car.leftlegup_offset.Set(.4, 2.5, -1.3);

	car.rightlegup_size.Set(.5, .5, 1);
	car.rightlegup_offset.Set(-.4, 2.5, -1.3);

	car.leftlegdown_size.Set(.5, 2.5, .5);
	car.leftlegdown_offset.Set(.4, 1.5, -.55);

	car.rightlegdown_size.Set(.5, 2.5, .5);
	car.rightlegdown_offset.Set(-.4, 1.5, -.55);

	car.head_radius = .6;
	car.head_offset.Set(0, 5.3, -2);

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.5f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;
	
	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = total_length*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

// Sensor Creation
	sensore_cube.Scale(.5, .8, .5);
	mat4x4 trans;
	car.sensor = App->physics->AddBody(sensore_cube);
	car.sensor->SetAsSensor(true);

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 3, 0);
	vehicle->collision_listeners.add(this);
	
	engine_normal_fx = App->audio->LoadFx("Audio/engine_normal.wav");
	engine_accel_fx = App->audio->LoadFx("Audio/engine_accel.wav");

	App->audio->PlayFx(engine_normal_fx, -1);
	playing_normal = true;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
}

void ModulePlayer::ResetPlayer()
{
	SetInitialPos();
	timer_started = false;
}

void ModulePlayer::SetInitialPos()
{
	vehicle->GetBody()->setLinearVelocity(btVector3(0, 0, 0));
	vehicle->GetBody()->setAngularVelocity(btVector3(0, 0, 0));

	btTransform startTransform;
	startTransform.setIdentity();
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	vehicle->GetBody()->setMotionState(myMotionState);

	vehicle->SetPos(0, 3, 0);
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (!App->scene_intro->delay) { // Don't allow input when restarting level
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			acceleration = max_acceleration;
			if (!timer_started) {
				App->scene_intro->timer.Start();
				timer_started = true;
			}
			if (playing_normal) {
				App->audio->StopFX();
				App->audio->PlayFx(engine_accel_fx, -1);
				playing_normal = false;
				playing_accel = true;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			if (playing_accel) {
				App->audio->StopFX();
				App->audio->PlayFx(engine_normal_fx, -1);
				playing_normal = true;
				playing_accel = false;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			brake = BRAKE_POWER;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			SetInitialPos();
		}

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
			max_acceleration = 500.0f;
			easy_mode = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
			max_acceleration = 700.0f;
			easy_mode = true;
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	mat4x4 trans;
	vec4 offset(0, -.7, 0, 0);
	vehicle->GetTransform(trans.M);
	offset = trans*offset;
	vehicle->info.sensor->SetPos(trans.M[12]+offset.x, trans.M[13]+offset.y, trans.M[14]+offset.z);

	return UPDATE_CONTINUE;
}



