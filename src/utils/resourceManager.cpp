
#include "resourceManager.h"
#include "glad/glad.h"


#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <string>

#include "stb_image.h"

namespace fs = std::filesystem;

// Instantiate static variables
std::map<int, Texture2D>  ResourceManager::Textures;
std::vector<std::string> texturePaths;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<GLchar, Character> ResourceManager::Characters;


Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}


void ResourceManager::LoadTextures(const std::string& path_str)
{
    // Create the directory path
    fs::path path(path_str);

    // Create a temporary vector to store the entries
    std::vector<fs::directory_entry> entries;

    // Check if it is the right path
    if(!fs::exists(path) || !fs::is_directory(path)) {

        std::cerr << "ERROR : Invalid directory path." << std::endl;
    }

    // Store all file paths in the string array
    for(const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_regular_file(entry)) {
            entries.push_back(entry);
        }
    }

    // Sort the entries by numeric filename, since the fs picks the files randomly
                                              // Lambda funtion - inline function that saves sorted list into []  
    std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry&b) {

        // gets the path -> remove the extensions name -> transform into string
        int aIndex = std::stoi(a.path().stem().string()); // transform to a int the file name 
        int bIndex = std::stoi(b.path().stem().string());
        return aIndex < bIndex;
    });
   
    

    // Check if there are texture avaiable
    if(entries.empty()) throw std::runtime_error("No textures avaiable");


    // Load the textures into the Textures map
    // The textures will have an index of [1: num_of_textures]
    for(const auto& entry : entries) {
        std::string fullPath = entry.path().string();
        int id = std::stoi(entry.path().stem().string()); // get the int from file name
    
        std::cout << "Loading texture id " << id << ": " << fullPath << std::endl;
        Textures[id] = loadTextureFromFile(fullPath.c_str(), false);
    
    }


}


Texture2D ResourceManager::GetTexture(int index)
{
    return Textures[index];
}

void ResourceManager::LoadCharacter(unsigned char c, Character character ) 
{
    Characters.insert(std::pair<char, Character>(c, character));
}

Character ResourceManager::GetCharacter(unsigned char c) 
{

    // Based on the type of char passed, finds the matching character
    auto it = Characters.find(c);
    if (it == Characters.end()) {
        std::cerr << "ERROR::ResourceManager: Character '" << c << "' not found!" << std::endl;
        throw std::runtime_error("Character not found in ResourceManager");
    }
    // return the character
    return it->second;
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
   // printf("vShaderCode: %s\n", vShaderCode);
   // printf("fShaderCode: %s\n", fShaderCode);
   // printf("gShaderCode: %s\n", gShaderCode);
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;

    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if(data) {
        
        // Save image on the cpu-side buffer
        texture.PixelBuffer.assign(data, data + width * height * nrChannels);

        // now generate texture
        texture.Generate(width, height, data);
        
    } else {
        
        std::cout << "Failed to load texture" << std::endl;
    }

    // and finally free image data
    stbi_image_free(data);
    return texture;
}

