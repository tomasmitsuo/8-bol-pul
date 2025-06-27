#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"


// NOVOS HEADERS CRIADOS PARA SEPARAR A MAIN
#include "shaders.hpp"
#include "callback.hpp"
#include "textrendering.hpp"
#include "scene.hpp"


// CODIGO UTIL

#include "load_objects.hpp"
#include "ball.hpp"
#include "cuestick.hpp"
#include "camera.hpp"
#include "collision.hpp"


std::map<std::string, SceneObject> g_VirtualScene;
std::stack<glm::mat4>  g_MatrixStack;

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
bool g_MiddleMouseButtonPressed = false; // Análogo para botão do meio do mouse

// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
bool g_UsePerspectiveProjection = true;

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;
GLint g_bbox_min_uniform;
GLint g_bbox_max_uniform;

constexpr float TABLE_HEIGHT = 1.5f;
constexpr float TABLE_WIDTH = 3.5f*2;
constexpr float TABLE_LENGTH = 5.5f*2;
constexpr float TABLE_CENTER_X = 0.4f;
constexpr float TABLE_CENTER_Z = 4.0f;

constexpr float BALL_RADIUS = 0.13f;
constexpr float BALL_DIAMITER = BALL_RADIUS * 2.0f;
const float ROW_SPACING = std::sqrt(3.0f) * BALL_RADIUS;

constexpr float TRIANGULE_CENTER_X = TABLE_CENTER_X;
constexpr float TRIANGULE_OFFSET_Z = 0.8f;
constexpr float TRIANGULE_CENTER_Z = TABLE_CENTER_Z - TABLE_CENTER_Z + TRIANGULE_OFFSET_Z;

constexpr float WHITE_BALL_X = TRIANGULE_CENTER_X;
constexpr float WHITE_BALL_Z = TABLE_CENTER_Z + TABLE_CENTER_Z;

constexpr float MAX_SHOT_STRENGTH = 5.0f;
constexpr float STRENGTH_INCREMENT = 2.5f;

// DECLARAÇÃO DAS BOLAS
std::vector<Ball> vec_balls = {
    Ball(WHITE_BALL_X, WHITE_BALL_Z, BALL_RADIUS), // WHITE BALL

    // First row
    Ball(TRIANGULE_CENTER_X, TRIANGULE_CENTER_Z, BALL_RADIUS), // 1

    // Second row
    Ball(TRIANGULE_CENTER_X - BALL_RADIUS, TRIANGULE_CENTER_Z - ROW_SPACING, BALL_RADIUS), // 2
    Ball(TRIANGULE_CENTER_X + BALL_RADIUS, TRIANGULE_CENTER_Z - ROW_SPACING, BALL_RADIUS), // 3

    // Third row
    Ball(TRIANGULE_CENTER_X - BALL_DIAMITER, TRIANGULE_CENTER_Z - 2*ROW_SPACING, BALL_RADIUS), // 4
    Ball(TRIANGULE_CENTER_X, TRIANGULE_CENTER_Z - 2*ROW_SPACING, BALL_RADIUS), // 5
    Ball(TRIANGULE_CENTER_X + BALL_DIAMITER, TRIANGULE_CENTER_Z - 2*ROW_SPACING, BALL_RADIUS), // 6

    // Fourth row
    Ball(TRIANGULE_CENTER_X - 3*BALL_RADIUS, TRIANGULE_CENTER_Z - 3*ROW_SPACING, BALL_RADIUS), // 7
    Ball(TRIANGULE_CENTER_X - BALL_RADIUS, TRIANGULE_CENTER_Z - 3*ROW_SPACING, BALL_RADIUS), // 8
    Ball(TRIANGULE_CENTER_X + BALL_RADIUS, TRIANGULE_CENTER_Z - 3*ROW_SPACING, BALL_RADIUS), // 9
    Ball(TRIANGULE_CENTER_X + 3*BALL_RADIUS, TRIANGULE_CENTER_Z - 3*ROW_SPACING, BALL_RADIUS), // 10

    // Fifth row
    Ball(TRIANGULE_CENTER_X - 2*BALL_DIAMITER, TRIANGULE_CENTER_Z - 4*ROW_SPACING, BALL_RADIUS), // 11
    Ball(TRIANGULE_CENTER_X - BALL_DIAMITER, TRIANGULE_CENTER_Z - 4*ROW_SPACING, BALL_RADIUS), // 12
    Ball(TRIANGULE_CENTER_X, TRIANGULE_CENTER_Z - 4*ROW_SPACING, BALL_RADIUS), // 13
    Ball(TRIANGULE_CENTER_X + BALL_DIAMITER, TRIANGULE_CENTER_Z - 4*ROW_SPACING, BALL_RADIUS), // 14
    Ball(TRIANGULE_CENTER_X + 2*BALL_DIAMITER, TRIANGULE_CENTER_Z - 4*ROW_SPACING, BALL_RADIUS)  // 15
};

