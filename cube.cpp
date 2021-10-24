#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 vertexColour;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"

    "out vec4 fragmentColour;\n"

    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   fragmentColour = vertexColour;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec4 fragmentColour;\n"
    "out vec4 colour;\n"
    ""
    "void main()\n"
    "{\n"
    "   colour = fragmentColour;\n"
    "}\0";

int main(){
    /*
    -----------INITIALIZATION-----------
    */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //major version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //minor version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profile

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for apple users
#endif

    const unsigned int width = 800, height = 600;
    GLFWwindow *win = glfwCreateWindow(width, height, "Cube", NULL, NULL);
    if(!win){
        cout << "Error creating OpenGL window" << endl;
        return -1;
    }
    glfwMakeContextCurrent(win);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        cout << "Error initializing glew" << endl;
    }
    
    /*----------SHADER SETUP----------*/
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM:LINKING_FAILED\n" << infoLog << endl;
        return -1;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    float vertices[]{
       -0.5f, 0.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 
       0.0f, 0.5f, 0.0f, 
       -0.5f, 0.5f, 0.0f,
       0.0f, 0.5f, 0.5f,
       -0.5f, 0.5f, 0.5f, 
       0.0f, 0.0f, 0.5f,
       -0.5f, 0.0f, 0.5f
    };

    float colours[]{
        0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        0.4f, 0.25f, 0.6f, 1.0f, 
        0.05f, 0.41f, 0.06f, 1.0f, 
        0.1f, 1.0f, 0.4f, 1.0f, 
        0.3f, 0.7f, 0.9f, 1.0f, 
        1.0f, 1.0f, 1.0f, 1.0f,
        0.4f, 0.3f, 0.1f, 1.0f
    };

    unsigned int indices[]{
       //front face
       0, 1, 2,
       0, 2, 3, 

       //right face
       1, 6, 2,
       6, 4, 2,

       //left face
       7, 0, 3,
       7, 3, 5,

       //top face
       3, 2, 4,
       3, 4, 5, 

       //bottom face
       0, 6, 1, 
       0, 7, 6,

       //back face
       7, 5, 4, 
       6, 7, 4

    };

    /*----------BUFFER CREATION----------*/
    unsigned int vbo, vao, ibo, colourBuffer;
    glGenVertexArrays(1, &vao); //creates vertex array object
    glGenBuffers(1, &vbo); //creates vertex buffer 
    glGenBuffers(1, &ibo); //creates index buffer
    glGenBuffers(1, &colourBuffer); //creates colour buffer

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    /*
    ----------RENDER LOOP----------
    */
    while(!glfwWindowShouldClose(win)){
        glClearColor(0.25f, 0.0f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &colourBuffer);
    glDeleteProgram(shaderProgram);


    return 0;
}