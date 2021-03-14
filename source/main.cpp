#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"


int selectedFig;

//vertes shader source code
const char *vertexShaderSourceTexture ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aText; \n"
    "out vec2 Text; \n"
    "uniform mat4 model; \n"
    "uniform mat4 view; \n"
    "uniform mat4 projection; \n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
    "   Text = aText; \n"
    "}\0";

// fragment shader source code
const char *fragmentShaderSourceTexture = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 Text; \n"
    "uniform sampler2D texture_var; \n"
    "void main()\n"
    "{\n"
    "       FragColor = texture(texture_var, Text);\n"
    "}\n\0";


const char *vertexShaderSourceColor ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 outColor;\n"
    "uniform mat4 model; \n"
    "uniform mat4 view; \n"
    "uniform mat4 projection; \n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
    "   outColor = aColor;\n"
    "}\0";

const char *fragmentShaderSourceColor = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 outColor;\n"
    "void main()\n"
    "{\n"
    "       FragColor = vec4(outColor.x, outColor.y, outColor.z, 1.0f);\n"
    "}\n\0";

// Camera Attributes
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int cameraSpin = 0;

glm::vec3 predecided1 = glm::vec3(3.0f, 2.0f, 3.0f);
glm::vec3 predecided2 = glm::vec3(-3.0, -2.0f, -3.0f);
glm::vec3 predecided3 = glm::vec3(4.0f, 4.0, 4.0f);

// Object Attributes
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
int objectSpin = 0;


void buttonHandler(GLFWwindow *window)
{
    // CAMERA MOTION
   const float cameraSpeed = 0.1f;
   // up
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos.y += cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos.y -= cameraSpeed;

    // left
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos.x -= cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos.x += cameraSpeed;

    // in
   if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
      cameraPos.z += cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
      cameraPos.z -= cameraSpeed;

    // OBJECT MOTION
    float objectSpeed = 0.1f;
   // up
   if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
      objectPos.y += objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
      objectPos.y -= objectSpeed;

    // left
   if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
      objectPos.x -= objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
      objectPos.x += objectSpeed;

    // in
   if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
      objectPos.z += objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
      objectPos.z -= objectSpeed;


    // SPIN OBJECT
   if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        objectSpin=1;
   if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        objectSpin=0;

    // SPIN CAMERA
   if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        cameraSpin=1;
   if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        cameraSpin=0;


   if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        cameraPos = predecided1;
        cameraFront = objectPos-cameraPos;
   }
   if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
        cameraPos= predecided2;
        cameraFront = objectPos-cameraPos;
   }
   if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        cameraPos= predecided3;
        cameraFront = objectPos-cameraPos;
   }


   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, true);
   }
}




// window size change handler
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// error handler
void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

// generate a shader from a source code
unsigned int createShader( std :: string type){
    unsigned int shader;
    // std :: cout << type << std::endl;
    if(type == "vertex")shader = glCreateShader(GL_VERTEX_SHADER);
    else shader = glCreateShader(GL_FRAGMENT_SHADER);

    if(type == "vertex"){
        if(selectedFig==2)glShaderSource(shader, 1, &vertexShaderSourceTexture, NULL);
        else glShaderSource(shader, 1, &vertexShaderSourceColor, NULL);
    }
    else {
        if(selectedFig==2)glShaderSource(shader, 1, &fragmentShaderSourceTexture, NULL);
        else glShaderSource(shader, 1, &fragmentShaderSourceColor, NULL);
    }
    glCompileShader(shader);


    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std :: cout << "ERROR : Unable to Compile " << type <<  " Shader\n" << std :: endl;
        std :: cout << infoLog << std :: endl;
        return 0;
    }
    return shader;
}

// create the program(pipeline)
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader){
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std :: cout <<  "Unable to Link Shaders\n" << std::endl;
        std :: cout << infoLog << std :: endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// creates a texture
unsigned int createTexture(){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../source/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "Unable to load texture 2 \n");
        return 0;
    }
    stbi_image_free(data);
    return texture;
}

