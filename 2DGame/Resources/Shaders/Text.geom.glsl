#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matProjection;
uniform vec2 uniTextureSize;

in GS_INPUT
{
	int Channel; //Texture Channel
	vec3 Position; //Left-Top Character Quad Starting Position
	vec4 Color; //Color of the vertex
	vec2 TexCoord; //Left-Top Character Texture Coordinate on Texture
	vec2 CharSize; //Size of the character (in screenspace)
} vert[];

out vec2 fUV;
out vec4 fColor;
out float fChannel;

void CreateVertex(vec3 pos, vec4 col, vec2 uv, int channel)
{
	gl_Position = matProjection * vec4(pos, 1.0f);
	fUV = uv;
	fColor = col;
	fChannel = channel;
	EmitVertex();
}

void main()
{
	//Create a Quad using the character information of the given vertex
	//Note that the Vertex.CharSize is in screenspace, TextureCoordinates aren't ;) [Range 0 > 1]
	vec2 texCoord = vert[0].CharSize / uniTextureSize;

	//1. Vertex Left-Top
	CreateVertex(vert[0].Position, vert[0].Color, vert[0].TexCoord + vec2(0, texCoord.y), vert[0].Channel);

	//2. Vertex Right-Top
	CreateVertex(vert[0].Position + vec3(vert[0].CharSize.x, 0, 0), vert[0].Color, vert[0].TexCoord + vec2(texCoord.x, texCoord.y) , vert[0].Channel);
																    
	//3. Vertex Left-Bottom										    
	CreateVertex(vert[0].Position + vec3(0, vert[0].CharSize.y, 0), vert[0].Color, vert[0].TexCoord , vert[0].Channel);

	//4. Vertex Right-Bottom
	CreateVertex(vert[0].Position + vec3(vert[0].CharSize.x, vert[0].CharSize.y, 0), vert[0].Color, vert[0].TexCoord + vec2(texCoord.x, 0), vert[0].Channel);

	EndPrimitive();
}