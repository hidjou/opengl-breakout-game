#include <cmath>
#include <iostream>

#ifdef __APPLE__

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include "Disk.h"





using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillDiscVertexArray(Vertex discVertices[DISK_SEGS])
{
	float angle = 360.0 / DISK_SEGS;
	int k;

	for (k = 0; k < DISK_SEGS; k++)
	{
		discVertices[k].coords.x = 30* cos((-1 + 2 * (float)k / DISK_SEGS) * PI);
		discVertices[k].coords.y = 30* sin((-1 + 2 * (float)k / DISK_SEGS) * PI);
		discVertices[k].coords.z = 1.0;
		discVertices[k].coords.w = 1.0;
		discVertices[k].normal.x = 0.0;
		discVertices[k].normal.y = 0.0;
		discVertices[k].normal.z = 1.0;
		discVertices[k].texCoords.s = (cos(glm::radians(angle * k)) + 1.0) * 0.5;
		discVertices[k].texCoords.t = (sin(glm::radians(angle * k)) + 1.0)  * 0.5;

	}
}