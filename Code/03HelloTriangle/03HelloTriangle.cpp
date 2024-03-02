#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
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
}
const char* vertexShaderSource = 
            "#version 330 core\n"
            "layout(location =0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
            "}\n"
;
const char* fragmentShaderSource =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "FragColor=vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n"
;
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
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };
    //数据进入顶点缓存
    unsigned int VBO,VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    

    //链接顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(0);
    //顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR:shader vertex compilation failed\n" << infoLog << std::endl;
    }
    //片段着色器
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(fragShader,512,NULL,infoLog);
        std::cout << "ERROR:shader fragment compilation failed!\n" << infoLog << std::endl;
    }

    //着色程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram,fragShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR:program link failed\n" << infoLog << std::endl;
    }
    //glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    while (!glfwWindowShouldClose(window))
    {
        procInput(window);

        //render here
        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

