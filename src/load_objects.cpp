#include "scene.hpp"
#include "shaders.hpp"
#include "load_objects.hpp"

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



void drawInitialScene(const std::vector<Ball>& vec_balls, const Cuestick& cuestick, const Camera& camera, bool isWhiteBallMoving)
{
    for (const auto& ball : vec_balls)
    {
        if (!ball.shouldBeDrawn()) {
            continue;
        }
        const glm::vec4& position = ball.getPosition();
        glm::mat4 t = Matrix_Translate(position.x, position.y, position.z);
        glm::mat4 r = Matrix_Rotate(-ball.getRotationAngle(), ball.getRotationAxis());
        glm::mat4 m = t * r;
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(m));
        glUniform1i(g_object_id_uniform, ball.getObjectID());
        DrawVirtualObject("the_ball");
    }


    // Desenhamos a mesa
    glm::mat4 model_table = Matrix_Translate(Table::CENTER_X, 0.0f, Table::CENTER_Z);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model_table));
    glUniform1i(g_object_id_uniform, static_cast<int>(ObjectID::BALL3));
    DrawVirtualObject("the_pooltable");

    if (!isWhiteBallMoving)
    {
        const glm::vec4& cuestick_position = cuestick.getPosition();
        const glm::vec3& cuestick_angles = cuestick.getAngles();
        const glm::mat4 t = Matrix_Translate(cuestick_position.x, cuestick_position.y, cuestick_position.z);

        const glm::mat4 r_yaw = Matrix_Rotate_Y(cuestick_angles.y);
        const glm::mat4 r_pitch = Matrix_Rotate_X(cuestick_angles.x);

        const glm::mat4 make_cue_horizontal = Matrix_Rotate_X(Cuestick::ANGLE);

        glm::mat4 model_cuestick = t * r_yaw * r_pitch * make_cue_horizontal;
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model_cuestick));
        glUniform1i(g_object_id_uniform, cuestick.getObjectID());
        DrawVirtualObject("the_cuestick");
    }
}