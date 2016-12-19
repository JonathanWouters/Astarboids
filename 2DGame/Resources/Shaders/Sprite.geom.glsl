#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matProjection;

in GS_INPUT
{
	vec4 TransformData;  //PosX, PosY, Depth (PosZ), Rotation
	vec4 TransformData2; //PivotX, PivotY, ScaleX, ScaleY
	vec4 Color;
} vert[];

out vec2 fUV;
out vec4 fColor;

void CreateVertex(vec3 pos, vec2 uv, float rotation, vec2 rotCosSin, vec2 offset, vec2 pivotOffset,vec4 color)
{
	if (rotation != 0)
	{
		//Do rotation calculations
		//Transform to origin
		pos.x -= offset.x;
		pos.y -= offset.y;

		pos.x -= pivotOffset.x;
		pos.y -= -pivotOffset.y;

		//Rotate
		vec2 startPos = pos.xy;
		pos.x = (startPos.x * rotCosSin.x) - (startPos.y * rotCosSin.y);
		pos.y = (startPos.y * rotCosSin.x) + (startPos.x * rotCosSin.y);

		//Transform back
		pos += vec3(offset, 0);
	}
	else
	{
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		pos.x -= pivotOffset.x;
		pos.y -= -pivotOffset.y;
	}

	//Geometry Vertex Output
	gl_Position = matProjection * vec4(pos, 1.0f);

	fUV = uv;
	fColor = color;
	EmitVertex();
}

void main()
{
	//Given Data (Vertex Data)
	vec3 position = vec3(vert[0].TransformData.x, vert[0].TransformData.y, vert[0].TransformData.z); //Extract the position data from the VS_DATA vertex struct
	vec2 offset = vec2(vert[0].TransformData.x, vert[0].TransformData.y); //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
	float rotation = vert[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
	vec2 pivot = vec2(vert[0].TransformData2.x, vert[0].TransformData2.y); //Extract the pivot data from the VS_DATA vertex struct
	vec2 size = vec2(vert[0].TransformData2.z, vert[0].TransformData2.w); //Extract the scale data from the VS_DATA vertex struct
	vec2 pivotOffset = size *pivot;
	vec2 rotCosSin = vec2(0, 0);

	if (rotation != 0)
	{
		rotCosSin.x = cos(rotation);
		rotCosSin.y = sin(rotation);
	}

	CreateVertex(position								, vec2(0.0f, 0.0f), rotation, rotCosSin, offset, pivotOffset, vert[0].Color);
	CreateVertex(position + vec3(0.0f  , -size.y, 0.0f) , vec2(0.0f, 1.0f), rotation, rotCosSin, offset, pivotOffset, vert[0].Color);
	CreateVertex(position + vec3(size.x,	0.0f, 0.0f) , vec2(1.0f, 0.0f), rotation, rotCosSin, offset, pivotOffset, vert[0].Color);
	CreateVertex(position + vec3(size.x, -size.y, 0.0f) , vec2(1.0f, 1.0f), rotation, rotCosSin, offset, pivotOffset, vert[0].Color);
	EndPrimitive();
}