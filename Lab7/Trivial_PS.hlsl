TextureCube skybox : register(t0);

SamplerState samp : register(s0);

struct INPUT
{
	float3 uv : UV;
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
};



float4 main(INPUT stuff) : SV_TARGET
{
	float4 temp = skybox.Sample(samp, stuff.uv);
	return temp;
}