#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;


// Outputs the color for the Fragment Shader
out vec3 color;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

// Controls the scale of the vertices
uniform float scale;


// Inputs the matrices needed for 3D viewing with perspective
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

	FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
	gl_Position = projection * view * vec4(FragPos, 1.0);
	color = aColor;
	TexCoord = aTexCoord;

	//gl_Position = projection * view * model * vec4(aPos, 1.0);

}