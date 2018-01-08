#ifndef BAT_H
#define BAT_H

#include "vertex.h"

#define PI 3.14159265
#define BAT_OUTRAD 26.0 // Bat outer radius.
#define BAT_INRAD 23.0 // Bat inner radius.
#define BAT_ANGLE 10.0 // Angle between each segment of bat.

#define BAT_COLORS 0.0, 1.0, 0.0, 1.0 // Bat colors.

// TODO: Need to update BAT_FACES to 6 when other faces complete
#define BAT_FACES 6 
#define BAT_VERTEX_COUNT 16
#define BAT_VERTICES_PER_FACE 8

void fillTorVertexArray(Vertex batVertices[BAT_VERTEX_COUNT]);
void fillTorIndices(unsigned int batIndices[BAT_FACES][BAT_VERTICES_PER_FACE]);
void fillTorCounts(int batCounts[BAT_FACES]);
void fillTorOffsets(void* batOffsets[BAT_FACES]);

void fillBat(Vertex batVertices[BAT_VERTEX_COUNT],
	unsigned int batIndices[BAT_FACES][BAT_VERTICES_PER_FACE],
	int batCounts[BAT_FACES],
	void* batOffsets[BAT_FACES]);
#endif
