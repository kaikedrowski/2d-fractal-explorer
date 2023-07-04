#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "utils.h"

float zoom=1.0;
float posx=0.0;
float posy=0.0;

bool firstMouse = true;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

bool key_forward=false;//w
bool key_left=false;//a
bool key_back=false;//s
bool key_right=false;//d
bool key_up=false;//space
bool key_down=false;//lshift

enum camera_mode {camera_mode_fps, camera_mode_three_dimensional};

int camera_mode=camera_mode_fps;

static inline void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action==GLFW_PRESS)
        switch(key){
            case GLFW_KEY_W:
                key_forward=true;
                break;
            case GLFW_KEY_A:
                key_left=true;
                break;
            case GLFW_KEY_S:
                key_back=true;
                break;
            case GLFW_KEY_D:
                key_right=true;
                break;
            case GLFW_KEY_SPACE:
                key_up=true;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                key_down=true;
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    if(action==GLFW_RELEASE)
        switch(key){
            case GLFW_KEY_W:
                key_forward=false;
                break;
            case GLFW_KEY_A:
                key_left=false;
                break;
            case GLFW_KEY_S:
                key_back=false;
                break;
            case GLFW_KEY_D:
                key_right=false;
                break;
            case GLFW_KEY_SPACE:
                key_up=false;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                key_down=false;
                break;
        }
}

static inline void keyActions() {
    if(key_forward)zoom*=1.1;
    if(key_back)zoom*=.9;
    if(key_left)posx-=0.1/(zoom);
    if(key_right)posx+=0.1/(zoom);
    if(key_up)posy+=0.1/(zoom);
    if(key_down)posy-=0.1/(zoom);
}

static inline void cursorpositioncallback(GLFWwindow* window, double xposIn, double yposIn){
    if(glfwGetInputMode(window,GLFW_CURSOR)==GLFW_CURSOR_NORMAL)return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    //yaw += xoffset;
    //pitch += yoffset;
}

#endif /* CONTROLS_H */