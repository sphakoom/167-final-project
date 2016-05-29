#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 TexCoords;

out vec4 color;
uniform samplerCube skybox;
uniform int buildingPart;

void main()
{
	if( buildingPart == 1 ) {
		color = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else if( buildingPart == 2) {
		color = vec4(1.0, 0.5, 0.5, 1.0);
	}
	else
		color = vec4(0.5, 0.5, 1.0, 1.0);
}