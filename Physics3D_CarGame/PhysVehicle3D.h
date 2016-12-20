#pragma once

#include "PhysBody3D.h"
#include "glmath.h"

class btRaycastVehicle;
struct PhysBody3D;

struct Wheel
{
	vec3 connection; // origin of the ray. Must come from within the chassis
	vec3 direction; 
	vec3 axis;
	float suspensionRestLength; // max length for suspension in meters
	float radius;
	float width;
	bool front; // is front wheel ?
	bool drive; // does this wheel received engine power ?
	bool brake; // does breakes affect this wheel ?
	bool steering; // does this wheel turns ?
};

struct VehicleInfo
{
	~VehicleInfo();
	
	vec3 collision_size;
	vec3 collision_offset;

	//motorbike
	vec3 chassis_size;
	vec3 chassis_offset;
	float handlebarright_radius;
	float handlebarright_height;
	vec3 handlebarright_offset;
	float handlebarleft_radius;
	float handlebarleft_height;
	vec3 handlebarleft_offset;
	vec3 saddle_size;
	vec3 saddle_offset;
	vec3 handlebarsupport_size;
	vec3 handlebarsupport_offset;
	float handelbarpost_radius;
	float handelbarpost_height;
	vec3 handelbarpost_offset;
	vec3 mudguard_size;
	vec3 mudguard_offset;
	vec3 frontwheelsupport_size;
	vec3 frontwheelsupport_offset;
	vec3 downchassis_size;
	vec3 downchassis_offset;
	vec3 panel_size;
	vec3 panel_offset;

	//Italian guy
	float head_radius;
	vec3 head_offset;
	vec3 body_size;
	vec3 body_offset;
	vec3 rightarm_size;
	vec3 rightarm_offset;
	vec3 leftarm_size;
	vec3 leftarm_offset;
	vec3 hip_size;
	vec3 hip_offset;
	vec3 leftlegup_size;
	vec3 leftlegup_offset;
	vec3 leftlegdown_size;
	vec3 leftlegdown_offset;
	vec3 rightlegup_size;
	vec3 rightlegup_offset;
	vec3 rightlegdown_size;
	vec3 rightlegdown_offset;


	vec3 sensor_offset;
	PhysBody3D* sensor;
	float mass;
	float suspensionStiffness; // default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression; // default to 0.83
	float suspensionDamping; // default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm; // default to 500 cm suspension can be compressed
	float frictionSlip; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce; // defaults to 6000 / max force to the chassis

	Wheel* wheels;
	int num_wheels;
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;
public:

	VehicleInfo info;
	btRaycastVehicle* vehicle;
};