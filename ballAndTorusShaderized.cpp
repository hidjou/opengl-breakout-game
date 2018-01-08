////////////////////////////////////////////////////////////////          
// ballAndBatShaderized.cpp
//
// 
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <fstream>

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "hemisphere.h"
#include "bat.h"
#include "material.h"
#include "light.h"
#include "brick.h"
#include "Disk.h"
#include "getbmp.h"

using namespace std;
using namespace glm;

static enum object {HEMISPHERE, BAT, BRICK, DISK}; // VAO ids.
static enum buffer { HEM_VERTICES, HEM_INDICES, BAT_VERTICES, BAT_INDICES, BRICK_VERTICES, BRICK_INDICES, BAT_TRANSFORM_MATS, BRICK_TRANSFORM_MATS, DISK_VERTICES }; // VBO ids.

// Globals.
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0, moveme = 1.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 33; // Time interval between frames.

// Hemisphere data.
static Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)]; 
static unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)]; 
static int hemCounts[HEM_LATS];
static void* hemOffsets[HEM_LATS]; 
static vec4 hemColors = vec4(HEM_COLORS); 

// Bat data.
static Vertex batVertices[BAT_VERTEX_COUNT];
static unsigned int batIndices[BAT_FACES][BAT_VERTICES_PER_FACE];
static int batCounts[BAT_FACES];
static void* batOffsets[BAT_FACES];
static vec4 batColors = vec4(BAT_COLORS);

// Brick data.
static Vertex brickVertices[BRICK_VERTEX_COUNT];
static unsigned int brickIndices[BRICK_FACES][BRICK_VERTICES_PER_FACE];
static int brickCounts[BRICK_FACES];
static void* brickOffsets[BRICK_FACES];
static vec4 brickColors = vec4(BRICK_COLORS);

// Disk data
static Vertex diskVertices[DISK_SEGS];
//static vec4 diskColors = vec4(DISK_COLORS);


static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

static mat4 batTransformMats[5];
static mat4 brickTransformMats[5];

// Ball translation coords
float ballX = 1.0;
float ballY = 1.0;
bool moveBall = 0;


static unsigned int texture[1];
static BitMapFile *image[1]; // Local storage for bmp image data.




static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
objectLoc,
hemColorLoc,
batColorLoc,
diskTexLoc,
brickColorLoc,//brick add
diskColorLoc,
buffer[13],
vao[4];


void fillHelTransformMats(void)
{
	batTransformMats[0] = mat4(1.0);

	batTransformMats[1] = mat4(1.0);
	batTransformMats[1] = rotate(batTransformMats[1], radians(72.0f), vec3(0.0, 0.0, 1.0));

	batTransformMats[2] = mat4(1.0);
	batTransformMats[2] = rotate(batTransformMats[2], radians(144.0f), vec3(0.0, 0.0, 1.0));

	batTransformMats[3] = mat4(1.0);
	batTransformMats[3] = rotate(batTransformMats[3], radians(216.0f), vec3(0.0, 0.0, 1.0));

	batTransformMats[4] = mat4(1.0);
	batTransformMats[4] = rotate(batTransformMats[4], radians(288.0f), vec3(0.0, 0.0, 1.0));

}

