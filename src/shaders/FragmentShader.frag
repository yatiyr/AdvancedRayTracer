#version 460

uniform sampler2D srcTex;

in vec2 texCoord;

out vec4 color;

void main()
{
    vec4 c = texture(srcTex, texCoord);
    color = c;
}