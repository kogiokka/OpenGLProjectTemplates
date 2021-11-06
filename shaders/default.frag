#version 150

in vec3 frag_Color;
out vec4 outColor;

void main()
{
    outColor = vec4(frag_Color, 1.0);
}
