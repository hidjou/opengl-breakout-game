#ifndef BRICK_H
#define BRICK_H

#include "vertex.h"

#define PI 3.14159265
#define BRICK_OUTRAD 10.0 // Bat outer radius.
#define BRICK_INRAD 6.0 // Bat inner radius.
#define BRICK_ANGLE 20.0 // Angle between each segment of bat.

#define BRICK_COLORS 1.0, 0.0, 0.0, 1.0 // red brick

// TODO: Need to update BAT_FACES to 6 when other faces complete
#define BRICK_FACES 6 
#define BRICK_VERTEX_COUNT 16
#define BRICK_VERTICES_PER_FACE 8

void fillBrickVertexArray(Vertex brickVertices[BRICK_VERTEX_COUNT]);
void fillBrickIndices(unsigned int brickIndices[BRICK_FACES][BRICK_VERTICES_PER_FACE]);
void fillBrickCounts(int brickCounts[BRICK_FACES]);
void fillBrickOffsets(void* brickOffsets[BRICK_FACES]);

void fillBrick(Vertex brickVertices[BRICK_VERTEX_COUNT],
	unsigned int brickIndices[BRICK_FACES][BRICK_VERTICES_PER_FACE],
	int brickCounts[BRICK_FACES],
	void* brickOffsets[BRICK_FACES]);

#endif
