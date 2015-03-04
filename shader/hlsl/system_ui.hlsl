// simple single colour per vertex shader
float4x4	proj_matrix;
float4x4	world_matrix;
float4		obj_clr;
texture	diffuse_texture;
sampler diffuse_texture_sampler = 
sampler_state
{
	Texture = <diffuse_texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_OUTPUT
{
	float4 position : POSITION;
	float4 diffuse  : COLOR0;
	float2 tex0		: TEXCOORD0;
};
	
struct PS_OUTPUT
{
	float4 clr : COLOR0;
};

VS_OUTPUT vs_main(float4 vPos : POSITION, float4 vNormal : NORMAL, float2 vTex0 : TEXCOORD0)
{
	VS_OUTPUT output;
	float4 world_pos = mul(vPos, world_matrix);
	
	output.position.x = world_pos.x * 2 - 1;
	output.position.y = world_pos.y * 2 - 1;
	output.position.z = world_pos.z;
	output.position.w = 1;
	output.diffuse = obj_clr;
	output.tex0 = vTex0;
	return output;
}

PS_OUTPUT ps_main(VS_OUTPUT input)
{
	PS_OUTPUT output;
	output.clr = tex2D(diffuse_texture_sampler, input.tex0);// * input.diffuse;
	//output.clr.rgb = output.clr.aaa;
	return output;
}


