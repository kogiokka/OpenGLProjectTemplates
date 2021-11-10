#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 modelMat;
uniform mat4 viewProjMat;

out vec3 frag_Color;

void main()
{
    gl_Position = viewProjMat * modelMat * vec4(position, 1.0);
    frag_Color = color;
}
