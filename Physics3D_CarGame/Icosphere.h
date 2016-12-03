#ifndef _ICOSPHERE_
#define _ICOSPHERE_

#include "glmath.h"
#include "p2List.h"

#define DEFAULT_RECURSION 2

// Each triangle have 3 points. Usually in mathematics they are named A, B and C.
// I will use this nomenclature too.
class Triangle3D { 
public:
	vec3 A, B, C;

public:
	// Constructors
	Triangle3D();
	Triangle3D(vec3 A, vec3 B, vec3 C);

	//Destructor
	~Triangle3D();
	
};

// An icosphere is a refined icosahedron (https://en.wikipedia.org/wiki/Icosahedron)
class Icosphere {
public:
	p2List<Triangle3D*> triangles;

public:
	// Constructor
	Icosphere();
	Icosphere(float size);

	//Destructor 
	~Icosphere();

	//Create the sphere
	void CreateIcosphere(float size, int recursion_iterations);
	void CreateUnitIcosphere(int recursion_iterations);
	void ResizeIcosphere(float size);

	void SetPos(float x, float y, float z);

	//Draw
	void InnerDraw();
	
private:
	p2List<vec3*> vertices;

private:

	//Auxiliar methods

	void AddVertex(vec3& point);
	void AddTriangle(int index_a, int index_b, int index_c);
	int GetMiddlePoint(vec3 p1, vec3 p2);

};




#endif // !_ICOSPHERE_

