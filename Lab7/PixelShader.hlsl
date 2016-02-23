texture2D text : register(t1);

SamplerState samp : register(s0);

cbuffer DLIGHT : register(b0)
{
	float4 dir0;
	float4 color0;
}
cbuffer PLIGHT : register(b1)
{
	float4 pos1;
	float4 dir1;
	float4 color1;
}
cbuffer SLIGHT : register(b2)
{
	float4 pos2;
	float4 dir2;
	float4 color2;
}

struct INPUT
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 uv : UV;
};



float4 main(INPUT stuff) : SV_TARGET
{
	float4 temp = text.Sample(samp, stuff.uv);
	//direction light
	float rat = saturate(dot(dir0.xyz, stuff.norm));
	float4 result = rat*color0*temp;
	//point light
	float3 lightdir = normalize(pos1.xyz - stuff.pos.xyz);
	float rat1 = saturate(dot(-lightdir, stuff.norm));
	float4 result1 = rat1*color1*temp;
	//spot light
	float3 lightdir2 = normalize(pos2.xyz - stuff.pos.xyz);
	float rat2 = saturate(dot(-lightdir2, dir2.xyz));
	float spotF = (rat2 > .9) ? 1 : 0;
	float lightrat = saturate(dot(-lightdir2, stuff.norm));
	float4 result2 = spotF*lightrat*color2*temp;
		return result+ result1+ result2;
}