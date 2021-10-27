#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


//holds the source code for each shader
struct ShaderSource{
    std::string vertexSource;
    std::string fragmentSource;
};


static unsigned int CompileShader(const std::string &source, unsigned int type){
    
    const char* src = source.c_str(); //returns a pointer to source

    unsigned int shaderID = glCreateShader(type); //generates a shader ID
    glShaderSource(shaderID, 1, &src, nullptr); //fills source code for desired shader
    glCompileShader(shaderID); //compiles shader

    //ensures succesful compilation
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(shaderID);
        exit(EXIT_FAILURE);
    }

    return shaderID; 
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader){

    unsigned int program = glCreateProgram(); //creates current shader program
    unsigned int vShader = CompileShader(vertexShader, GL_VERTEX_SHADER); //compiles vertex source
    unsigned int fShader = CompileShader(fragmentShader, GL_FRAGMENT_SHADER); //compiles fragment source

    //attaches shaders to program
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    //links the shader program
    glLinkProgram(program);
    //ensures successful linking
    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}

static ShaderSource ParseShader(const std::string &filePath){

    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::ifstream stream(filePath); //gets current input stream
    std::string line; //string to hold each line from file
    std::stringstream ss[2];

    while(getline(stream, line)){
    
        if(line.find("#shader") != std::string::npos){ //finds "#shader" in file
           if(line.find("vertexShader") != std::string::npos){ //the current line holds "vertexShader" set type 
               type = ShaderType::VERTEX;
           }
           else if(line.find("fragmentShader") != std::string::npos){//the current line holds "fragmentShader" set type 
               type = ShaderType::FRAGMENT;
           }    
        }
        else{
            ss[(int)type] << line << '\n'; //push the line into the given array based on the index
        }
    }

    return {ss[0].str(), ss[1].str()};


}

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
    GLFWwindow *win = glfwCreateWindow(width, height, "Cube", NULL, NULL); //creates current window
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
    ShaderSource source = ParseShader("shader.shader"); //creates source code for shaders
    unsigned int program = CreateShader(source.vertexSource, source.fragmentSource); //creates shader program
   

    
    /*----------BUFFER CREATION----------*/
    float vertices[]{
      //front face
      -0.5f, 0.0f, 0.0f, //0
      0.0f, 0.5f, 0.0f, //1
      -0.5f, 0.5f, 0.0f, //2
      0.0f, 0.0f, 0.0f, //3

      //right face
      0.0f, 0.5f, 0.0f, //4
      0.0f, 0.0f, 0.0f, //5
      0.0f, 0.0f, 0.5f, //6
      0.0f, 0.5f, 0.5f, //7

      //back face
      0.0f, 0.5f, 0.5f, //8
      0.0f, 0.0f, 0.5f, //9
      -0.5f, 0.0f, 0.5f, //10
      -0.5f, 0.5f, 0.5f, //11

      //left face
      -0.5f, 0.5f, 0.0f, //12
      -0.5f, 0.5f, 0.5f, //13
      -0.5f, 0.0f, 0.5f, //14
      -0.5f, 0.0f, 0.0f,  //15

      //top face
      0.0f, 0.5f, 0.0f, //16
      0.0f, 0.5f, 0.5f, //17
      -0.5f, 0.5f, 0.5f, //18
      -0.5f, 0.5f, 0.0f, //19

      //bottom face
      0.0f, 0.0f, 0.0f, //20
      -0.5f, 0.0f, 0.5f, //21
      0.0f, 0.0f, 0.5f, //22
      -0.5f, 0.0f, 0.0f  //23

      
    };
    unsigned int indices[]{
       //front face
       0, 1, 2,
       1, 0, 3,

       //right face
       4, 5, 6,
       6, 7, 4,

       //back face
       8, 9, 10, 
       10, 11, 8, 

       //left face
       12, 13, 14,
       14, 15, 12,

       //top face
       16, 17, 18, 
       18, 19, 16,

       //bottom face
       20, 21, 22, 
       20, 23, 21

    };
    float colours[]{
       //front face
       1.0f, 0.0f, 0.0f,
       1.0f, 0.0f, 0.0f, 
       1.0f, 0.0f, 0.0f,
       1.0f, 0.0f, 0.0f,

       //right face
       0.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f,

       //back face
       1.0f, 1.0f, 0.0f, 
       1.0f, 1.0f, 0.0f,
       1.0f, 1.0f, 0.0f,
       1.0f, 1.0f, 0.0f,

       //left face
       0.0f, 1.0f, 0.0f, 
       0.0f, 1.0f, 0.0f,
       0.0f, 1.0f, 0.0f,
       0.0f, 1.0f, 0.0f,

       //top face
       1.0f, 0.0f, 1.0f,
       1.0f, 0.0f, 1.0f,
       1.0f, 0.0f, 1.0f,
       1.0f, 0.0f, 1.0f,

       //bottom face
       1.0f, 0.5f, 0.0f, 
       1.0f, 0.5f, 0.0f,
       1.0f, 0.5f, 0.0f,
       1.0f, 0.5f, 0.0f,
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

    //colour nuffer creation
    unsigned int colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(1);

    /*
    ----------RENDER LOOP----------
    */
    while(!glfwWindowShouldClose(win)){

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f); //sets background colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears screen colour and depth buffer

        glUseProgram(program);
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f); //creates perspective viewing
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 0.5f, 0.5f)); //rotates model
        
        //sets uniforms
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
        
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0); //draw our triangles
        glfwSwapBuffers(win);
        glfwPollEvents(); //get input
    }
    
    //cleanup memory
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &colourBuffer);
    glDeleteProgram(program);
    
    return 0;
}
