#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

uniform vec4 palette[256];



void main()
{    

    uint id = uint(texture(image, TexCoords).r * 255.0);
    // GL_REPEAT willt tile the texture, so dont need to fract()
    color = palette[id];
}  