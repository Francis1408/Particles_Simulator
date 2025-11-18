#include "character.h"


// Constructor with arguments
Character::Character(unsigned int textureId, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance) 
    : TextureID(textureId), Size(size), Bearing(bearing), Advance(advance) {

}