#version 330 core
layout (location = 0) in vec4 vertex; 

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform float u_start;
uniform float u_end;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vec2(u_start + (u_end - u_start) * vertex.z, vertex.w);
}