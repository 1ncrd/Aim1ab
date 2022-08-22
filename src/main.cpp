#include <iostream>
#include <filesystem>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../inc/Shader.h"
#include "../inc/Camera.h"
#include "../inc/DirectLight.h"
#include "../inc/Sphere.h"
#include "../inc/Cube.h"
#include "../inc/MyPrinter.h"
#include "../inc/Crosshair.h"

void getMonitorResolution();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouseMoveCallback(GLFWwindow *window, double xposIn, double yposIn);
void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void updateDeltaTime();
GLuint loadTexture(const std::string &path);

void renderTriangle(const glm::vec3 &vertex_0, const glm::vec3 &vertex_1, const glm::vec3 &vertex_2, const glm::vec3 &color, const Shader &shader);

// screen
unsigned int screenWidth = 1980;
unsigned int screenHeight = 1080;

// path
const std::filesystem::path rootPath = std::filesystem::current_path();
const std::filesystem::path incPath = rootPath / "inc";
const std::filesystem::path resPath = rootPath / "res";
const std::filesystem::path srcPath = rootPath / "src";
const std::filesystem::path shaderPath = srcPath / "shader";

// direct light
const glm::vec3 DIRECTLIGHT_DIRECTION_DEFAULT(-2, -3, -3);
const glm::vec3 DIRECTLIGHT_AMBIENT_DEFAULT(0.3f, 0.3f, 0.3f);
const glm::vec3 DIRECTLIGHT_DIFFUSE_DEFAULT(0.4f, 0.4f, 0.4f);
const glm::vec3 DIRECTLIGHT_SPECULAR_DEFAULT(0.1f, 0.1f, 0.1f);

DirectLight directLight(DIRECTLIGHT_DIRECTION_DEFAULT, DIRECTLIGHT_AMBIENT_DEFAULT, DIRECTLIGHT_DIFFUSE_DEFAULT, DIRECTLIGHT_SPECULAR_DEFAULT);


