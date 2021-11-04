#version 330

layout (location = 0) in vec3 position;

uniform mat4 modelMat;
uniform mat4 viewProjMat;

void main()
{
    gl_Position = viewProjMat * modelMat * vec4(position, 1.0);
}
