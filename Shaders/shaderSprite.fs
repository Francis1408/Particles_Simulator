#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform float ZBuffer[512]; // By defauly our screen size is 1024, so half of it
uniform float screenWidth; // Total Screen Width
uniform float spriteDepth; // The TransformY that checks how deep is the srpite on the POV

uniform vec3 spriteColor;



void main()
{       

    // Since we are drawing half of screen, the first position of the screenX is >= screenWidth/2
    // Get the pixels screen X coordinate
    int screenX = int(gl_FragCoord.x) - int(screenWidth/2);

    // Read the wall distance for this pixel
    float wallDepth = ZBuffer[screenX];

    // If the sprite is behind the wall, skip drawing
    if(spriteDepth > wallDepth)
        discard;

    
    vec4 texColor = vec4(spriteColor, 1.0) * texture(image, TexCoords);

    // Discart if the pixel is black (or almost black)
    if(texColor.r <= 0.001 && texColor.g <= 0.001 && texColor.b <= 0.001)
        discard;

    color = texColor;
    

} 