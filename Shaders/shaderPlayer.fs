#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    

    vec4 texColor = vec4(spriteColor, 1.0) * texture(image, TexCoords);
    
    // Discart if the pixel is black (or almost black)
    if(texColor.r <= 0.001 && texColor.g <= 0.001 && texColor.b <= 0.001)
        discard;

    color = texColor;
} 