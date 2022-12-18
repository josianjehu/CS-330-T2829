
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int width = 800;
const unsigned int height = 800;

glm::vec3 cameraPos = glm::vec3(-2.0f, 1.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//glm::mat4 view = glm::mat4(1.0f);
glm::mat4 view;
glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float cameraSpeed = 0.05f;

//lightning
glm::vec3 wallPos(1.0f, 0.6f, 0.0f);
glm::vec3 cubePos(1.0f, 0.6f, 0.0f);
glm::vec3 cubePos2(2.0f, 0.6f, 0.0f);
glm::vec3 cubePos3(0.0f, 0.6f, 0.0f);
glm::vec3 floorPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos(2.5f, 2.0f, 0.0f);
glm::vec3 lightPos2(-1.0f, 2.0f, 0.0f);
glm::vec3 lightPos3(-1.0f, 1.8f, 0.0f);


GLfloat vertices[] =
{ //coordinates and colors
	//are colors affecting indices rendering?
	-1.0f,-0.75f,-1.0f,    //top left
	1.0f,0.0f,0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f,-0.75f,-1.0f,    //top right
	0.0f,1.0f,0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f,-0.75f,1.0f,     //bottom right
	0.0f,0.0f,1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	-1.0f,-0.75f,-1.0f,    //top left
	1.0f,0.0f,0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f,-0.75f,1.0f,     //bottom right
	0.0f,0.0f,1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	-1.0f,-0.75f,1.0f,   //bottom left
	1.0f,0.0f,1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,


};
GLuint indices[] =
{
	0,1,2,
	3,4,5
};



GLfloat verticesCube[] = {
		-0.5f, -0.5f, -0.5f,    
		 0.5f, -0.5f, -0.5f,    
		 0.5f,  0.5f, -0.5f,    
		 0.5f,  0.5f, -0.5f,    
		-0.5f,  0.5f, -0.5f,   
		-0.5f, -0.5f, -0.5f,   

		-0.5f, -0.5f,  0.5f,   
		 0.5f, -0.5f,  0.5f,    
		 0.5f,  0.5f,  0.5f,    
		 0.5f,  0.5f,  0.5f,   
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,     

		-0.5f,  0.5f,  0.5f,   
		-0.5f,  0.5f, -0.5f,   
		-0.5f, -0.5f, -0.5f,    
		-0.5f, -0.5f, -0.5f,    
		-0.5f, -0.5f,  0.5f,    
		-0.5f,  0.5f,  0.5f,    

		 0.5f,  0.5f,  0.5f,    
		 0.5f,  0.5f, -0.5f,     
		 0.5f, -0.5f, -0.5f,     
		 0.5f, -0.5f, -0.5f,    
		 0.5f, -0.5f,  0.5f,     
		 0.5f,  0.5f,  0.5f,    

		-0.5f, -0.5f, -0.5f,    
		 0.5f, -0.5f, -0.5f,     
		 0.5f, -0.5f,  0.5f,     
		 0.5f, -0.5f,  0.5f,     
		-0.5f, -0.5f,  0.5f,     
		-0.5f, -0.5f, -0.5f,     

		-0.5f,  0.5f, -0.5f,     
		 0.5f,  0.5f, -0.5f,     
		 0.5f,  0.5f,  0.5f,     
		 0.5f,  0.5f,  0.5f,     
		-0.5f,  0.5f,  0.5f,     
		-0.5f,  0.5f, -0.5f,     
};

GLuint pyramidIndices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};


