#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

// Parameters to map the floor row
uniform vec2 floor;
uniform vec2 floorStep;
uniform int screenWidth;

uniform vec3 spriteColor;



void main()
{    

    // GL_REPEAT willt tile the texture, so dont need to fract()
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}  