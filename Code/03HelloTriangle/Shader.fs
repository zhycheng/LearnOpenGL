#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float factor;
uniform float time;

void main()
{
	FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),abs(cos(time)*0.5f));
	// FragColor=texture(texture2,TexCoord);
}