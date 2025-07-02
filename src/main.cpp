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
#include "table.hpp"
#include "ball.hpp"
#include "camera.hpp"
#include "cuestick.hpp"
#include "game_balls.hpp"

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

// DECLARAÇÃO DAS BOLAS
GameBalls gameBalls(
    Ball::RADIUS, 
    glm::vec2(Ball::WHITE_BALL_X, Ball::WHITE_BALL_Z), 
    glm::vec2(Ball::TRIANGLE_CENTER_X, Ball::TRIANGLE_CENTER_Z), 
    Ball::ROW_SPACING
);
Ball& white_ball = gameBalls.getWhiteBall();
std::vector<Ball>& vec_balls = gameBalls.getBalls();


Camera camera(
    white_ball, 
    Camera::HEIGHT, // Altura da câmera
    Camera::SPEED, // Velocidade da câmera
    Camera::DISTANCE, // Distância da câmera
    Camera::THETA, // Ângulo theta (no plano ZX)
    Camera::PHI, // Ângulo phi (em relação ao eixo Y)
    Camera::NEAR_PLANE, // Posição do near plane
    Camera::FAR_PLANE, // Posição do far plane
    Camera::FIELD_OF_VIEW // Campo de visão (field of view) em radianos
);

Cuestick cuestick(
    ObjectID::CUESTICK,
    white_ball.getPosition().x,
    Cuestick::HEIGHT,
    white_ball.getPosition().z,
    g_AngleX, // Ângulo X
    g_AngleY, // Ângulo Y
    g_AngleZ, // Ângulo Z
    white_ball.getRadius()
);

Table pool_table(
    Table::CENTER_X,
    Table::CENTER_Z,
    Table::WIDTH,
    Table::LENGTH,
    Table::HOLE_RADIUS
);

// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;

float g_DeltaTime = 0.0f;

bool g_goFront = false;
bool g_goBack = false;
bool g_goRight = false;
bool g_goLeft = false;
bool g_goStrafeLeft = false;
bool g_goStrafeRight = false;

bool g_ResetGame = false;

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

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        float current_frame_time = (float)glfwGetTime();
        g_DeltaTime = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(g_GpuProgramID);

        if (g_ResetGame) {
            gameBalls.reset();
            white_ball = gameBalls.getWhiteBall();
            vec_balls = gameBalls.getBalls();
            camera.setUsingPerspectiveProjection(g_UsePerspectiveProjection);
            camera.setCameraType(true);
            cuestick.resetAim();
        }

        camera.control(g_DeltaTime, g_goFront, g_goBack, g_goRight, g_goLeft);
        const glm::mat4 view = camera.getViewMatrix();
        const glm::mat4 projection = camera.getProjectionMatrix(g_ScreenRatio);

        glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

        cuestick.control(
            g_DeltaTime,
            g_RightMouseButtonPressed,
            g_MiddleMouseButtonPressed,
            g_goFront,
            g_goBack,
            g_goLeft,
            g_goRight,
            g_goStrafeLeft,
            g_goStrafeRight,
            camera,
            white_ball
        );
       
        gameBalls.update(g_DeltaTime, pool_table);

        cuestick.update(g_DeltaTime, white_ball, camera);

        pool_table.update(vec_balls);

        drawInitialScene(vec_balls, cuestick, camera, white_ball.isMoving() || white_ball.isAnimating());

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
