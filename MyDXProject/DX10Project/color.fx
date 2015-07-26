cbuffer cbPerObject
{
	float4x4 gWVP;
};
RasterizerState Wireframe
{
	FillMode = WIREFRAME;
	CullMode = BACK;
	FrontCounterClockwise = true;
};
void VS(float3 iPosL : POSITION, 
	float4 iColor : COLOR,
	out float4 oPosH : SV_POSITION,
	out float4 oColor : COLOR)
{
	oPosH = mul(float4(iPosL, 1.0f), gWVP);
	oColor = iColor;
}
float4 PS(float4 posH : SV_POSITION,
	float4 color : COLOR) : SV_Target
{
	return color;
}

technique10 ColorTech
{
	pass Pass1
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
		//SetRasterizerState(Wireframe);
	}
}