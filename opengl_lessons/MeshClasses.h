#ifndef MESHCLASSES_H
#define MESHCLASSES_H

#include <glad/glad.h>
#include <vector>
#include <numeric>


class BaseMesh
{

public:
    //template<typename T>
    //BaseMesh(std::vector<T> data, std::vector<int> layout, std::vector<unsigned int> indices = {});
    template<typename T>
    BaseMesh(std::vector<T> data, std::vector<GLenum> datatypes, std::vector<int> layout, std::vector<unsigned int> indices = {})
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);        

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);

        int stride = std::accumulate(layout.begin(), layout.end(), 0);
        int offset = 0;
        for (int i = 0; i < layout.size(); ++i)
        {
            // TODO: check if datatypes are correct enum values
            glVertexAttribPointer(i, layout[i], datatypes[i], GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
            glEnableVertexAttribArray(i);
            offset += layout[i];
        }

        if (!indices.empty())
        {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    ~BaseMesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    GLuint GetVAO() { return VAO; }
    //virtual void Draw();
private:
    GLuint VAO, VBO, EBO;

    //template<typename P>
    //struct DataTypeTraits {};

    //template<>
    //struct DataTypeTraits<float>
    //{
    //    static const GLenum dataType = GL_FLOAT;
    //};

    //template<>
    //struct DataTypeTraits<int>
    //{
    //    static const GLenum dataType = GL_INT;
    //};
};



#endif
