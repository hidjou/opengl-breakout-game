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

#include "bat.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

// Fill the vertex array with co-ordinates of the sample points.
void fillTorVertexArray(Vertex batVertices[16])
{
	// Define a vector for the construction of the upper points of the bats
	vec4 upper_vector(0.0, 0.0, 2.0, 0.0);

	// Ground points
	for (int i = 0; i < 4; i++)
	{
		// inner wall
		batVertices[i].coords.x = BAT_INRAD * glm::cos(radians(BAT_ANGLE*i));
		batVertices[i].coords.y = BAT_INRAD * glm::sin(radians(BAT_ANGLE*i));
		batVertices[i].coords.z = 0;
		batVertices[i].coords.w = 1.0;

		// TODO - outer wall
		batVertices[i+4].coords.x = BAT_OUTRAD * glm::cos(radians(BAT_ANGLE*i));
		batVertices[i+4].coords.y = BAT_OUTRAD * glm::sin(radians(BAT_ANGLE*i));
		batVertices[i+4].coords.z = 0;
		batVertices[i+4].coords.w = 1.0;

	}

	// Upper points
	// inner wall
	batVertices[8].coords = batVertices[0].coords + upper_vector;
	batVertices[9].coords = batVertices[1].coords + upper_vector;
	batVertices[10].coords = batVertices[2].coords + upper_vector;
	batVertices[11].coords = batVertices[3].coords + upper_vector;

	// TODO - outer wall
	batVertices[12].coords = batVertices[4].coords + upper_vector;
	batVertices[13].coords = batVertices[5].coords + upper_vector;
	batVertices[14].coords = batVertices[6].coords + upper_vector;
	batVertices[15].coords = batVertices[7].coords + upper_vector;


 
	// Calculate the normals for the front face by taking the cross product of two edges of the triangle and normalising
	batVertices[0].normal = normalize(cross(vec3(batVertices[8].coords - batVertices[0].coords), vec3(batVertices[1].coords - batVertices[0].coords)));
	batVertices[1].normal = normalize(cross(vec3(batVertices[9].coords - batVertices[1].coords), vec3(batVertices[2].coords - batVertices[1].coords)));
	batVertices[2].normal = normalize(cross(vec3(batVertices[10].coords - batVertices[2].coords), vec3(batVertices[3].coords - batVertices[2].coords)));
	batVertices[3].normal = batVertices[2].normal; // Made same as above as on same face!!!

	batVertices[11].normal = normalize(cross(vec3(batVertices[3].coords - batVertices[11].coords), vec3(batVertices[1].coords - batVertices[10].coords)));
	batVertices[10].normal = normalize(cross(vec3(batVertices[2].coords - batVertices[10].coords), vec3(batVertices[9].coords - batVertices[10].coords)));
	batVertices[9].normal = normalize(cross(vec3(batVertices[1].coords - batVertices[9].coords), vec3(batVertices[8].coords - batVertices[9].coords)));
	batVertices[8].normal = batVertices[9].normal; // Made same as above as on same face!!!

}

// Fill the array of index arrays.
void fillTorIndices(unsigned int batIndices[6][8])
{
	// Front face 
	batIndices[0][0] = 8;
	batIndices[0][1] = 0;
	batIndices[0][2] = 9;
	batIndices[0][3] = 1;
	batIndices[0][4] = 10;
	batIndices[0][5] = 2;
	batIndices[0][6] = 11;
	batIndices[0][7] = 3;

	// Back face - TODO

	batIndices[1][0] = 12;
	batIndices[1][1] = 4;
	batIndices[1][2] = 13;
	batIndices[1][3] = 5;
	batIndices[1][4] = 14;
	batIndices[1][5] = 6;
	batIndices[1][6] = 15;
	batIndices[1][7] = 7;
	
	// Top face - TODO
	batIndices[2][0] = 11;
	batIndices[2][1] = 15;
	batIndices[2][2] = 10;
	batIndices[2][3] = 14;
	batIndices[2][4] = 9;
	batIndices[2][5] = 13;
	batIndices[2][6] = 8;
	batIndices[2][7] = 12;
	


	// Bottom face - TODO
	batIndices[3][0] = 4;
	batIndices[3][1] = 0;
	batIndices[3][2] = 5;
	batIndices[3][3] = 1;
	batIndices[3][4] = 6;
	batIndices[3][5] = 2;
	batIndices[3][6] = 7;
	batIndices[3][7] = 3;


	// Right end face - TODO
	batIndices[4][0] = 11;
	batIndices[4][1] = 15;
	batIndices[4][2] = 3;
	batIndices[4][3] = 7;

	// Left end face - TODO
	batIndices[5][0] = 8;
	batIndices[5][1] = 12;
	batIndices[5][2] = 0;
	batIndices[5][3] = 4;
}

// Fill the array of counts.
void fillTorCounts(int batCounts[6])
{
	batCounts[0] = 8;
	batCounts[1] = 8;
	batCounts[2] = 8;
	batCounts[3] = 8;
	batCounts[4] = 4;
	batCounts[5] = 4;
}

// Fill the array of buffer offsets.
void fillTorOffsets(void* batOffsets[6])
{
	batOffsets[0] = 0;
	batOffsets[1] = (GLvoid*)(8 * 1 * sizeof(unsigned int));
	batOffsets[2] = (GLvoid*)(8 * 2 * sizeof(unsigned int));
	batOffsets[3] = (GLvoid*)(8 * 3 * sizeof(unsigned int));
	batOffsets[4] = (GLvoid*)(8 * 4 * sizeof(unsigned int));
	batOffsets[5] = (GLvoid*)(8 * 5 * sizeof(unsigned int));
}

// Initialize the bat.
void fillBat(Vertex batVertices[BAT_VERTEX_COUNT],
	unsigned int batIndices[BAT_FACES][BAT_VERTICES_PER_FACE],
	int batCounts[BAT_FACES],
	void* batOffsets[BAT_FACES])
{
	fillTorVertexArray(batVertices);
	fillTorIndices(batIndices);
	fillTorCounts(batCounts);
	fillTorOffsets(batOffsets);
}