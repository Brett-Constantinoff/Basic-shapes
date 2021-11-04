#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.h"
#include "camera.h"


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
    GLFWwindow *win = glfwCreateWindow(width, height, "Crazy Shapes", NULL, NULL); //creates current window
    if(!win){
        std::cout << "Error creating OpenGL window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(win);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST); //enabled for 3D viewing
    glDepthFunc(GL_LESS);//compares the depths of pixels


    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Error initializing glew" << std::endl;
    }
    
    /*----------SHADER SETUP----------*/
    Shader myShader("shader.shader");
   
    /*----------BUFFER CREATION----------*/
    float vertices[]{
        //bottom pyramid
        0.0f, -0.5f, 0.0f,
        0.5f, 0.0f, 0.5f, 
        -0.5f, 0.0f, 0.5f,

        0.0f, -0.5f, 0.0f, 
        0.5f, 0.0f, -0.5f, 
        0.5f, 0.0f, 0.5f,

        0.0f, -0.5f, 0.0f, 
        -0.5f, 0.0f, 0.5f, 
        -0.5f, 0.0f, -0.5f,

        0.0f, -0.5f, 0.0f, 
        -0.5f, 0.0f, -0.5f, 
        0.5f, 0.0f, -0.5f,

        //bottom pyramid
        0.0f, 0.5f, 0.0f, 
        0.5f, 0.0f, 0.5f, 
        0.5f, 0.0f, -0.5f, 

        0.0f, 0.5f, 0.0f, 
        0.5f, 0.0f, -0.5f, 
        -0.5f, 0.0f, -0.5f, 

        0.0f, 0.5f, 0.0f, 
        -0.5f, 0.0f, -0.5f, 
        -0.5f, 0.0f, 0.5f, 

        0.0f, 0.5f, 0.0f, 
        -0.5f, 0.0f, 0.5f, 
        0.5f, 0.0f, 0.5f,
    };
    unsigned int indices[]{
       //bottom pyramid
       0, 1, 2, 
       3, 4, 5, 
       6, 7, 8,
       9, 10, 11, 
     
       //top pyramid
       12, 13, 14,
       15, 16, 17,
       18, 19, 20,
       21, 22, 23

    };
    float colours[]{
        //top pyramid
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        //bottom pyramid
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

    };

     glm::vec3 octohedrons[]{
       glm::vec3(-2.0f, 1.5f, -3.0f),
       glm::vec3(2.0f, 1.5f, -3.0f),
       glm::vec3(-2.0f, -1.5f, -3.0f),
       glm::vec3(2.0f, -1.5f, -3.0f),

       glm::vec3(-1.0f, 1.5f, -6.0f),
       glm::vec3(1.0f, 1.5f, -6.0f),
       glm::vec3(-1.0f, -1.5f, -6.0f),
       glm::vec3(1.0f, -1.5f, -6.0f)
    };
    
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //vertex buffer creation
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    //index buffer creation
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //colour buffer creation
    unsigned int colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(1);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Camera myCamera;
    myCamera.front = glm::vec3(0.0f, 0.0f, -2.0f);
    myCamera.position = glm::vec3(0.0f, 0.0f, 3.0f);
    myCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    /*
    ----------RENDER LOOP----------
    */
    while(!glfwWindowShouldClose(win)){

        myCamera.move(&win, &deltaTime, &lastFrame);
    
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //sets background colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears screen colour and depth buffer

        glUseProgram(myShader.ID);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f); //creates perspective viewing
        view = glm::lookAt(myCamera.position, myCamera.position + myCamera.front, myCamera.up);
        
        //pass uniforms to shader
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
        
        //render cubes
        for(unsigned int i = 0; i < sizeof(octohedrons) / sizeof(glm::vec3(0.0f)); i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, octohedrons[i]);
            float angle = 45.0f;
            if(i % 2){
                angle *= -1.5 * i;
            }
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.1f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &model[0][0]);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0); //draw our triangles
        }
        glfwSwapBuffers(win);
        glfwPollEvents(); //get input
    }
    
    //cleanup memory
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &colourBuffer);
    glDeleteProgram(myShader.ID);
    
    return 0;
}
