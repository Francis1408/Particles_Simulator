#include <iostream>

#include "texture.h"

// Default Constructor
Texture2D::Texture2D()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}

// Constructor with arguments
Texture2D::Texture2D(unsigned int internal_format, unsigned int image_format, unsigned int wrap_s, 
    unsigned int wrap_t, unsigned int filter_min, unsigned int filter_max): 
    Width(0), Height(0), Internal_Format(internal_format), Image_Format(image_format), Wrap_S(wrap_s), Wrap_T(wrap_t),
    Filter_Min(filter_min), Filter_Max(filter_max) 
    {
        glGenTextures(1,  &this->ID);
    }

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->Width = width;
    this->Height = height;
    this->IsInitialized = true; // glTexImage2D is called

    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture2D::Update(unsigned char* data) {

    glBindTexture(GL_TEXTURE_2D, this->ID);
    // GL method that updates texture
    // glTexImage2D must be called previously
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->Width, this->Height, this->Image_Format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}