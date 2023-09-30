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

        if (data.size() % stride != 0)
            throw std::invalid_argument("Size of data must be a multiple of layout width");

        trianglesNum = data.size() / stride;

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
        else
            EBO = 0;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    template<typename T>
    void AttachData(std::vector<T> data, std::vector<GLenum> datatypes, std::vector<int> layout)
    {

    }

    void Draw()
    {
        glBindVertexArray(VAO);
        if (EBO != 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, trianglesNum, GL_UNSIGNED_INT, 0);
        }
        else
            glDrawArrays(GL_TRIANGLES, 0, trianglesNum);

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
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint trianglesNum = 0;

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
