cbuffer OBJECT : register(b1)
{
	float4x4 WorldMatrix;
};

cbuffer SCENE : register(b2)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

struct VERT_IN
{
	float4 pos : POSITION;
	float3 norm : NORMAL;
	float2 uv : UV;
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float3 norm_o : NORMAL;
	float2 uv_o : UV;
	float4 worldPos: POS;
};

// TODO: PART 3 STEP 2a
cbuffer THIS_IS_VRAM : register(b0)
{
	float4 constantColor;
	float2 constantOffset;
	float2 padding;
};

OUTPUT_VERTEX main(VERT_IN fromVertexBuffer)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	float4 temp = float4(fromVertexBuffer.pos.xyz, 1);
	temp = mul(WorldMatrix, temp);
	sendToRasterizer.worldPos = temp;
	temp = mul(viewMatrix, temp);
	temp = mul(projectionMatrix, temp);

	
	
	sendToRasterizer.projectedCoordinate = temp;
	sendToRasterizer.norm_o = fromVertexBuffer.norm;
	sendToRasterizer.uv_o = fromVertexBuffer.uv;
	
	return sendToRasterizer;
}