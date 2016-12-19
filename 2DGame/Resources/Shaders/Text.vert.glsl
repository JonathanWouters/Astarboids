#version 400

in int vChannel;
in vec3 vPosition;
in vec4 vColor;
in vec2 vTexCoord;
in vec2 vCharSize;


out GS_INPUT
{
	int Channel; //Texture Channel
	vec3 Position; //Left-Top Character Quad Starting Position
	vec4 Color; //Color of the vertex
	vec2 TexCoord; //Left-Top Character Texture Coordinate on Texture
	vec2 CharSize; //Size of the character (in screenspace)
} vsOutput;


void main()
{
	vsOutput.Channel = vChannel;
	vsOutput.Position = vPosition;
	vsOutput.Color = vColor;
	vsOutput.TexCoord = vTexCoord;
	vsOutput.CharSize = vCharSize;

	gl_Position = vec4(0,0,0, 1.0f);
}