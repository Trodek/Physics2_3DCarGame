#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color.Set(.16,.16,.16);

	for(int i = 1; i < vehicle->getNumWheels(); i+=2)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);
		vec4 displacement(-.6, 0, 0, 0);
		displacement = wheel.transform*displacement;
		wheel.transform.M[12] += displacement.x;
		wheel.transform.M[13] += displacement.y;
		wheel.transform.M[14] += displacement.z;

		wheel.Render();
	}
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	mat4x4 trans;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&trans);

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.color.Set(.5,.874,1);

	Cube saddle(info.saddle_size.x, info.saddle_size.y, info.saddle_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&saddle.transform);
	btVector3 saddle_offset(info.saddle_offset.x, info.saddle_offset.y, info.saddle_offset.z);
	saddle_offset = saddle_offset.rotate(q.getAxis(), q.getAngle());

	saddle.transform.M[12] += saddle_offset.getX();
	saddle.transform.M[13] += saddle_offset.getY();
	saddle.transform.M[14] += saddle_offset.getZ();

	saddle.color.Set(0.6,.301,0);

	Cube downchassis(info.downchassis_size.x, info.downchassis_size.y, info.downchassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&downchassis.transform);
	btVector3 downchassis_offset(info.downchassis_offset.x, info.downchassis_offset.y, info.downchassis_offset.z);
	downchassis_offset = downchassis_offset.rotate(q.getAxis(), q.getAngle());

	downchassis.transform.M[12] += downchassis_offset.getX();
	downchassis.transform.M[13] += downchassis_offset.getY();
	downchassis.transform.M[14] += downchassis_offset.getZ();

	downchassis.color.Set(.5, .874, 1);

	Cube frontwheelsupport(info.frontwheelsupport_size.x, info.frontwheelsupport_size.y, info.frontwheelsupport_size.z);
	frontwheelsupport.SetRotation(10, vec3(1, 0, 0));
	frontwheelsupport.transform = trans*frontwheelsupport.transform;
	btVector3 frontwheelsupport_offset(info.frontwheelsupport_offset.x, info.frontwheelsupport_offset.y, info.frontwheelsupport_offset.z);
	frontwheelsupport_offset = frontwheelsupport_offset.rotate(q.getAxis(), q.getAngle());

	frontwheelsupport.transform.M[12] += frontwheelsupport_offset.getX();
	frontwheelsupport.transform.M[13] += frontwheelsupport_offset.getY();
	frontwheelsupport.transform.M[14] += frontwheelsupport_offset.getZ();

	frontwheelsupport.color.Set(.5, .874, 1);

	Cube mudguard(info.mudguard_size.x, info.mudguard_size.y, info.mudguard_size.z);
	mudguard.SetRotation(-30, vec3(1, 0, 0));
	mudguard.transform = trans*mudguard.transform;
	btVector3 mudguard_offset(info.mudguard_offset.x, info.mudguard_offset.y, info.mudguard_offset.z);
	mudguard_offset = mudguard_offset.rotate(q.getAxis(), q.getAngle());

	mudguard.transform.M[12] += mudguard_offset.getX();
	mudguard.transform.M[13] += mudguard_offset.getY();
	mudguard.transform.M[14] += mudguard_offset.getZ();

	mudguard.color.Set(.5, .874, 1);

	Cube handelbarsupport(info.handlebarsupport_size.x, info.handlebarsupport_size.y, info.handlebarsupport_size.z);
	handelbarsupport.SetRotation(-10, vec3(1, 0, 0));
	handelbarsupport.transform = trans*handelbarsupport.transform;
	btVector3 handelbarsupport_offset(info.handlebarsupport_offset.x, info.handlebarsupport_offset.y, info.handlebarsupport_offset.z);
	handelbarsupport_offset = handelbarsupport_offset.rotate(q.getAxis(), q.getAngle());

	handelbarsupport.transform.M[12] += handelbarsupport_offset.getX();
	handelbarsupport.transform.M[13] += handelbarsupport_offset.getY();
	handelbarsupport.transform.M[14] += handelbarsupport_offset.getZ();

	handelbarsupport.color.Set(.5, .874, 1);

	Cylinder handelbarpost(info.handelbarpost_radius, info.handelbarpost_height);
	handelbarpost.SetRotation(-100, vec3(0.4, 0, 1));
	handelbarpost.transform = trans*handelbarpost.transform;
	btVector3 handelbarpost_offset(info.handelbarpost_offset.x, info.handelbarpost_offset.y, info.handelbarpost_offset.z);
	handelbarpost_offset = handelbarpost_offset.rotate(q.getAxis(), q.getAngle());

	handelbarpost.transform.M[12] += handelbarpost_offset.getX();
	handelbarpost.transform.M[13] += handelbarpost_offset.getY();
	handelbarpost.transform.M[14] += handelbarpost_offset.getZ();

	handelbarpost.color.Set(.55, .7, .85);

	Cube panel(info.panel_size.x, info.panel_size.y, info.panel_size.z);
	panel.transform = trans*panel.transform;
	btVector3 panel_offset(info.panel_offset.x, info.panel_offset.y, info.panel_offset.z);
	panel_offset = panel_offset.rotate(q.getAxis(), q.getAngle());

	panel.transform.M[12] += panel_offset.getX();
	panel.transform.M[13] += panel_offset.getY();
	panel.transform.M[14] += panel_offset.getZ();

	panel.color.Set(.5, .874, 1);

	Cylinder handlebarright(info.handlebarright_radius, info.handlebarright_height);
	handlebarright.SetRotation(-10, vec3(0, 1, 0));
	handlebarright.transform = trans*handlebarright.transform;
	btVector3 handlebarright_offset(info.handlebarright_offset.x, info.handlebarright_offset.y, info.handlebarright_offset.z);
	handlebarright_offset = handlebarright_offset.rotate(q.getAxis(), q.getAngle());

	handlebarright.transform.M[12] += handlebarright_offset.getX();
	handlebarright.transform.M[13] += handlebarright_offset.getY();
	handlebarright.transform.M[14] += handlebarright_offset.getZ();

	handlebarright.color = Black;

	Cylinder handlebarleft(info.handlebarleft_radius, info.handlebarleft_height);
	handlebarleft.SetRotation(10, vec3(0, 1, 0));
	handlebarleft.transform = trans*handlebarleft.transform;
	btVector3 handlebarleft_offset(info.handlebarleft_offset.x, info.handlebarleft_offset.y, info.handlebarleft_offset.z);
	handlebarleft_offset = handlebarleft_offset.rotate(q.getAxis(), q.getAngle());

	handlebarleft.transform.M[12] += handlebarleft_offset.getX();
	handlebarleft.transform.M[13] += handlebarleft_offset.getY();
	handlebarleft.transform.M[14] += handlebarleft_offset.getZ();

	handlebarleft.color = Black;

	Cube hip(info.hip_size.x, info.hip_size.y, info.hip_size.z);
	hip.transform = trans*hip.transform;
	btVector3 hip_offset(info.hip_offset.x, info.hip_offset.y, info.hip_offset.z);
	hip_offset = hip_offset.rotate(q.getAxis(), q.getAngle());

	hip.transform.M[12] += hip_offset.getX();
	hip.transform.M[13] += hip_offset.getY();
	hip.transform.M[14] += hip_offset.getZ();

	hip.color = Blue;

	Cube body(info.body_size.x, info.body_size.y, info.body_size.z);
	body.transform = trans*body.transform;
	btVector3 body_offset(info.body_offset.x, info.body_offset.y, info.body_offset.z);
	body_offset = body_offset.rotate(q.getAxis(), q.getAngle());

	body.transform.M[12] += body_offset.getX();
	body.transform.M[13] += body_offset.getY();
	body.transform.M[14] += body_offset.getZ();

	body.color.Set(.14,.56,.14);

	Cube leftarm(info.leftarm_size.x, info.leftarm_size.y, info.leftarm_size.z);
	leftarm.SetRotation(20, vec3(1, 0, 0));
	leftarm.transform = trans*leftarm.transform;
	btVector3 leftarm_offset(info.leftarm_offset.x, info.leftarm_offset.y, info.leftarm_offset.z);
	leftarm_offset = leftarm_offset.rotate(q.getAxis(), q.getAngle());

	leftarm.transform.M[12] += leftarm_offset.getX();
	leftarm.transform.M[13] += leftarm_offset.getY();
	leftarm.transform.M[14] += leftarm_offset.getZ();

	leftarm.color.Set(1, .85, .7);

	Cube rightarm(info.rightarm_size.x, info.rightarm_size.y, info.rightarm_size.z);
	rightarm.SetRotation(20, vec3(1, 0, 0));
	rightarm.transform = trans*rightarm.transform;
	btVector3 rightarm_offset(info.rightarm_offset.x, info.rightarm_offset.y, info.rightarm_offset.z);
	rightarm_offset = rightarm_offset.rotate(q.getAxis(), q.getAngle());

	rightarm.transform.M[12] += rightarm_offset.getX();
	rightarm.transform.M[13] += rightarm_offset.getY();
	rightarm.transform.M[14] += rightarm_offset.getZ();

	rightarm.color.Set(1, .85, .7);

	Cube rightlegup(info.rightlegup_size.x, info.rightlegup_size.y, info.rightlegup_size.z);
	rightlegup.transform = trans*rightlegup.transform;
	btVector3 rightlegup_offset(info.rightlegup_offset.x, info.rightlegup_offset.y, info.rightlegup_offset.z);
	rightlegup_offset = rightlegup_offset.rotate(q.getAxis(), q.getAngle());

	rightlegup.transform.M[12] += rightlegup_offset.getX();
	rightlegup.transform.M[13] += rightlegup_offset.getY();
	rightlegup.transform.M[14] += rightlegup_offset.getZ();

	rightlegup.color = Blue;

	Cube leftlegup(info.leftlegup_size.x, info.leftlegup_size.y, info.leftlegup_size.z);
	leftlegup.transform = trans*leftlegup.transform;
	btVector3 leftlegup_offset(info.leftlegup_offset.x, info.leftlegup_offset.y, info.leftlegup_offset.z);
	leftlegup_offset = leftlegup_offset.rotate(q.getAxis(), q.getAngle());

	leftlegup.transform.M[12] += leftlegup_offset.getX();
	leftlegup.transform.M[13] += leftlegup_offset.getY();
	leftlegup.transform.M[14] += leftlegup_offset.getZ();

	leftlegup.color = Blue;

	Cube rightlegdown(info.rightlegdown_size.x, info.rightlegdown_size.y, info.rightlegdown_size.z);
	rightlegdown.transform = trans*rightlegdown.transform;
	btVector3 rightlegdown_offset(info.rightlegdown_offset.x, info.rightlegdown_offset.y, info.rightlegdown_offset.z);
	rightlegdown_offset = rightlegdown_offset.rotate(q.getAxis(), q.getAngle());

	rightlegdown.transform.M[12] += rightlegdown_offset.getX();
	rightlegdown.transform.M[13] += rightlegdown_offset.getY();
	rightlegdown.transform.M[14] += rightlegdown_offset.getZ();

	rightlegdown.color.Set(1, .85, .7);

	Cube leftlegdown(info.leftlegdown_size.x, info.leftlegdown_size.y, info.leftlegdown_size.z);
	leftlegdown.transform = trans*leftlegdown.transform;
	btVector3 leftlegdown_offset(info.leftlegdown_offset.x, info.leftlegdown_offset.y, info.leftlegdown_offset.z);
	leftlegdown_offset = leftlegdown_offset.rotate(q.getAxis(), q.getAngle());

	leftlegdown.transform.M[12] += leftlegdown_offset.getX();
	leftlegdown.transform.M[13] += leftlegdown_offset.getY();
	leftlegdown.transform.M[14] += leftlegdown_offset.getZ();

	leftlegdown.color.Set(1, .85, .7);

	Sphere head(info.head_radius);
	head.transform = trans*head.transform;
	btVector3 head_offset(info.head_offset.x, info.head_offset.y, info.head_offset.z);
	head_offset = head_offset.rotate(q.getAxis(), q.getAngle());

	head.transform.M[12] += head_offset.getX();
	head.transform.M[13] += head_offset.getY();
	head.transform.M[14] += head_offset.getZ();

	head.color.Set(1,.85,.7);


	chassis.Render();
	saddle.Render();
	downchassis.Render();
	frontwheelsupport.Render();
	mudguard.Render();
	handelbarsupport.Render();
	handelbarpost.Render();
	panel.Render();
	handlebarright.Render();
	handlebarleft.Render();
	hip.Render();
	body.Render();
	leftarm.Render();
	rightarm.Render();
	rightlegup.Render();
	leftlegup.Render();
	rightlegdown.Render();
	leftlegdown.Render();
	head.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}