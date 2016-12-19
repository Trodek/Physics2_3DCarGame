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

	// Car properties ----------------------------------------
	car.chassis_size.Set(1.5, 1.5, 1.5);
	car.chassis_offset.Set(0, 1, -2);
	car.saddle_size.Set(1.75, .5, 2);
	car.saddle_offset.Set(0, 2, -1.85);
	car.downchassis_size.Set(1.5, .1, 3);
	car.downchassis_offset.Set(0, .3, -.3);
	car.frontwheelsupport_size.Set(1.5, 1, .5);
	car.frontwheelsupport_offset.Set(0, 0.75, 1);
	car.mudguard_size.Set(.6, .2, .8);
	car.mudguard_offset.Set(0, 0.5, 1.8);
	car.handelbarsupport_size.Set(1.5, .5, .6);
	car.handelbarsupport_offset.Set(0, 1.1, 1.55);

	car.sensor_offset.Set(0, -.6, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 55.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 60000.0f;

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
	vehicle->SetPos(0, 5, 0);
	vehicle->collision_listeners.add(this);
	
	

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

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}
	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
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



