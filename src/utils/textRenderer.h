
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H


#include "glad/glad.h"
#include "resourceManager.h"

#include "texture.h"
#include "shader.h"
#include "character.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <filesystem>


namespace fs = std::filesystem;

class TextRenderer
{
    public:
        TextRenderer(Shader &shader);

        ~TextRenderer();

            
    void DrawText(std::string text,
    float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

    void LoadFont(std::string path, int size);

    private:
        Shader       shader; 
        unsigned int quadVAO;
        unsigned int quadVBO;

        void initRenderData();
};

#endif