// camera args
const glm::vec3 CAMERA_POS_DEFAULT(20.0f, 1.0f, 18.0f);
const glm::vec3 CAMERA_FRONT_DEFAULT(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP_DEFAULT(0.0f, 1.0f, 0.0f);

// create camera
Camera camera(CAMERA_POS_DEFAULT, CAMERA_FRONT_DEFAULT);

// track the change of time
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

// background color
const glm::vec4 BACKGROUND_COLOR(133 / 255.0f, 204 / 255.0f, 255 / 255.0f, 1.0f);

Shader triangleShader((shaderPath / "triangle.vert").string(), (shaderPath / "triangle.frag").string());

glm::vec3 wallColor(255 / 255.0, 229 / 255.0, 204 / 255.0);
glm::vec3 sphereColor(0 / 255.0, 255 / 255.0, 255 / 255.0);
float radius = 1.0f;
Sphere spheres[] = {
        Sphere(glm::vec3(-1.0), radius, sphereColor, triangleShader, camera, directLight, 64),
        Sphere(glm::vec3(-1.0), radius, sphereColor, triangleShader, camera, directLight, 64),
        Sphere(glm::vec3(-1.0), radius, sphereColor, triangleShader, camera, directLight, 64),
};

int hitTimes = 0;
int clickTimes = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    getMonitorResolution();
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // no border
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Aim1ab", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseClickCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ---------------------------------------------------------------------------------------------------------
    camera.setAspect((float)screenWidth / screenHeight);

    glEnable(GL_DEPTH_TEST);
    // build and compile our shader program
    // ------------------------------------
    triangleShader.init();
    Shader boxShader((shaderPath / "box.vert").string(), (shaderPath / "box.frag").string());
    boxShader.init();
    Shader lightingCubeShader((shaderPath / "light_cube.vert").string(), (shaderPath / "light_cube.frag").string());
    lightingCubeShader.init();
    Shader textShader((shaderPath / "character.vert").string(), (shaderPath / "character.frag").string());
    textShader.init();
    Shader crosshairShader((shaderPath / "crosshair.vert").string(), (shaderPath / "crosshair.frag").string());
    crosshairShader.init();

    MyPrinter printer(std::filesystem::path("C:/Windows/Fonts/consola.ttf").string().c_str(), textShader, screenWidth, screenHeight);

    for (auto &sphere : spheres)
    {
        sphere.init();
        sphere.setGridPos();
    }

    Cube cubes[] = {
    Cube(glm::vec3(0.0f), 40.0f, 0.01f, 20.0f, wallColor * 1.2f, triangleShader, camera, directLight), // floor
    Cube(glm::vec3(0.0f), 40.0f, 18.0f, 0.01f, wallColor, triangleShader, camera, directLight), // back wall
    Cube(glm::vec3(0.0f), 0.01f, 18.0f, 20.0f, wallColor, triangleShader, camera, directLight), // left wall
    Cube(glm::vec3(40.0f, 0.0f, 0.0f), 0.01f, 18.0f, 20.0f, wallColor, triangleShader, camera, directLight), // right wall
    };

    Crosshair crosshair(crosshairShader, 10.0, glm::vec3(1.0f, 0.0f, 0.0f), screenWidth, screenHeight);
    crosshair.init();

    // render loop
    // -----------
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_COLOR.w);
    float startTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        updateDeltaTime();
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &sphere : spheres)
        {
            sphere.renderSphere();
        }

        for (auto &cube : cubes)
        {
            cube.renderCube();
        }

        crosshair.renderCrosshair();

        // display
        float gameTime = glfwGetTime() - startTime;
        static float fpsLastTime = glfwGetTime();
        static float fps = 0;
        if (glfwGetTime() - fpsLastTime > 1)
        {
            fpsLastTime = glfwGetTime();
            fps = 1 / deltaTime;
        }

        float acc = 0;
        if (clickTimes > 0)
        {
            acc = (float)hitTimes / clickTimes;
        }

        float KPM = 0;
        if (gameTime > 0)
        {
            KPM = hitTimes / gameTime * 60;
        }
        printer.renderText(std::format("FPS         : {:.1f}", fps), 10.0f, screenHeight - 40.0, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
        printer.renderText(std::format("Time        : {:.1f}", gameTime), 10.0f, screenHeight - 60.0, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
        printer.renderText(std::format("hitTimes    : {:d}", hitTimes), 10.0f, screenHeight - 80.0, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
        printer.renderText(std::format("Accurancy   : {:.1f}%", acc * 100), 10.0f, screenHeight - 100.0, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
        printer.renderText(std::format("KPM         : {:.1f}", KPM), 10.0f, screenHeight - 120.0, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

        printer.renderText(std::string("PRESS ESC TO QUIT"), 10.0f, 25, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // press ESC to quit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.bodyMove(Camera::Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.bodyMove(Camera::Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.bodyMove(Camera::Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.bodyMove(Camera::Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.bodyMove(Camera::Movement::WORLD_UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.bodyMove(Camera::Movement::WORLD_DOWN, deltaTime);


}

void updateDeltaTime()
{
    float currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
}

GLuint loadTexture(const std::string &path)
{
    GLuint texture;
    glGenTextures(1, &texture);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
        {
            std::cout << "Image file format unexpected";
            return -1;
        }

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // mipmap automatically
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
        return -1;
    }
    stbi_image_free(data);
    return texture;
}

// the normal will be calculate from normal(cross(vertex_1 - vertex_0, vertex_2 - vertex_0))
void renderTriangle(const glm::vec3 &vertex_0, const glm::vec3 &vertex_1, const glm::vec3 &vertex_2, const glm::vec3 &color, const Shader &shader)
{
    shader.use();
    // camera
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getPersMatrix());
    shader.setVec3("cameraPos", camera.getPosition());

    // direct light
    shader.setVec3("directLight.direction", directLight.direction);
    shader.setVec3("directLight.ambient", directLight.ambient);
    shader.setVec3("directLight.diffuse", directLight.diffuse);
    shader.setVec3("directLight.specular", directLight.specular);

    // material
    shader.setFloat("material.shininess", 64);
    shader.setVec3("aColor", color);

    // ---------------

    glm::vec3 normal = glm::normalize(glm::cross(vertex_1 - vertex_0, vertex_2 - vertex_0));
    float vertices[] = {
        // vertex                           // color                    // normal
        vertex_0.x, vertex_0.y, vertex_0.z, normal.x, normal.y, normal.z,
        vertex_1.x, vertex_1.y, vertex_1.z, normal.x, normal.y, normal.z,
        vertex_2.x, vertex_2.y, vertex_2.z, normal.x, normal.y, normal.z,
    };
    GLuint VBO, triangleVAO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glBindVertexArray(triangleVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(NULL);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &triangleVAO);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouseMoveCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    static bool firstMouse = true;
    static float lastX = static_cast<float>(screenWidth / 2.0);
    static float lastY = static_cast<float>(screenHeight / 2.0);

    float xPos = static_cast<float>(xposIn);
    float yPos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = -(yPos - lastY); // 鼠标向下移动时，yposIn 反而增大
    lastX = xPos;
    lastY = yPos;

    camera.persMove(xOffset, yOffset);
}

void mouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
    // hit judgement
    if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS)
    {
        clickTimes++;
        for (auto &sphere : spheres)
        {
            glm::vec3 c = sphere.getCenter() - camera.getPosition();
            glm::vec3 f = camera.getFront();
            float d = glm::dot(c, f) / glm::length(f);
            float h_2 = length(c) * length(c) - d * d;
            float r = sphere.getRadius();
            if (h_2 < r * r)
            {
                hitTimes++;
                sphere.setGridPos();
            }
        }
    }
};

void getMonitorResolution()
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    screenWidth = mode->width;
    screenHeight = mode->height;
}