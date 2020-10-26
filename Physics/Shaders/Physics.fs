#version 330 core
out vec4 fragColour;

in myInterface{
	vec3 colour;
	vec2 texCoords;
} fsIn;

uniform bool text;
uniform bool useTex;
uniform bool useCustom;
uniform float myAlpha;
uniform sampler2D tex;
uniform vec3 textColour;
uniform vec3 myColour;

void main(){
	if(useCustom){
		fragColour = useTex ? (texture2D(tex, fsIn.texCoords) * vec4(myColour, myAlpha)) : vec4(myColour, myAlpha);
	} else{
		fragColour = useTex ? (texture2D(tex, fsIn.texCoords) * vec4(text ? textColour : fsIn.colour, 1.f)) : vec4(fsIn.colour, 1.f);
	}
}