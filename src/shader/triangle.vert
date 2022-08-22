#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 aColor;

out vec3 fragPos;
out vec3 color;
out vec3 normal;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
	fragPos = aPos;
	color = aColor;
	normal = aNormal;
}