GLfloat pyramid[] =
{
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

float TEXTURE = 5.0f;

GLfloat vertices2[] =
{
//Positions          //Normals
// --------------------------------------
//Back Face          //Negative Z Normals
-3.5f, -0.1f, -3.5f,      0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
3.5f, -0.1f, -3.5f,       0.0f,  0.0f, -1.0f,     0.0f, 1.0f, 0.0f,     TEXTURE, 0.0f,
3.5f,  0.1f, -3.5f,       0.0f,  0.0f, -1.0f,     0.0f, 0.0f, 1.0f,     TEXTURE, TEXTURE,
3.5f,  0.1f, -3.5f,       0.0f,  0.0f, -1.0f,     1.0f, 1.0f, 0.0f,     TEXTURE, TEXTURE,
-3.5f,  0.1f, -3.5f,       0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 1.0f,     0.0f, TEXTURE,
-3.5f, -0.1f, -3.5f,       0.0f,  0.0f, -1.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f,

//Front Face         //Positive Z Normals
-3.5f, -0.1f,  3.5f,       0.0f,  0.0f,  1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
3.5f, -0.1f,  3.5f,       0.0f,  0.0f,  1.0f,     0.0f, 1.0f, 0.0f,     TEXTURE, 0.0f,
3.5f,  0.1f,  3.5f,       0.0f,  0.0f,  1.0f,     0.0f, 0.0f, 1.0f,     TEXTURE, TEXTURE,
3.5f,  0.1f,  3.5f,       0.0f,  0.0f,  1.0f,     1.0f, 1.0f, 0.0f,     TEXTURE, TEXTURE,
-3.5f,  0.1f,  3.5f,       0.0f,  0.0f,  1.0f,     1.0f, 0.0f, 1.0f,     0.0f, TEXTURE,
-3.5f, -0.1f,  3.5f,       0.0f,  0.0f,  1.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f,

//Left Face          //Negative X Normals
-3.5f,  0.1f,  3.5f,      -1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 0.0f,     0.0f, TEXTURE,
-3.5f,  0.1f, -3.5f,      -1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 0.0f,     0.0f, TEXTURE,
-3.5f, -0.1f, -3.5f,      -1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
-3.5f, -0.1f, -3.5f,      -1.0f,  0.0f,  0.0f,     1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
-3.5f, -0.1f,  3.5f,      -1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f,
-3.5f,  0.1f,  3.5f,      -1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 1.0f,     0.0f, TEXTURE,

//Right Face         //Positive X Normals
3.5f,  0.1f,  3.5f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 0.0f,     TEXTURE, TEXTURE,
3.5f,  0.1f, -3.5f,       1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 0.0f,     TEXTURE, TEXTURE,
3.5f, -0.1f, -3.5f,       1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,     TEXTURE, 0.0f,
3.5f, -0.1f, -3.5f,       1.0f,  0.0f,  0.0f,     1.0f, 1.0f, 0.0f,     TEXTURE, 0.0f,
3.5f, -0.1f,  3.5f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 1.0f,     TEXTURE, 0.0f,
3.5f,  0.1f,  3.5f,       1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 1.0f,     TEXTURE, TEXTURE,

//Bottom Face        //Negative Y Normals
-3.5f, -0.1f, -3.5f,       0.0f, -1.0f,  0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
3.5f, -0.1f, -3.5f,       0.0f, -1.0f,  0.0f,     0.0f, 1.0f, 0.0f,     TEXTURE, 0.0f,
3.5f, -0.1f,  3.5f,       0.0f, -1.0f,  0.0f,     0.0f, 0.0f, 1.0f,     TEXTURE, 0.0f,
3.5f, -0.1f,  3.5f,       0.0f, -1.0f,  0.0f,     1.0f, 1.0f, 0.0f,     TEXTURE, 0.0f,
-3.5f, -0.1f,  3.5f,       0.0f, -1.0f,  0.0f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f,
-3.5f, -0.1f, -3.5f,       0.0f, -1.0f,  0.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f,

//Top Face           //Positive Y Normals
-3.5f,  0.1f, -3.5f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f, 0.0f,     0.0f, TEXTURE, //back left
3.5f,  0.1f, -3.5f,       0.0f,  1.0f,  0.0f,     0.0f, 1.0f, 0.0f,     TEXTURE, TEXTURE, //back right
3.5f,  0.1f,  3.5f,       0.0f,  1.0f,  0.0f,     0.0f, 0.0f, 1.0f,     TEXTURE, TEXTURE, //front right

3.5f,  0.1f,  3.5f,       0.0f,  1.0f,  0.0f,     1.0f, 1.0f, 0.0f,     TEXTURE, TEXTURE, //front right
-3.5f,  0.1f,  3.5f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f, 1.0f,     0.0f, TEXTURE, //front left
-3.5f,  0.1f, -3.5f,       0.0f,  1.0f,  0.0f,     0.0f, 1.0f, 1.0f,     0.0f, TEXTURE  //back left

};

GLfloat vertices3[] =
{
//Positions          //Normals
// --------------------------------------
//Back Face          //Negative Z Normals
-0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,     1.0f, 1.0f, 0.0f,     1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 1.0f,     0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f,

//Front Face         //Positive Z Normals
-0.5f, -0.5f,  0.5f,       0.0f,  0.0f,  1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
0.5f, -0.5f,  0.5f,       0.0f,  0.0f,  1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
0.5f,  0.5f,  0.5f,       0.0f,  0.0f,  1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
0.5f,  0.5f,  0.5f,       0.0f,  0.0f,  1.0f,     1.0f, 1.0f, 0.0f,     1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,       0.0f,  0.0f,  1.0f,     1.0f, 0.0f, 1.0f,     0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,       0.0f,  0.0f,  1.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f,

//Left Face          //Negative X Normals
-0.5f,  0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
-0.5f,  0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,     1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
-0.5f, -0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 1.0f,     0.0f, 1.0f,

//Right Face         //Positive X Normals
0.5f,  0.5f,  0.5f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
0.5f,  0.5f, -0.5f,       1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
0.5f, -0.5f, -0.5f,       1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
0.5f, -0.5f, -0.5f,       1.0f,  0.0f,  0.0f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f,
0.5f, -0.5f,  0.5f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f, 1.0f,     1.0f, 0.0f,
0.5f,  0.5f,  0.5f,       1.0f,  0.0f,  0.0f,     0.0f, 1.0f, 1.0f,     1.0f, 1.0f,

//Bottom Face        //Negative Y Normals
-0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,       0.0f, -1.0f,  0.0f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,       0.0f, -1.0f,  0.0f,     0.0f, 1.0f, 1.0f,     0.0f, 0.0f,

//Top Face           //Positive Y Normals
-0.5f,  0.5f, -0.5f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 1.0f, //back left
0.5f,  0.5f, -0.5f,       0.0f,  1.0f,  0.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f, //back right
0.5f,  0.5f,  0.5f,       0.0f,  1.0f,  0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f, //front right

0.5f,  0.5f,  0.5f,       0.0f,  1.0f,  0.0f,     1.0f, 1.0f, 0.0f,     1.0f, 1.0f, //front right
-0.5f,  0.5f,  0.5f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f, 1.0f,     0.0f, 1.0f, //front left
-0.5f,  0.5f, -0.5f,       0.0f,  1.0f,  0.0f,     0.0f, 1.0f, 1.0f,     0.0f, 1.0f  //back left

};




int main()
{
	// initilializing GLFW
	glfwInit();

	// tells GLDW whats version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	// tells GLFW we are using core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates a GLFWwindow 800x800 pixels, naming it "Jehu Domenech Ortiz"
	GLFWwindow* window = glfwCreateWindow(width, height, "Jehu Domenech Ortiz", NULL, NULL);
	// error check if window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// introduces window to context
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//loading glad to confuigure OpenGL
	gladLoadGL();
	// specifying viewport
	glViewport(0, 0, width, height);
	//glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, 0.1f, 200.0f);

	// generates shader objects with default.vert file and default, frag file
	Shader shaderProgram("default.vert", "default.frag");
	Shader lightingShader("1.colors.vs", "1.colors.fs");
	Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");


	// generating vao and binding
	//floor
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices2, sizeof(vertices2));
	//EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	//EBO1.Unbind();


	//pyramid
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(vertices3, sizeof(vertices3));
	//EBO EBO2(indices3, sizeof(indices3));
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	VBO2.Unbind();
	VAO2.Unbind();
	//EBO2.Unbind();

	//light
	VAO VAO3;
	VAO3.Bind();
	VBO VBO3(verticesCube, sizeof(verticesCube));
	VAO3.LinkAttrib(VBO3, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	VBO3.Unbind();
	VAO3.Unbind();

	//pyramid
	VAO VAO4;
	VAO4.Bind();
	VBO VBO4(pyramid, sizeof(pyramid));
	EBO EBO1(pyramidIndices, sizeof(pyramidIndices));
	VAO4.LinkAttrib(VBO4, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO4.LinkAttrib(VBO4, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO4.LinkAttrib(VBO4, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO4.LinkAttrib(VBO4, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	VBO4.Unbind();
	VAO4.Unbind();
	EBO1.Unbind();
	

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// load and generate the texture
	int widthimg, heightimg, nrChannels;
	unsigned char* data = stbi_load("wall.jpg", &widthimg, &heightimg, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthimg, heightimg, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// getting id of scale uniform
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// used for the rotation of pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// enabling depth buffer
	glEnable(GL_DEPTH_TEST);

	// while loop
	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);
		
		// background color
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// cleans the back and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		//lightingShader.Activate();
		
		// timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}
		//floor
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, floorPos);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		int lightColorLoc = glGetUniformLocation(shaderProgram.ID, "lightColor");
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		int objectColorLoc = glGetUniformLocation(shaderProgram.ID, "objectColor");
		glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
		int lightPositionLoc = glGetUniformLocation(shaderProgram.ID, "lightPos");
		glUniform3fv(lightPositionLoc, 1, glm::value_ptr(lightPos));
		int viewPositionLoc = glGetUniformLocation(shaderProgram.ID, "viewPos");
		glUniform3fv(viewPositionLoc, 1, glm::value_ptr(cameraPos));
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);

		//floor
		VAO1.Bind();
		//glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//pyramid
		VAO2.Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePos);
		modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawElements(GL_TRIANGLES, sizeof(indices3) / sizeof(int), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePos2);
		modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawElements(GL_TRIANGLES, sizeof(indices3) / sizeof(int), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePos3);
		modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//light fixtures
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.3f));
		modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		VAO4.Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos3);
		model = glm::scale(model, glm::vec3(0.6f));
		modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		//lighting
		VAO3.Bind();
		lightCubeShader.Activate();

		int lightCubeProjLoc = glGetUniformLocation(lightCubeShader.ID, "projection");
		glUniformMatrix4fv(lightCubeProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
		int lightCubeViewLoc = glGetUniformLocation(lightCubeShader.ID, "view");
		glUniformMatrix4fv(lightCubeViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		int lightCubeModelLoc = glGetUniformLocation(lightCubeShader.ID, "model");
		glUniformMatrix4fv(lightCubeModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos2);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeModelLoc = glGetUniformLocation(lightCubeShader.ID, "model");
		glUniformMatrix4fv(lightCubeModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glUseProgram(0);
		// swapping fron buffer with back buffer
		glfwSwapBuffers(window);

		glfwPollEvents();
	}



	// deletes all objects
	VAO1.Delete();
	VBO1.Delete();
	//EBO1.Delete();
	shaderProgram.Delete();
	// deletes window
	glfwDestroyWindow(window);
	// kills GLFW
	glfwTerminate();
	return 0;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraSpeed -= (float)xoffset;
	cameraSpeed -= (float)yoffset;
	if (cameraSpeed < 0.1f)
		cameraSpeed = 0.1f;
	if (cameraSpeed > 1.0f)
		cameraSpeed = 1.0f;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
		 // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	//sets it in perspective mode
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
		projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f);
	//sets it in ortho mode
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -10.0f, 1000.0f);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraSpeed * -cameraUp;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
