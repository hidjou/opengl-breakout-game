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

#include "Brick.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

// Fill the vertex array with co-ordinates of the sample points.
void fillBrickVertexArray(Vertex brickVertices[16])
{
	// Define a vector for the construction of the upper points of the bats
	vec4 upper_vector(0.0, 0.0, 2.0, 0.0);

	// Ground points
	for (int i = 0; i < 4; i++)
	{
		// inner wall
		brickVertices[i].coords.x = BRICK_INRAD * glm::cos(radians(BRICK_ANGLE*i));
		brickVertices[i].coords.y = BRICK_INRAD * glm::sin(radians(BRICK_ANGLE*i));
		brickVertices[i].coords.z = 0;
		brickVertices[i].coords.w = 1.0;

		// TODO - outer wall
		brickVertices[i + 4].coords.x = BRICK_OUTRAD * glm::cos(radians(BRICK_ANGLE*i));
		brickVertices[i + 4].coords.y = BRICK_OUTRAD * glm::sin(radians(BRICK_ANGLE*i));
		brickVertices[i + 4].coords.z = 0;
		brickVertices[i + 4].coords.w = 1.0;

	}

	// Upper points
	// inner wall
	brickVertices[8].coords = brickVertices[0].coords + upper_vector;
	brickVertices[9].coords = brickVertices[1].coords + upper_vector;
	brickVertices[10].coords = brickVertices[2].coords + upper_vector;
	brickVertices[11].coords = brickVertices[3].coords + upper_vector;

	// TODO - outer wall
	brickVertices[12].coords = brickVertices[4].coords + upper_vector;
	brickVertices[13].coords = brickVertices[5].coords + upper_vector;
	brickVertices[14].coords = brickVertices[6].coords + upper_vector;
	brickVertices[15].coords = brickVertices[7].coords + upper_vector;



	// Calculate the normals for the front face by taking the cross product of two edges of the triangle and normalising
	brickVertices[0].normal = normalize(cross(vec3(brickVertices[8].coords - brickVertices[0].coords), vec3(brickVertices[1].coords - brickVertices[0].coords)));
	brickVertices[1].normal = normalize(cross(vec3(brickVertices[9].coords - brickVertices[1].coords), vec3(brickVertices[2].coords - brickVertices[1].coords)));
	brickVertices[2].normal = normalize(cross(vec3(brickVertices[10].coords - brickVertices[2].coords), vec3(brickVertices[3].coords - brickVertices[2].coords)));
	brickVertices[3].normal = brickVertices[2].normal; // Made same as above as on same face!!!

	brickVertices[11].normal = normalize(cross(vec3(brickVertices[3].coords - brickVertices[11].coords), vec3(brickVertices[1].coords - brickVertices[10].coords)));
	brickVertices[10].normal = normalize(cross(vec3(brickVertices[2].coords - brickVertices[10].coords), vec3(brickVertices[9].coords - brickVertices[10].coords)));
	brickVertices[9].normal = normalize(cross(vec3(brickVertices[1].coords - brickVertices[9].coords), vec3(brickVertices[8].coords - brickVertices[9].coords)));
	brickVertices[8].normal = brickVertices[9].normal; // Made same as above as on same face!!!

}

// Fill the array of index arrays.
void fillBrickIndices(unsigned int brickIndices[6][8])
{
	// Front face 
	brickIndices[0][0] = 8;
	brickIndices[0][1] = 0;
	brickIndices[0][2] = 9;
	brickIndices[0][3] = 1;
	brickIndices[0][4] = 10;
	brickIndices[0][5] = 2;
	brickIndices[0][6] = 11;
	brickIndices[0][7] = 3;

	// Back face - TODO

	brickIndices[1][0] = 12;
	brickIndices[1][1] = 4;
	brickIndices[1][2] = 13;
	brickIndices[1][3] = 5;
	brickIndices[1][4] = 14;
	brickIndices[1][5] = 6;
	brickIndices[1][6] = 15;
	brickIndices[1][7] = 7;

	// Top face - TODO
	brickIndices[2][0] = 11;
	brickIndices[2][1] = 15;
	brickIndices[2][2] = 10;
	brickIndices[2][3] = 14;
	brickIndices[2][4] = 9;
	brickIndices[2][5] = 13;
	brickIndices[2][6] = 8;
	brickIndices[2][7] = 12;



	// Bottom face - TODO
	brickIndices[3][0] = 4;
	brickIndices[3][1] = 0;
	brickIndices[3][2] = 5;
	brickIndices[3][3] = 1;
	brickIndices[3][4] = 6;
	brickIndices[3][5] = 2;
	brickIndices[3][6] = 7;
	brickIndices[3][7] = 3;


	// Right end face - TODO
	brickIndices[4][0] = 11;
	brickIndices[4][1] = 15;
	brickIndices[4][2] = 3;
	brickIndices[4][3] = 7;

	// Left end face - TODO
	brickIndices[5][0] = 8;
	brickIndices[5][1] = 12;
	brickIndices[5][2] = 0;
	brickIndices[5][3] = 4;
}

// Fill the array of counts.
void fillBrickCounts(int brickCounts[6])
{
	brickCounts[0] = 8;
	brickCounts[1] = 8;
	brickCounts[2] = 8;
	brickCounts[3] = 8;
	brickCounts[4] = 4;
	brickCounts[5] = 4;
}

// Fill the array of buffer offsets.
void fillBrickOffsets(void* brickOffsets[6])
{
	brickOffsets[0] = 0;
	brickOffsets[1] = (GLvoid*)(8 * 1 * sizeof(unsigned int));
	brickOffsets[2] = (GLvoid*)(8 * 2 * sizeof(unsigned int));
	brickOffsets[3] = (GLvoid*)(8 * 3 * sizeof(unsigned int));
	brickOffsets[4] = (GLvoid*)(8 * 4 * sizeof(unsigned int));
	brickOffsets[5] = (GLvoid*)(8 * 5 * sizeof(unsigned int));
}

// Initialize the bat.
void fillBrick(Vertex brickVertices[BRICK_VERTEX_COUNT],
	unsigned int brickIndices[BRICK_FACES][BRICK_VERTICES_PER_FACE],
	int brickCounts[BRICK_FACES],
	void* brickOffsets[BRICK_FACES])
{
	fillBrickVertexArray(brickVertices);
	fillBrickIndices(brickIndices);
	fillBrickCounts(brickCounts);
	fillBrickOffsets(brickOffsets);
}