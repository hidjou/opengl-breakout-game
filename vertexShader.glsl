#version 420 core

#define HEMISPHERE 0
#define BAT 1
#define BRICK 2
#define DISK 3

layout(location=0) in vec4 hemCoords;
layout(location=1) in vec4 batCoords;
layout(location=2) in mat4 batTransformMats; // Because of type mat4 it will 
												// occupy locations 2, 3, 4, 5.

layout(location=6) in vec4 brickCoords;
layout(location=7) in mat4 brickTransformMats; // 7,8,9,10
layout(location = 11) in vec4 diskCoords;
layout(location = 12) in vec2 discTexCoords;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform uint object;

vec4 coords;
out vec2 texCoordsExport;

void main(void)
{
	if (object == HEMISPHERE)
	{
		coords = hemCoords;
		gl_Position = projMat * modelViewMat * coords;
	}
	if (object == BAT)
	{
		coords = batCoords;
		gl_Position = projMat * modelViewMat * batTransformMats * coords;
	}
	if (object == BRICK)
	{
		coords = brickCoords;
		gl_Position = projMat * modelViewMat * brickTransformMats * coords;
	}
	if (object == DISK)
	{
		coords = diskCoords;
		texCoordsExport = discTexCoords;
		gl_Position = projMat * modelViewMat * coords;
		
	}


}
