#version 400

in vec4 vTransformData;
in vec4 vTransformData2;
in vec4 vColor;


out GS_INPUT
{
	vec4 TransformData;
	vec4 TransformData2;
	vec4 Color;
} vsOutput;


void main()
{
	vsOutput.TransformData = vTransformData;
	vsOutput.TransformData2 = vTransformData2;
	vsOutput.Color = vColor;

	gl_Position = vec4(0,0,0, 1.0f);
}