#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
GLuint cargarTextura(const char* path); //Convierte archivo de imagen en textura de OpenGL

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,   0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f)
};

//Se modifico para hacer uso de las coordenadas de textura (UV)
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Id s de las texturas que utilizaremos, las cuales estan almacenadas en una carpeta de nuestro proyecto
GLuint texMorado, texGris, texVerde, texRosa, texAzul,
texFucsia, texCrema, texNegro, texAzulLibro, texMoradoLibro,
texMoradoMesa, texRosaSilla, texRosaTapete;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cuarto Chicas Superpoderosas", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST); // Habilita el Z-buffer (orden de profundidad 3D)



    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model Piso((char*)"Models/piso.obj");
    Model bus((char*)"Models/pared1.obj");
    Model Pared((char*)"Models/pared1.obj");
    Model Pared2((char*)"Models/pared2.obj");
    Model Pared3((char*)"Models/pared3.obj");
    Model Pared4((char*)"Models/pared4.obj");

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configuraci n de coordenadas de textura (UV) que agregamos
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Aqu  cargamos nuestras texturas que estan en nuestra carpeta "texturas", las cuales con colores solidos
    texMorado = cargarTextura("textures/morado.png");       // Base cama, respaldo, repisa, baul fuera, buros
    texGris = cargarTextura("textures/gris.png");         // Cajones de buros
    texVerde = cargarTextura("textures/verde.png");        // Cobija bellota
    texRosa = cargarTextura("textures/rosa.png");         // Cobija bomb n
    texAzul = cargarTextura("textures/azul.png");         // Cobija burbuja
    texFucsia = cargarTextura("textures/fucsia.png");       // Respaldo cama, base cama
    texCrema = cargarTextura("textures/crema.png");        // baul adentro
    texAzulLibro = cargarTextura("textures/azul_libro.png");   // Libros azules
    texMoradoLibro = cargarTextura("textures/morado_libro.png"); // Libros morados
    texMoradoMesa = cargarTextura("textures/morado_mesa.png");  // Mesa y patas
    texRosaSilla = cargarTextura("textures/rosa_silla.png");   // Sillas
    texRosaTapete = cargarTextura("textures/rosa_tapete.png");  // Tapete corazon

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16);

    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {

        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL options
        //glEnable(GL_DEPTH_TEST);



        //Load Model

        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();


        //glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
        //glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

        // Point light 3
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

        // SpotLight 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 0.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 20.0f, 20.0f, 20.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.8f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(1.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // Create camera transformations
        glm::mat4 view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);


        //Carga de modelo
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.380f, 1.24f));///AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
        model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE0);
        Piso.Draw(lightingShader);


        // Carga el modelo de las paredes
        //pared1 YA ESTAAAAAAA LISTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.20f, 3.12f, 1.24f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE0);
        Pared.Draw(lightingShader);


        //pared2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.29f, 3.12f, 1.24f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE0);
        Pared2.Draw(lightingShader);


        //pared3 YA ESTAAAAAAA LISTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 3.12f, 6.525f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE0);
        Pared3.Draw(lightingShader);


        //pared4 YA ESTAAAAAAA LISTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 3.12f, -5.97f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE0);
        Pared4.Draw(lightingShader);




        // Activar unidad de textura 0 para todos los cubos
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        //******************
        //Cama con respaldo* 
        //******************

        glBindTexture(GL_TEXTURE_2D, texFucsia);

        float anchoCama = 4.2f;
        float numCuadros = 40.0f;
        float anchoPieza = anchoCama / numCuadros;

        float alturaBase = 1.2f;
        float alturaMaxima = 1.9f;
        float incremento = (alturaMaxima - alturaBase) / 20.0f;

        for (int i = 0; i < 40; i++) {
            float alturaActual;
            if (i < 20) {
                alturaActual = alturaBase + (i * incremento);
            }
            else {
                alturaActual = alturaBase + ((39 - i) * incremento);
            }

            model = glm::mat4(1.0f);
            float posX = -2.1f + (i * anchoPieza) + (anchoPieza / 2.0f);
            model = glm::translate(model, glm::vec3(posX, (alturaActual / 2.0f) + 0.5f, -4.5f));
            model = glm::scale(model, glm::vec3(anchoPieza, alturaActual, 0.15f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //*********************
        //Coraz n del respaldo* 
        //*********************

        glBindTexture(GL_TEXTURE_2D, texRosaTapete);

        glm::vec3 centroCorazonCabecera = glm::vec3(0.0f, 1.45f, -4.35f);

        // Pico del coraz n
        model = glm::mat4(1.0f);
        model = glm::translate(model, centroCorazonCabecera);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Orejitas del coraz n
        for (int j = 0; j < 10; j++) {
            float rot = j * 18.0f;

            // Izquierdo
            model = glm::mat4(1.0f);
            model = glm::translate(model, centroCorazonCabecera + glm::vec3(-0.18f, 0.18f, 0.02f));
            model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Derecho
            model = glm::mat4(1.0f);
            model = glm::translate(model, centroCorazonCabecera + glm::vec3(0.18f, 0.18f, 0.02f));
            model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //**********
        //Almohadas*
        //**********

        // Almohada izquierda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.1f, 0.65f, -3.8f));
        model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Almohada del centro
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.65f, -3.8f));
        model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Almohada derecha
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.1f, 0.65f, -3.8f));
        model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //*****************
        // Base de la cama*
        //*****************

        glBindTexture(GL_TEXTURE_2D, texFucsia);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.25f, -2.0f));
        model = glm::scale(model, glm::vec3(4.2f, 0.5f, 5.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //*******
        //Cobija*
        //*******

        // Cobija verde
        glBindTexture(GL_TEXTURE_2D, texVerde);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.2f, 0.55f, -2.0f));
        model = glm::scale(model, glm::vec3(1.4f, 0.12f, 5.0f)); // m s larga en Z
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cobija rosa
        glBindTexture(GL_TEXTURE_2D, texRosa);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.55f, -2.0f));
        model = glm::scale(model, glm::vec3(1.4f, 0.12f, 5.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cobija azul
        glBindTexture(GL_TEXTURE_2D, texAzul);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, 0.55f, -2.0f));
        model = glm::scale(model, glm::vec3(1.4f, 0.12f, 5.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //******
        //Buros*
        //******


        float posicionesX[] = { -2.8f, 2.8f };

        for (int i = 0; i < 2; i++) {

            //Base
            glBindTexture(GL_TEXTURE_2D, texMorado);
            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(posicionesX[i], 0.5f, -4.0f));
            model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.6f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Dos cajones
            glBindTexture(GL_TEXTURE_2D, texGris);

            float alturas[2] = { 0.7f, 0.3f };

            for (int j = 0; j < 2; j++) {

                // Parte de enfrente del caj n
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(posicionesX[i], alturas[j], -3.68f)); // un poco al frente
                model = glm::scale(model, glm::vec3(0.6f, 0.3f, 0.06f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);

                //Manija de los cajones
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(posicionesX[i], alturas[j], -3.62f)); // sobresale m s
                model = glm::scale(model, glm::vec3(0.15f, 0.05f, 0.05f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        }

        //******************
        //Repisa con libros*
        //******************

        glBindTexture(GL_TEXTURE_2D, texMorado);

        // Base
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, 3.4f, -4.8f));
        model = glm::scale(model, glm::vec3(2.0f, 0.1f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Techo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, 4.2f, -4.8f));
        model = glm::scale(model, glm::vec3(2.0f, 0.1f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Lado izquierdo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.5f, 3.8f, -4.8f));
        model = glm::scale(model, glm::vec3(0.1f, 0.8f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Lado derecho
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.5f, 3.8f, -4.8f));
        model = glm::scale(model, glm::vec3(0.1f, 0.8f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Fondo de la repiza
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, 3.8f, -4.95f));
        model = glm::scale(model, glm::vec3(2.0f, 0.8f, 0.05f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Libro 1: Azul claro (inclinado izquierda)
        glBindTexture(GL_TEXTURE_2D, texAzulLibro);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.1f, 3.75f, -4.75f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.18f, 0.6f, 0.35f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Libro 2: Morado magenta (inclinado derecha)
        glBindTexture(GL_TEXTURE_2D, texMoradoLibro);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.6f, 3.75f, -4.75f));
        model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.18f, 0.65f, 0.35f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Libro 3: Azul (derecho)
        glBindTexture(GL_TEXTURE_2D, texAzulLibro);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.1f, 3.8f, -4.75f));
        model = glm::scale(model, glm::vec3(0.18f, 0.7f, 0.35f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Libro 4: Morado (derecho)
        glBindTexture(GL_TEXTURE_2D, texMoradoLibro);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.8f, 3.75f, -4.75f));
        model = glm::scale(model, glm::vec3(0.15f, 0.6f, 0.35f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //*****
        //Ba l*
        //*****

        // Caja
        glBindTexture(GL_TEXTURE_2D, texMorado);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.3f, 0.4f, -2.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.2f, 0.8f, 1.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Interior
        glBindTexture(GL_TEXTURE_2D, texCrema);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.28f, 0.45f, -2.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 0.8f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Tapa 
        glBindTexture(GL_TEXTURE_2D, texMorado);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.3f, 0.4f, -2.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.6f));
        model = glm::rotate(model, glm::radians(-115.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(2.2f, 0.8f, 0.15f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Tapa por dentro
        glBindTexture(GL_TEXTURE_2D, texCrema);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.3f, 0.4f, -2.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.6f));
        model = glm::rotate(model, glm::radians(-115.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.38f, 0.08f));
        model = glm::scale(model, glm::vec3(2.0f, 0.7f, 0.05f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //**************************
        //Mesa "circular" y sillas*
        //**************************

        // Base "circular"
        glBindTexture(GL_TEXTURE_2D, texMoradoMesa);
        glm::vec3 centroMesa = glm::vec3(-3.5f, 0.0f, 1.5f);
        for (int i = 0; i < 26; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, centroMesa + glm::vec3(0.0f, 0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(i * 3.4f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.4f, 0.1f, 1.4f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Patas de la mesa 
        float mpx[] = { -0.4f,  0.4f, -0.4f, 0.4f };
        float mpz[] = { -0.4f, -0.4f,  0.4f, 0.4f };
        for (int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, centroMesa + glm::vec3(mpx[i], 0.25f, mpz[i]));
            model = glm::scale(model, glm::vec3(0.15f, 0.5f, 0.15f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Sillas
        glBindTexture(GL_TEXTURE_2D, texRosaSilla);
        glm::vec3 posSillas[] = {
            centroMesa + glm::vec3(-1.2f, 0.0f,  0.0f),
            centroMesa + glm::vec3(0.0f, 0.0f, -1.2f),
            centroMesa + glm::vec3(1.2f, 0.0f,  0.0f)
        };
        float angSillas[] = { -90.0f, 180.0f, 90.0f };

        for (int i = 0; i < 3; i++) {
            // Asiento
            model = glm::mat4(1.0f);
            model = glm::translate(model, posSillas[i] + glm::vec3(0.0f, 0.35f, 0.0f));
            model = glm::rotate(model, glm::radians(angSillas[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f, 0.15f, 0.7f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Respaldo
            model = glm::mat4(1.0f);
            model = glm::translate(model, posSillas[i] + glm::vec3(0.0f, 0.75f, 0.0f));
            model = glm::rotate(model, glm::radians(angSillas[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
            model = glm::scale(model, glm::vec3(0.7f, 0.65f, 0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // 4 patas para c/silla
            float spx[] = { -0.25f,  0.25f, -0.25f, 0.25f };
            float spz[] = { -0.25f, -0.25f,  0.25f, 0.25f };
            for (int j = 0; j < 4; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, posSillas[i] + glm::vec3(0.0f, 0.175f, 0.0f));
                model = glm::rotate(model, glm::radians(angSillas[i]), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::translate(model, glm::vec3(spx[j], 0.0f, spz[j]));
                model = glm::scale(model, glm::vec3(0.12f, 0.35f, 0.12f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        //***************
        //Tapete coraz n*
        //***************

        glBindTexture(GL_TEXTURE_2D, texRosaTapete);
        glm::vec3 centroCorazon = glm::vec3(0.0f, 0.01f, 1.2f);

        // Base picuda
        model = glm::mat4(1.0f);
        model = glm::translate(model, centroCorazon);
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Orejita izquierda
        for (int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, centroCorazon + glm::vec3(-0.35f, 0.0f, 0.35f));
            model = glm::rotate(model, glm::radians(i * 9.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f, 0.01f, 0.7f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Orejita derecha
        for (int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, centroCorazon + glm::vec3(0.35f, 0.0f, 0.35f));
            model = glm::rotate(model, glm::radians(i * 9.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f, 0.01f, 0.7f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);


        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        GLint lModelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint lViewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint lProjLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Set matrices
        glUniformMatrix4fv(lViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //model = glm::mat4(1);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        // Draw the light object (using light's vertex attributes)

        for (GLuint i = 0; i < 4; i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(lModelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);



        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}


//***************************************************
//Funci n para cargar textura con ayuda de stb_image*
//***************************************************

GLuint cargarTextura(const char* path)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR: No se pudo cargar textura: " << path << std::endl;
    }
    stbi_image_free(data);
    return texID;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{

    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);

    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);


    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);


    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);


    }

    if (keys[GLFW_KEY_T])
    {
        pointLightPositions[0].x += 0.01f;
    }
    if (keys[GLFW_KEY_G])
    {
        pointLightPositions[0].x -= 0.01f;
    }

    if (keys[GLFW_KEY_Y])
    {
        pointLightPositions[0].y += 0.01f;
    }

    if (keys[GLFW_KEY_H])
    {
        pointLightPositions[0].y -= 0.01f;
    }
    if (keys[GLFW_KEY_U])
    {
        pointLightPositions[0].z -= 0.1f;
    }
    if (keys[GLFW_KEY_J])
    {
        pointLightPositions[0].z += 0.01f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        if (active)
        {
            Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
        }
        else
        {
            Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}