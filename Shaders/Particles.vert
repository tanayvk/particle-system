#version 330 core

layout (location = 0) in vec3 vPos;

uniform mat4 Transform;

void main()
{
    gl_Position = Transform * vec4(vPos, 1.0);
}
