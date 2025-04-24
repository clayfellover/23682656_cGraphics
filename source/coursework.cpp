#include <iostream>
#include <cmath>

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

    // vertex colours
    const float colours[] = {
        1,0,0, 1,0,0, 1,0,0, 1,0,0, // front face red
        0,1,0, 0,1,0, 0,1,0, 0,1,0, // back face green
        0,0,1, 0,0,1, 0,0,1, 0,0,1, // left face blue
        1,1,0, 1,1,0, 1,1,0, 1,1,0, // right face yellow
        1,0,1, 1,0,1, 1,0,1, 1,0,1, // bottom face magenta
        0,1,1, 0,1,1, 0,1,1, 0,1,1  // top face cyan
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

    // Camera setup
    Camera camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    camera.calculateMatrices();

    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
     
    // Object placement
    glm::vec3 positions[] = {
            {0.0f, 0.0f, 0.0f}, {2.0f, 5.0f, -10.0f}, {-3.0f, -2.0f, -3.0f},
            {-4.0f, -2.0f, -8.0f}, {2.0f, 2.0f, -6.0f}, {-4.0f, 3.0f, -8.0f},
            {0.0f, -2.0f, -5.0f}, {4.0f, 2.0f, -4.0f}, {2.0f, 0.0f, -2.0f}, {-1.0f, 1.0f, -2.0f}
    };

    std::vector<Object> objects;
    for (int i = 0; i < 10; ++i) {
        objects.push_back({ positions[i], glm::vec3(1.0f), glm::vec3(0.5f), Maths::radians(20.0f * i), "cube" });
    }

    // Input mode
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); 


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // inputs
        keyboardInput(window);

        // clear window
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // Use shader + bind
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Loop through all objects and draw
        for (int i = 0; i < static_cast<unsigned int>(objects.size()); ++i) 
        {
            // Model transformation
            glm::mat4 translate = Maths::translate(objects[i].position); 
            glm::mat4 scale = Maths::scale(objects[i].scale); 
            glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(objects[i].angle), objects[i].rotation); 
            glm::mat4 model = translate * rotate * scale; 

            // MVP matrix
            glm::mat4 MVP = camera.projection * camera.view * model; 

            // Send MVP matrix to shader
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &camera.view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &camera.projection[0][0]);


            // Bind index buffer and draw
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0); 
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
