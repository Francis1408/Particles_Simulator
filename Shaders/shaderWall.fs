#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float texXOffset;


void main()
{    
    // Adjusted texture lookup: only sample a vertical slice
    vec2 newTexCoords = vec2(texXOffset, TexCoords.y);
    color = vec4(spriteColor, 1.0) * texture(image, newTexCoords);
}  