#version 430 core

attribute vec3 vertPosition;
attribute vec4 vertColor;

varying vec4 fragColor;

void main()
{
	fragColor = vertColor;
	gl_Position = vec4(vertPosition, 1.0f);
}
