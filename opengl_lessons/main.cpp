
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assert.h>
#include <iostream>
//#include <filesystem>

#include "ShaderHelpers.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool initTexture(const std::string& path, GLuint* texture)
{
    //GLuint texture;
    glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (nrChannels != 3 && nrChannels != 4)
    {
        std::cout << "Unknown texture format" << std::endl;
        return false;
    }

    if (data)
    {
        assert(nrChannels == 3 || nrChannels == 4);
        unsigned int img_fmt = nrChannels == 3 ? GL_RGB : GL_RGBA;   // this is not quite right...
        glTexImage2D(GL_TEXTURE_2D, 0, img_fmt, width, height, 0, img_fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    stbi_image_free(data);
    return true;
}

const std::string SHADERS_ROOT = "..\\Shaders";
const std::string TEXTURES_ROOT = "..\\Textures";

int main() {    
    //std::cout << std::experimental::filesystem::current_path() << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //

    ShaderProgram uniformShaderProgram(SHADERS_ROOT + "\\basicVertexShader.vert", SHADERS_ROOT + "\\uniformColorShader.frag");
    ShaderProgram myShaderProgram(SHADERS_ROOT + "\\basicVertexShader.vert", SHADERS_ROOT + "\\basicFragmentShader.frag");
    ShaderProgram colorShaderProgram(SHADERS_ROOT + "\\vertexColorShader.vert", SHADERS_ROOT + "\\customColorShader.frag");
    ShaderProgram textureShaderProgram(SHADERS_ROOT + "\\textureShader.vert", SHADERS_ROOT + "\\textureShader.frag");
    ShaderProgram transformShaderProgram(SHADERS_ROOT + "\\transformShader.vert", SHADERS_ROOT + "\\textureShader.frag");

    //

    /*float triVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };*/
    

    float triVertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    //float quadVertices[] = {
    // 0.5f,  0.5f, 0.0f,  // top right
    // 0.5f, -0.5f, 0.0f,  // bottom right
    //-0.5f, -0.5f, 0.0f,  // bottom left
    //-0.5f,  0.5f, 0.0f   // top left
    //};

    float quadVertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int quadIndices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    
    //

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    ////// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //// color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //

    stbi_set_flip_vertically_on_load(true);

    bool res;
    unsigned int texture1, texture2;
    res = initTexture(TEXTURES_ROOT + "\\wall.jpg", &texture1);
    if (!res)
        return -1;

    res = initTexture(TEXTURES_ROOT + "\\awesomeface.png", &texture2);
    if (!res)
        return -1;

    //

    //glUseProgram(uniformShaderProgram.getID());
    //glUseProgram(myShaderProgram.getID());
    //glUseProgram(colorShaderProgram.getID());
    //glUseProgram(textureShaderProgram.getID());
    glUseProgram(transformShaderProgram.getID());

    glBindVertexArray(VAO);
    //glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glUniform1i(glGetUniformLocation(textureShaderProgram.getID(), "ourTexture"), 0);
    glUniform1i(glGetUniformLocation(textureShaderProgram.getID(), "ourTexture2"), 1);

    //

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*GLuint shaders[3] = { uniformShaderProgram.getID() , myShaderProgram.getID() , colorShaderProgram.getID() };
    int frameTimer = 0;
    int curShader = 0;*/

    glm::mat4 trans = glm::mat4(1.0f);    
    //trans = glm::mat4({ -1.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f });
    //trans = glm::translate(trans, { 0.2, 0.2, 0.0 });
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    unsigned int transformLoc = glGetUniformLocation(transformShaderProgram.getID(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(uniformShaderProgram.getID(), "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        /*frameTimer++;
        if (frameTimer >= 5000)
        {
            frameTimer = 0;
            curShader = (curShader + 1) % (sizeof(shaders) / sizeof(shaders[0]));
        }
        glUseProgram(shaders[curShader]);*/

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}