void fillBrickTransformMats(void)
{
	brickTransformMats[0] = mat4(1.0);

	brickTransformMats[1] = mat4(1.0);
	brickTransformMats[1] = rotate(brickTransformMats[1], radians(72.0f), vec3(0.0, 0.0, 1.0));

	brickTransformMats[2] = mat4(1.0);
	brickTransformMats[2] = rotate(brickTransformMats[2], radians(144.0f), vec3(0.0, 0.0, 1.0));

	brickTransformMats[3] = mat4(1.0);
	brickTransformMats[3] = rotate(brickTransformMats[3], radians(216.0f), vec3(0.0, 0.0, 1.0));

	brickTransformMats[4] = mat4(1.0);
	brickTransformMats[4] = rotate(brickTransformMats[4], radians(288.0f), vec3(0.0, 0.0, 1.0));

}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId);    
   glLinkProgram(programId); 
   glUseProgram(programId); 

   // Initialize hemishpere and bat.
   fillHemisphere(hemVertices, hemIndices, hemCounts, hemOffsets);
   fillBat(batVertices, batIndices, batCounts, batOffsets);
   fillHelTransformMats();
   fillBrick(brickVertices, brickIndices, brickCounts, brickOffsets); //brick add
   fillBrickTransformMats();
   fillDiscVertexArray(diskVertices);
   

   // Create VAOs and VBOs... 
   glGenVertexArrays(4, vao);
   glGenBuffers(13, buffer); 

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[HEMISPHERE]);  
   glBindBuffer(GL_ARRAY_BUFFER, buffer[HEM_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(hemVertices), hemVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[HEM_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hemIndices), hemIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), 0);
   glEnableVertexAttribArray(0);

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[BAT]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[BAT_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(batVertices), batVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[BAT_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(batIndices), batIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(batVertices[0]), 0);
   glEnableVertexAttribArray(1);

   // ...and associate data with vertex shader.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[BAT_TRANSFORM_MATS]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(batTransformMats), batTransformMats, GL_STATIC_DRAW);
   for (int i = 0; i < 4; i++)
   {
	   glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(vec4) * i)); // gonna take locations 2 through 5
	   glEnableVertexAttribArray(2 + i);
	   glVertexAttribDivisor(2 + i, 1); // Set attribute instancing.
   }

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[BRICK]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[BRICK_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(brickVertices), brickVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[BRICK_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(brickIndices), brickIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(brickVertices[0]), 0);
   glEnableVertexAttribArray(6);

   // ...and associate data with vertex shader.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[BRICK_TRANSFORM_MATS]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(brickTransformMats), brickTransformMats, GL_STATIC_DRAW);
   for (int i = 0; i < 4; i++)
   {
	   glVertexAttribPointer(7 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(vec4) * i)); // gonna take locations 2 through 5
	   glEnableVertexAttribArray(7 + i);
	   glVertexAttribDivisor(7 + i, 1); // Set attribute instancing.
   }

   // ...and associate data with vertex shader.
   glBindVertexArray(vao[DISK]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[DISK_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(diskVertices), diskVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(diskVertices[0]), 0);
   glEnableVertexAttribArray(11);

   // Load the images.
   image[0] = getbmp("diskTex.bmp");

   // Create texture ids.
   glGenTextures(1, texture);

   // Bind can label image.
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
	   GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   diskTexLoc = glGetUniformLocation(programId, "canLabelTex");
   glUniform1i(diskTexLoc, 0);





   
   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat"); 
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); 
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   // Obtain color uniform locations and set values.
   hemColorLoc = glGetUniformLocation(programId, "hemColor");
   glUniform4fv(hemColorLoc, 1, &hemColors[0]);
   batColorLoc = glGetUniformLocation(programId, "batColor");
   glUniform4fv(batColorLoc, 1, &batColors[0]);
   brickColorLoc = glGetUniformLocation(programId, "brickColor");
   glUniform4fv(brickColorLoc, 1, &brickColors[0]);
   /*diskColorLoc = glGetUniformLocation(programId, "diskColor");
   glUniform4fv(diskColorLoc, 1, &diskColors[0]);*/


   // Obtain modelview matrix uniform and object uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat"); 
   objectLoc = glGetUniformLocation(programId, "object");

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   //modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -25.0));
   modelViewMat = translate(modelViewMat, vec3(0.0, -20.0, -50.0));
   modelViewMat = rotate(modelViewMat, radians(Zangle), vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, radians(Yangle), vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, radians(Xangle), vec3(1.0, 0.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); 



   //push
   mat4 baseModelViewMat = modelViewMat;
   modelViewMat = rotate(modelViewMat, radians(moveme), vec3(0.0, 0.0, 1.0));
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, 0.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   //push

   // Draw bat.
   glUniform1ui(objectLoc, BAT); // Update object name.
   glBindVertexArray(vao[BAT]);
   for (int i = 0; i < 6; i++)
	   glDrawElementsInstanced(GL_TRIANGLE_STRIP, batCounts[i], GL_UNSIGNED_INT, batOffsets[i], 5);

   //pop
   modelViewMat = baseModelViewMat;
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, 23.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   //pop



   //mat4 baseModelViewMat = modelViewMat; // push

   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -25.0)); // TODO: move bats using a key // same as the one that rotate the whole scene, use the ball as reference

   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
  

   // Draw brick.
   glUniform1ui(objectLoc, BRICK); // Update object name.
   glBindVertexArray(vao[BRICK]);
   for (int i = 0; i < 6; i++)
	   glDrawElementsInstanced(GL_TRIANGLE_STRIP, brickCounts[i], GL_UNSIGNED_INT, brickOffsets[i], 5);

   // Draw Disk
  
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   glUniform1ui(objectLoc, DISK); // Update object name.
   glBindVertexArray(vao[DISK]);
   glDrawArrays(GL_TRIANGLE_FAN, 0, DISK_SEGS);

   modelViewMat = baseModelViewMat;
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));


   modelViewMat = baseModelViewMat; // pop


   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   //vic added
   
   if (sqrt((ballX * ballX) + (ballY * ballY)) > 30)
   {
	   ballX = 0;
	   ballY = 0;
   }

 
   modelViewMat = translate(modelViewMat, vec3(ballX, ballY, 0.0)); // TODO: move bats using a key // same as the one that rotate the whole scene, use the ball as reference
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   //vic added

   // Draw ball as two hemispheres.
   glUniform1ui(objectLoc, HEMISPHERE); // Update object name.
   glBindVertexArray(vao[HEMISPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);
   modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0)); // Scale to make inverted hemisphere.
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); // Update modelview matrix.
   glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);

   //// Draw Disk
   //glUniform1ui(objectLoc, DISK); // Update object name.
   //glBindVertexArray(vao[DISK]);
   //glDrawArrays(GL_TRIANGLE_FAN, 0, DISK_SEGS);

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
	  moveBall = 1;
	  if (moveBall)
	  {
		  ballX += 0.1;
		  ballY += 0.1;
		  cout << "ballx = " << ballX << endl;
	  }
	  
      latAngle += 5.0;
	  if (latAngle > 360.0) latAngle -= 360.0;
      longAngle += 1.0;
	  if (longAngle > 360.0) longAngle -= 360.0;

	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else 
		 {
	        isAnimate = 1; 
			animate(1);
			
		 }
		 break;

      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         moveme += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         moveme -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 2);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("ballAndBatShaderized.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}

