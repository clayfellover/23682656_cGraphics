#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);

// time stuff
float lastFrame = 0.0f;
bool running = false;

// mouse input stuff
float lastX = 1024.0f / 2.0f;
float lastY = 768.0f / 2.0f;
bool firstMouse = true;
Camera* cameraPtr = nullptr;

// mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float sensitivity = 0.05f;
    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);
    xoffset *= sensitivity; 
    yoffset *= sensitivity; 
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    if (cameraPtr)
        cameraPtr->processMouseMovement(xoffset, yoffset);
}

int main( void )
{
    // GLFW / GLEW Setup 

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================

    // Define vertex positions
    const float vertices[] = {
        // Front
        -1, -1,  1,   1, -1,  1,   1,  1,  1,   -1,  1,  1,
        // Back
        -1, -1, -1,   1, -1, -1,   1,  1, -1,   -1,  1, -1,
        // Left
        -1, -1, -1,  -1, -1,  1,  -1,  1,  1,  -1,  1, -1,
        // Right
         1, -1, -1,   1, -1,  1,   1,  1,  1,   1,  1, -1,
         // Bottom
         -1, -1, -1,   1, -1, -1,   1, -1,  1,  -1, -1,  1,
         // Top
         -1,  1, -1,   1,  1, -1,   1,  1,  1,  -1,  1,  1,
    };

    // Define texture coordinates
    const float uv[] = {
        // 6 faces, 4 vertices per face
        0, 0,  1, 0,  1, 1,  0, 1,  
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
    };

    // Normals 
    const float normals[] = {
         0,0,1,  0,0,1,  0,0,1,  0,0,1,
         0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
        -1,0,0, -1,0,0, -1,0,0, -1,0,0,
         1,0,0,  1,0,0,  1,0,0,  1,0,0,
         0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
         0,1,0,  0,1,0,  0,1,0,  0,1,0
    };

    // Define indices
    const unsigned int indices[] = {
       0, 1, 2,  2, 3, 0,       // front
       4, 5, 6,  6, 7, 4,       // back
       8, 9,10, 10,11, 8,       // left
      12,13,14, 14,15,12,       // right
      16,17,18, 18,19,16,       // bottom
      20,21,22, 22,23,20        // top
    };

    // object
    struct Object {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        float angle;
        std::string name;
    };

    // Load and Use Shaders
    GLuint shaderProgram = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderProgram);

    // Create VAO
    GLuint VAO; 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create VBO 
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 0)
    glEnableVertexAttribArray(0);

    // Create texture buffer
    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 2)
    glEnableVertexAttribArray(2);

    // normals 
    unsigned int normalBuffer; 
    glGenBuffers(1, &normalBuffer); 
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); 
    glEnableVertexAttribArray(3); // layout(location = 3)

    // vertex colours
    const float colours[] = {
        1,0,0, 1,0,0, 1,0,0, 1,0,0, 
        0,1,0, 0,1,0, 0,1,0, 0,1,0, 
        0,0,1, 0,0,1, 0,0,1, 0,0,1, 
        1,1,0, 1,1,0, 1,1,0, 1,1,0, 
        1,0,1, 1,0,1, 1,0,1, 1,0,1, 
        0,1,1, 0,1,1, 0,1,1, 0,1,1 
    };

    // Create colour buffer
    unsigned int colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // layout(location = 1)
    glEnableVertexAttribArray(1);

    // EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Load texture
    GLuint texture;
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../assets/cobblestone.png", &width, &height, &nChannels, 0);
    if (data) {
        GLenum format = (nChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);

    // Set texture uniform
    glUseProgram(shaderProgram); 
    glUniform1i(glGetUniformLocation(shaderProgram, "textureMap"), 0); 
 
    // Load normal map
    GLuint normalMap; 
    glGenTextures(1, &normalMap); 
    glBindTexture(GL_TEXTURE_2D, normalMap); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int normalWidth, normalHeight, normalChannels; 
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* normalData = stbi_load("../assets/pathstone_normalmap.png", &normalWidth, &normalHeight, &normalChannels, 0); 
    if (normalData) { 
        GLenum normalFormat = (normalChannels == 4) ? GL_RGBA : GL_RGB; 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, normalWidth, normalHeight, 0, normalFormat, GL_UNSIGNED_BYTE, normalData); 
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    else {
        std::cout << "Failed to load normal map\n"; 
    }
    stbi_image_free(normalData); 

    // Load diffuse map 
    GLuint diffuseMap; 
    glGenTextures(1, &diffuseMap); 
    glBindTexture(GL_TEXTURE_2D, diffuseMap); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int diffuseWidth, diffuseHeight, diffuseChannels; 
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* diffuseData = stbi_load("../assets/pathstone_diffusemap.png", &diffuseWidth, &diffuseHeight, &diffuseChannels, 0); 
    if (diffuseData) { 
        GLenum diffuseFormat = (diffuseChannels == 4) ? GL_RGBA : GL_RGB; 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, diffuseWidth, diffuseHeight, 0, diffuseFormat, GL_UNSIGNED_BYTE, diffuseData); 
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    else { 
        std::cout << "Failed to load diffuse map\n"; 
    }
    stbi_image_free(diffuseData); 

    // Load specular map
    GLuint specularMap; 
    glGenTextures(1, &specularMap); 
    glBindTexture(GL_TEXTURE_2D, specularMap); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    int specularWidth, specularHeight, specularChannels; 
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* specularData = stbi_load("../assets/pathstone_specularmap.png", &specularWidth, &specularHeight, &specularChannels, 0);
    if (specularData) { 
        GLenum specularFormat = (specularChannels == 4) ? GL_RGBA : GL_RGB; 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, specularWidth, specularHeight, 0, specularFormat, GL_UNSIGNED_BYTE, specularData); 
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    else { 
        std::cout << "Failed to load specular map\n"; 
    }
    stbi_image_free(specularData); 

    // Camera setup
    Camera camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -2.0f)); 
    cameraPtr = &camera; 

    // Set pitch and yaw based on initial direction
    camera.yaw = -90.0f; 
    camera.pitch = 0.0f; 

    // Update vectors so front/right/up are correct
    camera.updateCameraVectors(); 

    // Set initial quaternion orientation
    camera.orientation = Quaternion(-camera.pitch, camera.yaw); 
    camera.targetOrientation = camera.orientation; 

      
    // Object placement
    glm::vec3 positions[] = {
      {0.0f, 0.0f, -5.0f}, {2.5f, 0.0f, -5.0f}, {5.0f, 0.0f, -5.0f}, {7.5f, 0.0f, -5.0f}, {10.0f, 0.0f, -5.0f},
      {0.0f, -2.5f, -5.0f}, {2.5f, -2.5f, -5.0f}, {5.0f, -2.5f, -5.0f}, {7.5f, -2.5f, -5.0f}, {10.0f, -2.5f, -5.0f}
    };

    std::vector<Object> objects;
    for (int i = 0; i < 10; ++i) {
        objects.push_back({ positions[i], glm::vec3(1.0f), glm::vec3(0.5f), Maths::radians(20.0f * i), "cube" });
    }

    // light setup
    glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);  
    glm::vec3 lightColour(1.0f, 1.0f, 1.0f);
    glm::vec3 ambientColour(0.05f, 0.05f, 0.05f); 
    glm::vec3 specularColour(1.0f, 1.0f, 1.0f); 
    float shininess = 32.0f; 

    // Send light data to shaders
    GLuint programID = shaderProgram;

    glUniform3fv(glGetUniformLocation(programID, "lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniform3fv(glGetUniformLocation(programID, "lightColour"), 1, glm::value_ptr(lightColour)); 
    glUniform3fv(glGetUniformLocation(programID, "ambientColour"), 1, glm::value_ptr(ambientColour));  
    glUniform3fv(glGetUniformLocation(programID, "specularColour"), 1, glm::value_ptr(specularColour));  
    glUniform1f(glGetUniformLocation(programID, "shininess"), shininess); 

    // Input mode
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwSetCursorPosCallback(window, mouse_callback);  

    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);  
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMap); 
 

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // inputs
        keyboardInput(window);

        // time logic
        float currentFrame = glfwGetTime(); 
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Running! (thought this was a better way to handle movement speed)
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            running = true;
        else
            running = false;

        // Input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
            camera.processInput('W', deltaTime, running); 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
            camera.processInput('S', deltaTime, running); 
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
            camera.processInput('A', deltaTime, running); 
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
            camera.processInput('D', deltaTime, running);  
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && camera.isGrounded) { 
            camera.processInput(' ', deltaTime, running);
        }

        camera.updatePhysics(deltaTime);

        // collision
        for (Object& obj : objects)
        {
            float cameraRadius = 0.25f; 

            for (Object& obj : objects)
            {
                // get object's AABB min/max
                glm::vec3 min = obj.position - obj.scale;
                glm::vec3 max = obj.position + obj.scale;

                // find closest point on box to camera
                glm::vec3 closestPoint = glm::clamp(camera.eye, min, max);

                // check distance between camera eye and closest point
                float distance = glm::distance(camera.eye, closestPoint);

                if (distance < cameraRadius)
                {
                    // push camera out of the object
                    glm::vec3 pushDirection = glm::normalize(camera.eye - closestPoint);
                    if (glm::length(pushDirection) == 0.0f)
                    {
                        // if camera is exactly at the closestPoint, pick an arbitrary direction
                        pushDirection = glm::vec3(0.0f, 1.0f, 0.0f);
                    }
                    camera.eye = closestPoint + pushDirection * cameraRadius;

                    // land the player if touching ground
                    if (pushDirection.y > 0.5f)
                    {
                        camera.isGrounded = true;
                        camera.verticalVelocity = 0.0f;
                    }
                }
            }

        }

        // Update camera matrices
        camera.quaternionCamera();

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

        // Use shader + bind
        glUseProgram(shaderProgram); 

        // Set view and projection matrices
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view"); 
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection"); 
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera.view[0][0]); 
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera.projection[0][0]); 

        // light
        glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);  
        glm::vec3 lightColour(1.0f, 1.0f, 1.0f);    
        glm::vec3 ambientColour(0.2f, 0.2f, 0.2f);
        glm::vec3 specularColour(1.0f, 1.0f, 1.0f);  
        float shininess = 32.0f;

        glUniform3fv(glGetUniformLocation(shaderProgram, "lightDirection"), 1, glm::value_ptr(lightDirection)); 
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColour"), 1, glm::value_ptr(lightColour));
        glUniform3fv(glGetUniformLocation(shaderProgram, "ambientLightColour"), 1, glm::value_ptr(ambientColour));  
        glUniform3fv(glGetUniformLocation(shaderProgram, "specularLightColour"), 1, glm::value_ptr(specularColour));  
        glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), shininess);  

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Bind the diffuse map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glUniform1i(glGetUniformLocation(shaderProgram, "diffuseMap"), 1); 

        // Bind the normal map
        glActiveTexture(GL_TEXTURE0);  
        glBindTexture(GL_TEXTURE_2D, normalMap); 
        glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 0); 

        // Bind the specular map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glUniform1i(glGetUniformLocation(shaderProgram, "specularMap"), 2); 


        // Bind VAO
        glBindVertexArray(VAO); 

        // Draw all objects
        for (Object& obj : objects) 
        {
            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::translate(model, obj.position); 
            model = glm::scale(model, obj.scale); 
            model = glm::rotate(model, obj.angle, obj.rotation); 

            GLuint modelLoc = glGetUniformLocation(shaderProgram, "model"); 
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]); 

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
        } 

        // swap buffers + process window events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}



