#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <stdio.h>
#include <thread>
#include <string>
#include <unistd.h>

#include "textrenderer.h"
#include "logger.h"
#include "controls.h"
#include "utils.h"
#include "shader.h"

//config
bool guitext = true;
int screenWidth=960, screenHeight=540;

/* ---------- */

float vertices[] = {
    1.0f,  1.0f, 0.0f,  // top right
    1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};
unsigned int VBO, VAO, EBO;

void renderScene(const Shader &shader);

static void error_callback(int error, const char* description)
{
    Logger("Error: "+(std::string)description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    keycallback(window,key,scancode,action,mods);
}

static void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn){
    cursorpositioncallback(window,xposIn,yposIn);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    //focus on the window when clicked on, esc to re-enable cursor
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    glfwGetWindowSize(window,&screenWidth,&screenHeight);
}

void rendertext(GLFWwindow* window, Shader program){
    if(guitext){
        std::string fpstring="FPS "+std::to_string(fps).substr(0,4);
        char zm[16];
        sprintf(zm,"%.0f",zoom);
        std::string zmstr="Zoom: ";
        zmstr+=zm;
        std::string coordstring=std::to_string(posx)+", "+std::to_string(posy);
        textRenderer(fpstring.c_str(),32.0,-32.0,window,32,1.0,1.0,1.0,topLeft,program);
        textRenderer(zmstr.c_str(),32.0,-96.0,window,32,1.0,1.0,1.0,topLeft,program);
        textRenderer(coordstring.c_str(),32.0,-160.0,window,32,1.0,1.0,1.0,topLeft,program);
    }
}

GLFWwindow* window;

int main(void)
{
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    Logger("Program Initialized");

    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
    {
        Logger("glfwInit Failed");
        exit(EXIT_FAILURE);
    }
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
 
    window = glfwCreateWindow(screenWidth, screenHeight, "Graphics Simulator", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        Logger("glfwCreateWindow Failed");
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
 
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger("Failed to initialize GLAD");
        return -1;
    }
    glfwSwapInterval(1);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Shader mainShader("main.vert","main.frag");
    Shader textShader("2dtext.vert","2dtext.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    //textrenderer
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glGenBuffers(1, &textEBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    fontTexture=loadTexture("../font/bitmapfont.bmp",GL_NEAREST_MIPMAP_NEAREST,GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        double gameTime=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()%100000000;

        loopTimer();

        keyActions();

        glClearColor(0.0f, 0.35f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        
        renderScene(mainShader);

        rendertext(window,textShader);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
 
    glfwTerminate();
    Logger("Window Closed");
    exit(EXIT_SUCCESS);
}

void renderScene(const Shader &shader){
    shader.use();
    shader.setVec2("screensize",screenWidth,screenHeight);
    shader.setDVec2("coords",posx,posy);
    shader.setDouble("zoom",zoom);
    shader.setFloat("itr",128);

    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0,6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}