
#include "spriteRenderer.h"

// GLM Mathematics Library headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <sstream>
#include <fstream>



SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    //glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f

    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

    
void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, 
glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 pivot)
  {
      // prepare transformations
      this->shader.Use();
      glm::mat4 model = glm::mat4(1.0f);

      // 1. Translate to the sprite's position
        model = glm::translate(model, glm::vec3(position, 0.0f));

        // 2. Translate to the pivot point (in local space)
        model = glm::translate(model, glm::vec3(pivot.x * size.x, pivot.y * size.y, 0.0f));

        // 3. Rotate around the pivot
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));

        // 4. Translate back from pivot
        model = glm::translate(model, glm::vec3(-pivot.x * size.x, -pivot.y * size.y, 0.0f));

        // 5. Scale the sprite to the correct size
        model = glm::scale(model, glm::vec3(size, 1.0f));
      
    
    
      this->shader.SetMat4("model", model);
      this->shader.SetVec3("spriteColor", color);
    
      glActiveTexture(GL_TEXTURE0);
      texture.Bind();
  
      glBindVertexArray(this->quadVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
  } 