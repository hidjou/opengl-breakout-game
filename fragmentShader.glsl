#version 420 core

#define HEMISPHERE 0
#define BAT 1
#define BRICK 2
#define DISK 3

uniform uint object;

uniform vec4 hemColor, batColor, brickColor, diskColor;

uniform sampler2D canLabelTex;

in vec2 texCoordsExport;

out vec4 colorsOut;

void main(void)
{
	if (object == HEMISPHERE) colorsOut = hemColor;
   if (object == BAT) colorsOut = batColor;
   if (object == BRICK) colorsOut = brickColor;
   if (object == DISK) colorsOut = texture(canLabelTex, texCoordsExport);

}