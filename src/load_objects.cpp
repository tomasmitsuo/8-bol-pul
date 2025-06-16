#include "scene.hpp"
#include "shaders.hpp"
# include "load_objects.hpp"

#include "matrices.h"
#include "ball.hpp"

extern GLint g_model_uniform;
extern float g_AngleX;
extern float g_AngleY;
extern GLint g_object_id_uniform;

void loadAllTextures(void)
{
    LoadTextureImage("../../data/textures/ball_00.png");      // TextureImage0
    LoadTextureImage("../../data/textures/ball_01.png");      // TextureImage1
    LoadTextureImage("../../data/textures/ball_02.png");      // TextureImage2
    LoadTextureImage("../../data/textures/ball_03.png");      // TextureImage3
    LoadTextureImage("../../data/textures/ball_04.png");      // TextureImage4
    LoadTextureImage("../../data/textures/ball_05.png");      // TextureImage5
    LoadTextureImage("../../data/textures/ball_06.png");      // TextureImage6
    LoadTextureImage("../../data/textures/ball_07.png");      // TextureImage7
    LoadTextureImage("../../data/textures/ball_08.png");      // TextureImage8
    LoadTextureImage("../../data/textures/ball_09.png");      // TextureImage9
    LoadTextureImage("../../data/textures/ball_10.png");      // TextureImage10
    LoadTextureImage("../../data/textures/ball_11.png");      // TextureImage11
    LoadTextureImage("../../data/textures/ball_12.png");      // TextureImage12
    LoadTextureImage("../../data/textures/ball_13.png");      // TextureImage13
    LoadTextureImage("../../data/textures/ball_14.png");      // TextureImage14
    LoadTextureImage("../../data/textures/ball_15.png");      // TextureImage15
}



void loadAllObjects(void)
{   
    ObjModel pooltablemodel("../../data/objs/pooltable.obj");
    ComputeNormals(&pooltablemodel);
    BuildTrianglesAndAddToVirtualScene(&pooltablemodel);

    ObjModel cuestickmodel("../../data/objs/cuestick.obj");
    ComputeNormals(&cuestickmodel);
    BuildTrianglesAndAddToVirtualScene(&cuestickmodel);
    
    ObjModel ballmodel("../../data/objs/ball.obj");
    ComputeNormals(&ballmodel);
    BuildTrianglesAndAddToVirtualScene(&ballmodel);
}



void drawInitialScene(std::vector<Ball>& vec_balls)
{
    enum enum_name {
            BALL0,
            BALL1,BALL2,BALL3,BALL4,BALL5,
            BALL6,BALL7,BALL8,BALL9,BALL10,
            BALL11,BALL12,BALL13,BALL14,BALL15,
            TABLE, CUESTICK
        };

    int balls[] = {BALL0, BALL1, BALL2, BALL3, BALL4, BALL5, BALL6, BALL7, BALL8, BALL9, BALL10, BALL11, BALL12, BALL13, BALL14, BALL15};

    for(int i=0; i<16; i++){
        glm::mat4 m = Matrix_Translate(vec_balls[i].x, 1.1f, vec_balls[i].z);
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(m));
        glUniform1i(g_object_id_uniform, balls[i]);
        DrawVirtualObject("the_ball");
    }


    // Desenhamos a mesa
    glm::mat4 model_table = Matrix_Translate(0.4f,0.0f,4.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model_table));
        glUniform1i(g_object_id_uniform, BALL3);
        DrawVirtualObject("the_pooltable");


    glm::mat4 model = Matrix_Translate(0.0f, 0.0f, 0.0f)
        * Matrix_Rotate_Y(g_AngleY + (float)glfwGetTime() * 0.1f);
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, CUESTICK);
        DrawVirtualObject("the_cuestick");
}