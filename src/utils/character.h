// Character class used to hold all state information relevant to a character as loaded using FreeType
#ifndef CHARACTER_H
#define CHARACTER_H


#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


class Character {

    public:
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph


    // Constructor
    Character(unsigned int textureID, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance);
};

#endif
