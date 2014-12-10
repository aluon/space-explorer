#version 120

uniform samplerCube CubeMap;

void main()
{
	gl_FragColor = textureCube(CubeMap, gl_TexCoord[0].stp);
}