auto& white_ball = vec_balls[0];
Camera camera(
    white_ball, 
    TABLE_HEIGHT,
    3.5f, // Distância da câmera
    0.0f, // Ângulo theta (no plano ZX)
    0.6f, // Ângulo phi (em relação ao eixo Y)
    -0.1f, // Posição do near plane
    -100.0f, // Posição do far plane
    3.141592f / 3.0f // Campo de visão (field of view) em radianos
);


// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;

float g_DeltaTime = 0.0f;

int main(int argc, char* argv[])
{   
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 800 colunas e 600 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(1280, 720, "8-bol-pul", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 1280, 720); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados para renderização.

    LoadShadersFromFiles(); // MODIFICAR ESSA FUNÇÃO SEMPRE QUE ADICIONAR TEXTURAS NOVAS

    // CARREGA OBJETOS E TEXTURAS DOS OBJETOS
    loadAllTextures();
    loadAllObjects();

    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf, slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    float last_frame_time = (float)glfwGetTime();

    bool last_right_button = g_RightMouseButtonPressed;
    float shot_strength = 0.0f;
    bool ball_was_shot = false;

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        float current_frame_time = (float)glfwGetTime();
        g_DeltaTime = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(g_GpuProgramID);

        camera.updatePosition();
        const glm::mat4 view = camera.getViewMatrix();
        const glm::mat4 projection = camera.getProjectionMatrix(g_ScreenRatio);

        glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));  

        if (!white_ball.isMoving() && ball_was_shot)
        {
            ball_was_shot = false;
            std::cout << "Ball stopped moving." << std::endl;
        }

        // Only allow a shot if the white ball is not already moving
        if (!white_ball.isMoving() && !g_RightMouseButtonPressed && last_right_button)
        {
            // Apply velocity to the white ball in the camera's direction
            // The g_CameraViewVector is already normalized and points in the look-at direction
            std::cout << "Shot with strength: " << shot_strength << std::endl;
            const glm::vec4 camera_view_vector = camera.getViewVector();
            white_ball.vx = camera_view_vector.x * shot_strength;
            white_ball.vz = camera_view_vector.z * shot_strength;
            shot_strength = 0.0f; // Reset shot strength after applying it
            ball_was_shot = true;
        }
        last_right_button = g_RightMouseButtonPressed;
        
        if (g_RightMouseButtonPressed)
        {
            const float current_increment = STRENGTH_INCREMENT * g_DeltaTime;
            shot_strength = std::min(shot_strength + current_increment, MAX_SHOT_STRENGTH);
            std::cout << "Shot strength: " << shot_strength << std::endl;
        }

        for (auto& ball : vec_balls)
        {
            ball.update(g_DeltaTime);
        }

        handle_ball_collisions(vec_balls,
                              std::make_pair(TABLE_CENTER_X, TABLE_CENTER_Z),
                              TABLE_WIDTH,
                              TABLE_LENGTH);

        drawInitialScene(vec_balls);

        TextRendering_ShowEulerAngles(window);
        TextRendering_ShowProjection(window);
        TextRendering_ShowFramesPerSecond(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}
