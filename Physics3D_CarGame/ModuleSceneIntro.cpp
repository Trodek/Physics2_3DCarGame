#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	inner_icosphere = new Icosphere(40);
	inner_icosphere->SetPos(0, 21, 0);
	outer_icosphere = new Icosphere(40.5f);
	outer_icosphere->SetPos(0, 21, 0);

	App->physics->AddIcocapsule(*inner_icosphere, *outer_icosphere, icocapsule_bodies);

	total_triangles = inner_icosphere->triangles.count();

	timer.Stop();
	delay_timer.Stop();

	App->audio->PlayMusic("Audio/Music.ogg");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (delay_timer.Read() > 3000) { //delay execution to give time player to prepare for next round
		delay = false;
		App->audio->PlayMusic("Audio/Music.ogg"); // Restart audio for new round
		delay_timer.Start();
		delay_timer.Stop();
	}

	if (painted_triangles == total_triangles) {
		timer.Stop();
		if (timer.Read() < fastest_time)
			fastest_time = timer.Read();
		timer.Start();
		timer.Stop();
		ResetLevel();
		delay = true; 
		delay_timer.Start();
	}

	if (timer.Read() > max_time) {
		timer.Start();
		timer.Stop();
		ResetLevel();
		delay = true;
		delay_timer.Start();
	}

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	inner_icosphere->InnerDraw();

	char title[80];
	sprintf_s(title, "Velocity: %.1f Km/h, Score: %d/%d, Time: %d:%.2d, Fastest Time: %d:%.2d", App->player->vehicle->GetKmh(), painted_triangles, total_triangles,
		timer.Read() / (1000 * 60), timer.Read() / 1000 % 60, fastest_time / (1000 * 60), fastest_time / 1000 % 60);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->triangle != nullptr){
		if(body1->triangle->r == 1.0f)
			painted_triangles++;
		body1->triangle->r = 0.0f;
		body1->triangle->g = 1.0f;
	}
}

void ModuleSceneIntro::ResetLevel()
{
	for (p2List_item<Triangle3D*>* triangle = inner_icosphere->triangles.getFirst(); triangle; triangle = triangle->next) {
		triangle->data->r = 1.0f;
		triangle->data->g = 0.0f;
	}
	painted_triangles = 0;
	App->player->ResetPlayer();
}

