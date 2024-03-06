#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Shader.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
using namespace glm;

float s = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void procInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        s += 0.001f;
        if (s > 1)
        {
            s = 1.0f;
        }
        std::cout<<"ket up clicked" << std::endl;

    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        
        s -= 0.001f;
        if (s < 0)
        {
            s = 0;
        }
        std::cout << "key down clicked" << std::endl;
    }
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //顶点数据
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    };
    //数据进入顶点缓存
    unsigned int VBO,VAO,EBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices,GL_STATIC_DRAW);
    

    //链接顶点属性 坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //纹理坐标
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);

    Shader sh("Shader.vs","Shader.fs");



    unsigned int texture1,texture2;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("brag.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);
   

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    data = stbi_load("ask.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    sh.use();
    sh.setInt("texture1", 0);
    sh.setInt("texture2", 1);

 
    



    while (!glfwWindowShouldClose(window))
    {
        procInput(window);

        //render here
        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        sh.use();

        /*
        int colorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        float time = glfwGetTime();
        float greenValue = (sin(time)+1.0f)/2;
        glUniform4f(colorLocation,0.0f, greenValue,0.0f,1.0f);*/

       

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        sh.setFloat("factor", s);

        mat4 trans = mat4(1.0f);
        trans = translate(trans, vec3(0.2f, 0, 0));
        trans = rotate(trans, (float)glfwGetTime()*0.1f, vec3(0, 0, 1.0f));
        unsigned int translateDoc = glGetUniformLocation(sh.ID,"transform");
        glUniformMatrix4fv(translateDoc, 1, GL_FALSE, value_ptr(trans));
        float time = glfwGetTime();
        sh.setFloat("time", time);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(sh.ID);
    glfwTerminate();
    return 0;
}

