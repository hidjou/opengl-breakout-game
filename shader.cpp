#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

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

using namespace std;

// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");	
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char)); 
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}


// Function to initialize shaders.
int setShader(char* shaderType, char* shaderFile)
{
   int shaderId;
   char* shader = readTextFile(shaderFile);
   
   if (shaderType == "vertex") shaderId = glCreateShader(GL_VERTEX_SHADER); 
   if (shaderType == "tessControl") shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);    
   if (shaderType == "tessEvaluation") shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER); 
   if (shaderType == "geometry") shaderId = glCreateShader(GL_GEOMETRY_SHADER); 
   if (shaderType == "fragment") shaderId = glCreateShader(GL_FRAGMENT_SHADER); 

   glShaderSource(shaderId, 1, (const char**) &shader, NULL); 
   glCompileShader(shaderId); 

   // Test for shader compilation errors
   std::cout << "SHADER" << std::endl;
   shaderCompileTest(shaderId);

   return shaderId;
}

