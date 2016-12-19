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

	wheel.color = Blue;

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

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.color = Blue;

	Cube saddle(info.saddle_size.x, info.saddle_size.y, info.saddle_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&saddle.transform);
	btVector3 saddle_offset(info.saddle_offset.x, info.saddle_offset.y, info.saddle_offset.z);
	saddle_offset = saddle_offset.rotate(q.getAxis(), q.getAngle());

	saddle.transform.M[12] += saddle_offset.getX();
	saddle.transform.M[13] += saddle_offset.getY();
	saddle.transform.M[14] += saddle_offset.getZ();

	saddle.color = Black;

	Cube downchassis(info.downchassis_size.x, info.downchassis_size.y, info.downchassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&downchassis.transform);
	btVector3 downchassis_offset(info.downchassis_offset.x, info.downchassis_offset.y, info.downchassis_offset.z);
	downchassis_offset = downchassis_offset.rotate(q.getAxis(), q.getAngle());

	downchassis.transform.M[12] += downchassis_offset.getX();
	downchassis.transform.M[13] += downchassis_offset.getY();
	downchassis.transform.M[14] += downchassis_offset.getZ();

	downchassis.color = Blue;

	Cube frontwheelsupport(info.frontwheelsupport_size.x, info.frontwheelsupport_size.y, info.frontwheelsupport_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontwheelsupport.transform);
	btVector3 frontwheelsupport_offset(info.frontwheelsupport_offset.x, info.frontwheelsupport_offset.y, info.frontwheelsupport_offset.z);
	frontwheelsupport_offset = frontwheelsupport_offset.rotate(q.getAxis(), q.getAngle());

	frontwheelsupport.transform.M[12] += frontwheelsupport_offset.getX();
	frontwheelsupport.transform.M[13] += frontwheelsupport_offset.getY();
	frontwheelsupport.transform.M[14] += frontwheelsupport_offset.getZ();

	frontwheelsupport.color = Blue;

	Cube mudguard(info.mudguard_size.x, info.mudguard_size.y, info.mudguard_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&mudguard.transform);
	btVector3 mudguard_offset(info.mudguard_offset.x, info.mudguard_offset.y, info.mudguard_offset.z);
	mudguard_offset = mudguard_offset.rotate(q.getAxis(), q.getAngle());

	mudguard.transform.M[12] += mudguard_offset.getX();
	mudguard.transform.M[13] += mudguard_offset.getY();
	mudguard.transform.M[14] += mudguard_offset.getZ();

	mudguard.color = Blue;

	Cube handelbarsupport(info.handelbarsupport_size.x, info.handelbarsupport_size.y, info.handelbarsupport_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&handelbarsupport.transform);
	btVector3 handelbarsupport_offset(info.handelbarsupport_offset.x, info.handelbarsupport_offset.y, info.handelbarsupport_offset.z);
	handelbarsupport_offset = handelbarsupport_offset.rotate(q.getAxis(), q.getAngle());

	handelbarsupport.transform.M[12] += handelbarsupport_offset.getX();
	handelbarsupport.transform.M[13] += handelbarsupport_offset.getY();
	handelbarsupport.transform.M[14] += handelbarsupport_offset.getZ();

	handelbarsupport.color = Red;

	chassis.Render();
	saddle.Render();
	downchassis.Render();
	frontwheelsupport.Render();
	mudguard.Render();
	handelbarsupport.Render();
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