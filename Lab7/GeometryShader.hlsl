struct VERT_OUT
{
		float4 pos_o : SV_POSITION;
		float3 norm_o : NORMAL;
		float2 uv_o : UV;
		float4 worldPos: POS;
};

struct VERT_IN
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 uv : UV;
	float4 worldPos: POS;
};

cbuffer OBJECT : register(b1)
{
	float4x4 WorldMatrix;
};

cbuffer SCENE : register(b2)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}


[maxvertexcount(6)]
void main(point VERT_IN input[1],inout TriangleStream< VERT_OUT > output)
{

	float offset = 10;




	for (uint i = 0; i < 6; i++)
	{
		VERT_OUT send = (VERT_OUT)0;
		switch (i)
		{
		case 0:
		{
			send.pos_o = input[0].pos;
			send.pos_o.x -= offset;
			send.pos_o.y -= offset;
			send.uv_o = float2(0, 1);
			break;
		}
		case 1:
		{
			send.pos_o = input[0].pos;
			send.pos_o.x -= offset;
			send.pos_o.y += offset;
			send.uv_o = float2(0, 0);
			break;
		}
		case 2:
		{
			send.pos_o = input[0].pos;
			send.pos_o.x += offset;
			send.pos_o.y += offset;
			send.uv_o = float2(1, 0);
			break;
		}
		case 3:
		{
			output.RestartStrip();
			send.pos_o = input[0].pos;
			send.pos_o.x -= offset;
			send.pos_o.y -= offset;
			send.uv_o = float2(0, 1);
			break;
		}
		case 4:
		{
			send.pos_o = input[0].pos;
			send.pos_o.x += offset;
			send.pos_o.y += offset;
			send.uv_o = float2(1, 0);
			break;
		}
		case 5:
		{
			send.pos_o = input[0].pos;
			send.pos_o.x += offset;
			send.pos_o.y -= offset;
			send.uv_o = float2(1, 1);
			break;
		}
		default:
			break;
		}

		send.pos_o = mul(WorldMatrix, send.pos_o);
		send.pos_o = mul(viewMatrix, send.pos_o);
		send.pos_o = mul(projectionMatrix, send.pos_o);

		send.norm_o = input[0].norm;
		


		output.Append(send);
	}

	output.RestartStrip();
}