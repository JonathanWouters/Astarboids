#version 400

in vec2 fUV;
in vec4 fColor;
in float fChannel;

uniform sampler2D uniTex;

// Output Variables
out vec4 OutputColor;
void main(void)
{
	vec4 texColor = texture2D(uniTex, fUV);

	OutputColor = fColor * texColor[int(fChannel)];
}