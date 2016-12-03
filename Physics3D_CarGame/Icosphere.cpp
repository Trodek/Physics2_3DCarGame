#include "Icosphere.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glut/glut.h"

#pragma comment (lib, "glut/glut32.lib")

// -------------- TRIANGLE3D ---------------

Triangle3D::Triangle3D()
{
}

Triangle3D::Triangle3D(vec3 A, vec3 B, vec3 C) : A(A), B(B), C(C)
{
}

Triangle3D::~Triangle3D()
{
}

//---------------------------------

// -------------- ICOSPHERE ----------------

Icosphere::Icosphere(): transform(IdentityMatrix)
{
}

Icosphere::Icosphere(float size): transform(IdentityMatrix)
{
	CreateIcosphere(size, DEFAULT_RECURSION);
}

Icosphere::~Icosphere()
{
	// Release vertices data
	for (p2List_item<vec3*>* item = vertices.getFirst(); item; item = item->next) {
		delete item->data;
		item->data = nullptr;
	}
	vertices.clear();

	// Release triangles data
	for (p2List_item<Triangle3D*>* item = triangles.getFirst(); item; item = item->next) {
		delete item->data;
		item->data = nullptr;
	}
	triangles.clear();
}

void Icosphere::CreateIcosphere(float size, int recursion_iterations)
{
	CreateUnitIcosphere(recursion_iterations);

	ResizeIcosphere(size);
}

void Icosphere::CreateUnitIcosphere(int recursion_iterations)
{
	//Create the initial 12 vertices of a icosahedron
	float t = (1.0f + sqrtf(5.0f)) / 2.0f;

	AddVertex(vec3(-1, t, 0));
	AddVertex(vec3(1, t, 0));
	AddVertex(vec3(-1, -t, 0));
	AddVertex(vec3(1, -t, 0));

	AddVertex(vec3(0, -1, t));
	AddVertex(vec3(0, 1, t));
	AddVertex(vec3(0, -1, -t));
	AddVertex(vec3(0, 1, -t));

	AddVertex(vec3(t, 0, -1));
	AddVertex(vec3(t, 0, 1));
	AddVertex(vec3(-t, 0, -1));
	AddVertex(vec3(-t, 0, 1));

	// Create 20 initial triangles 
	
	AddTriangle(0, 11, 5);
	AddTriangle(0, 5, 1);
	AddTriangle(0, 1, 7);
	AddTriangle(0, 7, 10);
	AddTriangle(0, 10, 11);

	AddTriangle(1, 5, 9);
	AddTriangle(5, 11, 4);
	AddTriangle(11, 10, 2);
	AddTriangle(10, 7, 6);
	AddTriangle(7, 1, 8);

	AddTriangle(3, 9, 4);
	AddTriangle(3, 4, 2);
	AddTriangle(3, 2, 6);
	AddTriangle(3, 6, 8);
	AddTriangle(3, 8, 9);

	AddTriangle(4, 9, 5);
	AddTriangle(2, 4, 11);
	AddTriangle(6, 2, 10);
	AddTriangle(8, 6, 7);
	AddTriangle(9, 8, 1);

	///copy triangles to tmp and clear triangles. then refine icosphere
	for (int i = 0; i < recursion_iterations; i++) {
		p2List<Triangle3D*> trianglestmp; //store temporal triangles
		for (p2List_item<Triangle3D*>* triangle = triangles.getFirst(); triangle; triangle = triangle->next) {
			trianglestmp.add(triangle->data);
		}
		triangles.clear();

		//from each triangle, create 4 smaller ones
		for (p2List_item<Triangle3D*>* triangle = trianglestmp.getFirst(); triangle; triangle = triangle->next) {
			int a = GetMiddlePoint(triangle->data->A, triangle->data->B);
			int b = GetMiddlePoint(triangle->data->B, triangle->data->C);
			int c = GetMiddlePoint(triangle->data->C, triangle->data->A);

			triangles.add(new Triangle3D(triangle->data->A, *vertices[a], *vertices[c]));
			triangles.add(new Triangle3D(triangle->data->B, *vertices[b], *vertices[a]));
			triangles.add(new Triangle3D(triangle->data->C, *vertices[c], *vertices[b]));
			triangles.add(new Triangle3D(*vertices[a], *vertices[b], *vertices[c]));
		}
		trianglestmp.clear();

	}
}

void Icosphere::ResizeIcosphere(float size)
{
	// Multiply each vertice and triangle for the size
	for (p2List_item<vec3*>* item = vertices.getFirst(); item; item = item->next) {
		*(item->data) *= size;
	}
	for (p2List_item<Triangle3D*>* item = triangles.getFirst(); item; item = item->next) {
		item->data->A *= size;
		item->data->B *= size;
		item->data->C *= size;
	}
}

void Icosphere::SetPos(float x, float y, float z)
{
	for (p2List_item<Triangle3D*>* item = triangles.getFirst(); item; item = item->next) {
		item->data->A.x += x;
		item->data->A.y += y;
		item->data->A.z += z;

		item->data->B.x += x;
		item->data->B.y += y;
		item->data->B.z += z;

		item->data->C.x += x;
		item->data->C.y += y;
		item->data->C.z += z;

	}

	transform.translate(x, y, z);
}

void Icosphere::InnerDraw()
{
	glLineWidth(1.5);
	glColor3f(1.0, 0.0, 0.0);
	for (p2List_item<Triangle3D*>* item = triangles.getFirst(); item; item = item->next) {
		glBegin(GL_LINE_LOOP);
		glVertex3f(item->data->A.x, item->data->A.y, item->data->A.z);
		glVertex3f(item->data->B.x, item->data->B.y, item->data->B.z);
		glVertex3f(item->data->C.x, item->data->C.y, item->data->C.z);
		glEnd();
	}
}

// Add a vertex to a unit sphere
void Icosphere::AddVertex(vec3& point)
{
	float length = sqrtf(point.x*point.x + point.y*point.y + point.z*point.z);
	vertices.add(new vec3(point.x / length, point.y / length, point.z / length));
}

void Icosphere::AddTriangle(int index_a, int index_b, int index_c)
{
	vec3 *a, *b, *c;
	vertices.at(index_a, a);
	vertices.at(index_b, b);
	vertices.at(index_c, c);
	triangles.add(new Triangle3D(*a, *b, *c));
}

//Get the index in vertices list of the middlepoint between p1 and p2
int Icosphere::GetMiddlePoint(vec3 p1, vec3 p2)
{
	int ret = -1;

	vec3* midpoint = new vec3((p1 + p2) / 2);
	float length = sqrtf(midpoint->x*midpoint->x + midpoint->y*midpoint->y + midpoint->z*midpoint->z);
	vec3* unitmidpoint = new vec3(*midpoint / length);

	// If point already exist return the index
	if ((ret = vertices.find(unitmidpoint)) != -1) {
		delete midpoint;
		delete unitmidpoint;
		return ret;
	}

	//Add the point to vertices 
	AddVertex(*midpoint);
	ret = vertices.count() - 1;
	delete midpoint;
	delete unitmidpoint;
	return ret;
}


// --------------------------------