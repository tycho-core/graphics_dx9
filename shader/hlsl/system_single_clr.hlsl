// simple single colour per vertex shader
float4x4 world_view_proj;
float4x4 world_matrix;

// light direction and colour
float4 light_dir;
float4 light_clr;
float4 obj_clr;

struct VS_OUTPUT
{
	float4 position : POSITION;
	float4 diffuse  : COLOR0;
};
	
struct PS_OUTPUT
{
	float4 clr : COLOR0;
};

PS_OUTPUT ps_main(VS_OUTPUT input)
{
	PS_OUTPUT output;
	output.clr = input.diffuse;
	return output;
}

VS_OUTPUT vs_main(float4 vPos : POSITION, float4 vNormal : NORMAL)
{
	float3 ambient = float3(0.5, 0.5, 0.5);
	VS_OUTPUT output;
	float4 world_pos = mul(vPos, world_matrix);
	output.position = mul(world_pos, world_view_proj);
	float3 diffuse = ambient + light_clr * dot((float3)vNormal, -(float3)light_dir);
	output.diffuse = float4(diffuse.x, diffuse.y, diffuse.z, 1) * obj_clr;
	return output;
}

