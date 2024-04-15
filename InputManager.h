#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include "Camera.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class InputManager {
public:
    Camera* camera;
    bool firstMouse;
    float lastX, lastY;

    InputManager(Camera* camera) : camera(camera), firstMouse(true), lastX(400), lastY(300) {}

    void ProcessKeyboard(GLFWwindow* window, float deltaTime) {
        float velocity = camera->MovementSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->Position += camera->Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->Position -= camera->Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->Position -= camera->Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->Position += camera->Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera->Position += camera->Up * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            camera->Position -= camera->Up * velocity;
            std::cout << camera->Position.s << std::endl;
            //Confirming that the camera is actually moving
        }
    }

    void ProcessMouseMovement(GLFWwindow* window, double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= camera->MouseSensitivity;
        yoffset *= camera->MouseSensitivity;

        camera->Yaw += xoffset;
        camera->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (camera->Pitch > 89.0f)
            camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f)
            camera->Pitch = -89.0f;

        camera->updateCameraVectors();
    }

    void ProcessMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
        camera->Zoom -= (float)yoffset;
        if (camera->Zoom < 1.0f)
            camera->Zoom = 1.0f;
        if (camera->Zoom > 45.0f)
            camera->Zoom = 45.0f;
    }

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (manager) {
            manager->ProcessMouseMovement(window, xpos, ypos);
        }
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (manager) {
            manager->ProcessMouseScroll(window, xoffset, yoffset);
        }
    }

    // Call this function in your main program after creating the InputManager instance
    void SetupCallbacks(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, InputManager::MouseCallback);
        glfwSetScrollCallback(window, InputManager::ScrollCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
};

#endif