// sets up a window using glfw
GLFWwindow* initialize() {
    int glfwInitRes = glfwInit();
    if (!glfwInitRes) {
        fprintf(stderr, "Unable to initialize GLFW\n");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "InitGL", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Unable to create GLFW window\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int gladInitRes = gladLoadGL();
    if (!gladInitRes) {
        fprintf(stderr, "Unable to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}




int main(){


    // INITIAL PROMPT
    std :: cout << "Select the figure you want to see, by entering the corresponding number!" <<  std :: endl;
    std :: cout << "1 : Hexagonal Bypyramid" << std :: endl;
    std :: cout << "2 : Unidecagonal Pyramid" << std :: endl;
    std :: cout << "3 : Elongated Square Dipyramid" << std :: endl;
    std :: cin >> selectedFig;
    if(selectedFig >3 || selectedFig <= 0){
        std :: cout << "ERROR : Invalid Input! Please enter a choice between 1 to 3!";
        return 0;
    }
    // CODE STARTS

    // The callBack functions which handle all errors thrown by the glfw
    glfwSetErrorCallback(errorCallback);

    GLFWwindow* window = initialize();
    if (!window) {
        return 0;
    }

    // Create the pipeline
    unsigned int vertexShader = createShader("vertex");
    unsigned int fragmentShader = createShader("fragment");
    unsigned int shaderProgram = createShaderProgram(vertexShader,fragmentShader);
    glUseProgram(shaderProgram);

    // // create and add the texture
    // Set up texture only for 2nd fig
    unsigned int texture;
    if(selectedFig==2){
        texture = createTexture();
        glUniform1i(glGetUniformLocation(shaderProgram, "texture_var"), 0); // weird syntax
    }


    // to make the depth shit not happen
    glEnable(GL_DEPTH_TEST);

    //CUBE & TWO TRIANGLES
    float obj3[] = {

    // THE TWO FACES
    //    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,


    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //TWO PYRAMIDS
    // 1
       0.5f, -0.5f, -0.5f,  1,1,0,
       0.5f,  0.5f, -0.5f, 1,0,1,
       0.0f,  0.0f,  -1.0f, 1,1,0,

       0.5f,  0.5f, -0.5f,  1,0,1,
       -0.5f,  0.5f, -0.5f,  1,1,0,
       0.0f,  0.0f,  -1.0f, 1,0,0,

       -0.5f,  0.5f, -0.5f, 1,0,1,
       -0.5f, -0.5f, -0.5f,  1,1,0,
       0.0f,  0.0f,  -1.0f, 1,0,0,

       -0.5f, -0.5f, -0.5f,  1,0,1,
       0.5f, -0.5f, -0.5f,  1,1,0,
       0.0f,  0.0f,  -1.0f, 1,0,1,

    // 2
       0.5f, -0.5f, 0.5f,  1,0,1,
       0.5f,  0.5f, 0.5f,  1,0,1,
       0.0f,  0.0f,  1.0f, 1,1,0,

       0.5f,  0.5f, 0.5f, 1,1,0,
       -0.5f,  0.5f, 0.5f, 1,0,1,
       0.0f,  0.0f,  1.0f, 1,1,0,

       -0.5f,  0.5f, 0.5f,  1,0,1,
       -0.5f, -0.5f, 0.5f,  1,1,0,
       0.0f,  0.0f,  1.0f, 1,0,1,

       -0.5f, -0.5f, 0.5f, 1,1,0,
       0.5f, -0.5f, 0.5f,  1,0,1,
       0.0f,  0.0f,  1.0f, 1,1,0,

    //REAMING SQUARES
        -0.5f,  0.5f,  0.5f,  0,1,0,
        -0.5f,  0.5f, -0.5f, 0,1,0,
        -0.5f, -0.5f, -0.5f, 0,1,0,
        -0.5f, -0.5f, -0.5f,  0,1,0,
        -0.5f, -0.5f,  0.5f,  0,1,0,
        -0.5f,  0.5f,  0.5f, 0,1,0,

        0.5f,  0.5f,  0.5f, 0,0,1,
        0.5f,  0.5f, -0.5f,  0,0,1,
        0.5f, -0.5f, -0.5f,  0,0,1,
        0.5f, -0.5f, -0.5f,  0,0,1,
        0.5f, -0.5f,  0.5f,  0,0,1,
        0.5f,  0.5f,  0.5f,  0,0,1,

        -0.5f, -0.5f, -0.5f,  1,0,1,
        0.5f, -0.5f, -0.5f, 1,0,1,
        0.5f, -0.5f,  0.5f,  1,0,1,
        0.5f, -0.5f,  0.5f,  1,0,1,
        -0.5f, -0.5f,  0.5f, 1,0,1,
        -0.5f, -0.5f, -0.5f,  1,0,1,

        -0.5f,  0.5f, -0.5f,  0,1,1,
        0.5f,  0.5f, -0.5f,  0,1,1,
        0.5f,  0.5f,  0.5f, 0,1,1,
        0.5f,  0.5f,  0.5f,  0,1,1,
        -0.5f,  0.5f,  0.5f,  0,1,1,
        -0.5f,  0.5f, -0.5f, 0,1,1,
    };

    // UNIDECAGONAL PRISM
    float obj2[] = {
        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        1.000f, 0.000f, 0.000f, 0.05f, 1.000f,
        0.841f, 0.541f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f,0.00f, 0.000f,
        0.841f, 0.541f, 0.000f,0.05f, 1.000f,
        0.415f, 0.910f, 0.000f,1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        0.415f, 0.910f, 0.000f, 0.05f, 1.000f,
        -0.142f, 0.990f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        -0.142f, 0.990f, 0.000f, 0.05f, 1.000f,
        -0.655f, 0.756f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        -0.655f, 0.756f, 0.000f, 0.05f, 1.000f,
        -0.959f, 0.282f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        -0.959f, 0.282f, 0.000f, 0.05f, 1.000f,
        -0.959f, -0.282f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        -0.959f, -0.282f, 0.000f, 0.05f, 1.000f,
        -0.655f, -0.756f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        -0.655f, -0.756f, 0.000f, 0.05f, 1.000f,
        -0.142f, -0.990f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        -0.142f, -0.990f, 0.000f, 0.05f, 1.000f,
        0.415f, -0.910f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        0.415f, -0.910f, 0.000f, 0.05f, 1.000f,
        0.841f, -0.541f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 0.000f, 0.00f, 0.000f,
        0.841f, -0.541f, 0.000f, 0.05f, 1.000f,
        1.000f, 0.000f, 0.000f, 1.000f, 0.005f,




        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        1.000f, 0.000f, 0.000f, 0.05f, 1.000f,
        0.841f, 0.541f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        0.841f, 0.541f, 0.000f, 0.05f, 1.000f,
        0.415f, 0.910f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        0.415f, 0.910f, 0.000f, 0.05f, 1.000f,
        -0.142f, 0.990f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        -0.142f, 0.990f, 0.000f, 0.05f, 1.000f,
        -0.655f, 0.756f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        -0.655f, 0.756f, 0.000f, 0.05f, 1.000f,
        -0.959f, 0.282f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        -0.959f, 0.282f, 0.000f, 0.05f, 1.000f,
        -0.959f, -0.282f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        -0.959f, -0.282f, 0.000f, 0.05f, 1.000f,
        -0.655f, -0.756f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        -0.655f, -0.756f, 0.000f, 0.05f, 1.000f,
        -0.142f, -0.990f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        -0.142f, -0.990f, 0.000f, 0.05f, 1.000f,
        0.415f, -0.910f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        0.415f, -0.910f, 0.000f, 0.05f, 1.000f,
        0.841f, -0.541f, 0.000f, 1.000f, 0.005f,


        0.000f, 0.000f, 1.000f, 0.00f, 0.000f,
        0.841f, -0.541f, 0.000f, 0.05f, 1.000f,
        1.000f, 0.000f, 0.000f, 1.000f, 0.005f
    };


    // HEXAGONAL BIPYRAMID
    float obj1[]= {

        0.000f, 0.000f, 1.000f, 0.000f, 0.000f, 1.000f,
        1.000f, 0.000f, 0.000f,0.000f, 0.000f, 1.000f,
        0.500f, 0.866f, 0.000f,0.000f, 0.000f, 1.000f,


        0.000f, 0.000f, 1.000f, 0.000f, 1.000f, 0.000f,
        0.500f, 0.866f, 0.000f,0.000f, 1.000f, 0.000f,
        -0.500f, 0.866f, 0.000f,0.000f, 1.000f, 0.000f,


        0.000f, 0.000f, 1.000f, 0.000f, 1.000f, 1.000f,
        -0.500f, 0.866f, 0.000f,0.000f, 1.000f, 1.000f,
        -1.000f, 0.000f, 0.000f,0.000f, 1.000f, 1.000f,


        0.000f, 0.000f, 1.000f, 0.000f, 0.000f, 1.000f,
        -1.000f, 0.000f, 0.000f,0.000f, 0.000f, 1.000f,
        -0.500f, -0.866f, 0.000f,0.000f, 0.000f, 1.000f,


        0.000f, 0.000f, 1.000f, 0.000f, 1.000f, 0.000f,
        -0.500f, -0.866f, 0.000f,0.000f, 1.000f, 0.000f,
        0.500f, -0.866f, 0.000f,0.000f, 1.000f, 0.000f,


        0.000f, 0.000f, 1.000f, 0.000f, 1.000f, 1.000f,
        0.500f, -0.866f, 0.000f,0.000f, 1.000f, 1.000f,
        1.000f, 0.000f, 0.000f,0.000f, 1.000f, 1.000f,


        0.000f, 0.000f, -1.000f, 0.000f, 0.000f, 1.000f ,
        1.000f, 0.000f, 0.000f,0.000f, 0.000f, 1.000f,
        0.500f, 0.866f, 0.000f,0.000f, 0.000f, 1.000f,


        0.000f, 0.000f, -1.000f, 0.000f, 1.000f, 0.000f,
        0.500f, 0.866f, 0.000f,0.000f, 1.000f, 0.000f,
        -0.500f, 0.866f, 0.000f,0.000f, 1.000f, 0.000f,


        0.000f, 0.000f, -1.000f, 0.000f, 1.000f, 1.000f,
        -0.500f, 0.866f, 0.000f,0.000f, 1.000f, 1.000f,
        -1.000f, 0.000f, 0.000f,0.000f, 1.000f, 1.000f,


        0.000f, 0.000f, -1.000f, 0.000f, 0.000f, 1.000f,
        -1.000f, 0.000f, 0.000f,0.000f, 0.000f, 1.000f,
        -0.500f, -0.866f, 0.000f,0.000f, 0.000f, 1.000f,


        0.000f, 0.000f, -1.000f, 0.000f, 1.000f, 0.000f,
        -0.500f, -0.866f, 0.000f,0.000f, 1.000f, 0.000f,
        0.500f, -0.866f, 0.000f,0.000f, 1.000f, 0.000f,


        0.000f, 0.000f, -1.000f, 0.000f, 0.000f, 1.000f,
        0.500f, -0.866f, 0.000f,0.000f, 1.000f, 1.000f,
        1.000f, 0.000f, 0.000f,0.000f, 1.000f, 1.000f

    };


    // set total vertices
    int totalVertices = 0;
    if(selectedFig==1) totalVertices = 36;
    else if(selectedFig==2) totalVertices = 66;
    else totalVertices = 60;

    // Local Space Set Up
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if(selectedFig==1)glBufferData(GL_ARRAY_BUFFER, sizeof(obj1), obj1,GL_STATIC_DRAW);
    if(selectedFig==2)glBufferData(GL_ARRAY_BUFFER, sizeof(obj2), obj2,GL_STATIC_DRAW);
    if(selectedFig==3)glBufferData(GL_ARRAY_BUFFER, sizeof(obj3), obj3,GL_STATIC_DRAW);

    if(selectedFig==2)glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    else glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // set the Attribute Pointer correctly
    if(selectedFig==2)glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
    else glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);


    // Set up 3D

    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);


    // Add uniforms to the shaderProgram
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));





    // white screen
    glClearColor(1, 1, 1, 1);


    while (!glfwWindowShouldClose(window)) {
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        buttonHandler(window);


        // dont know why we need to do this in loop for adding the texture in the 2nd fig
        if(selectedFig==2){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        // have to bind VAO again before drawing
        glBindVertexArray(VAO);

        glm::vec3 target = cameraPos + cameraFront;
        // moving view(CAMERA MOVEMENT)
        glm::mat4 view;
        if(!cameraSpin)view = glm::lookAt(cameraPos, target, cameraUp);
        else{
            float radius = glm::length(objectPos - cameraPos);
            float camX   = sin((float)glfwGetTime() * glm::radians(50.0f)) * radius;
            float camZ   = cos((float)glfwGetTime() * glm::radians(50.0f)) * radius;
            view = glm::lookAt( glm::vec3(camX, 0.0f, camZ) + objectPos, objectPos, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // moving model(OBJECT MOVEMENT)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, objectPos);
        if(objectSpin)model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),glm::vec3(0.1f, 0.0f, 0.0f));
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // draw the array
        glDrawArrays(GL_TRIANGLES, 0, totalVertices);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}