#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;
layout(location = 2) in vec3 aNormals;
layout(location = 3) in vec2 aTexCoords;

out myInterface{
	vec3 colour;
	vec2 texCoords;
} vsOut;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(aPos, 1);
	vsOut.colour = aColour;
	vsOut.texCoords = aTexCoords;
}