
#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include <vector>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particular texture
    unsigned int ID;
    // texture image dimensions
    unsigned int Width, Height; // width and height of loaded image in pixels
    // texture Format
    unsigned int Internal_Format; // format of texture object
    unsigned int Image_Format; // format of loaded image
    // texture configuration
    unsigned int Wrap_S; // wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    
    //====== ONLY USED FOR FLOOR CASTING =======
    std::vector<unsigned char> PixelBuffer; // Buffer to save image content on CPU-SIDE only when is nedded
    bool IsInitialized = false; // Flag to check if the glTexImage2D was called once so it is possible to Update the buffer
    // Update the texture inside the class
    void Update(unsigned char* data);
    //===============================
    
    // constructor (sets default texture modes)
    Texture2D();
    // constructor with params
    Texture2D(unsigned int internal_format, unsigned int image_format, unsigned int wrap_s, 
              unsigned int wrap_t, unsigned int filter_min, unsigned int filter_max);

    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

#endif

