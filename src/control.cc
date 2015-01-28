#include "control.hh"

extern GLFWwindow* window;

void Control::computeMVP()
{
    float FoV = initialFoV;
    double mousePosX = 0;
    double mousePosY = 0;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    glfwSetCursorPos(window, widthPixel / 2, heightPixel / 2);

    horizontalAngle += mouseSpeed * float(widthPixel / 2 - mousePosX);
    verticalAngle += mouseSpeed * float(heightPixel / 2 - mousePosY);

    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
        );

    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
        );

    glm::vec3 up = glm::cross(right, direction);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }


    proj = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);

    view = glm::lookAt(
        position,           
        position + direction, 
        up                  
        );


    lastTime = currentTime;
}

glm::mat4 Control::getProjMatrix()
{
    return proj;
}

glm::mat4 Control::getViewMatrix()
{
    return view;
}

glm::mat4 Control::getModMatrix()
{
    return mod;
}

glm::mat4 Control::getMVP()
{
    return proj * view * mod;
}
