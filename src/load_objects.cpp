#include "scene.hpp"
#include "shaders.hpp"
#include "load_objects.hpp"

#include "matrices.h"
#include "ball.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

extern GLint g_model_uniform;
extern float g_AngleX;
extern float g_AngleY;
extern GLint g_object_id_uniform;
extern GLuint g_GpuProgramID;

GLuint table_texture_ids[2];

struct Material {
    std::string name;
    std::string texturePath;  // map_Kd
};

std::unordered_map<std::string, Material> loadMTL(const std::string& filename) 
{
    std::unordered_map<std::string, Material> materials;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir " << filename << std::endl;
        return materials;
    }

    std::string line;
    Material currentMaterial;

    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "newmtl") 
        {
            if (!currentMaterial.name.empty()) 
            {
                materials[currentMaterial.name] = currentMaterial;
            }
            currentMaterial = Material(); // cria novo material limpo
            iss >> currentMaterial.name;
        }
        else if (token == "map_Kd") 
        {
            iss >> currentMaterial.texturePath;
        }
    }

    // Adiciona o último material
    if (!currentMaterial.name.empty()) 
    {
        materials[currentMaterial.name] = currentMaterial;
    }

    return materials;
}



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

    // TODO: ARRUMAR O LOAD DE TEXTURAS

    // CARREGA o MTL da mesa
    auto materials = loadMTL("../../data/objs/pooltable.mtl");
    for (const auto& [name, mat] : materials) 
    {
        std::string fullPath = "../../data/" + mat.texturePath;
        LoadTextureImage(fullPath.c_str()); // TableTexture0 e TableTexture1
    }

    // TEXTURA DO TACO
    LoadTextureImage("../../data/textures/darkwood.png"); // TextureImage18
    
}


void loadAllObjects(void)
{   
    ObjModel pooltablemodel("../../data/objs/pooltable.obj");
    ComputeNormals(&pooltablemodel);
    // BuildTrianglesAndAddToVirtualScene(&pooltablemodel);
    BuildTableByMaterial(&pooltablemodel);

    ObjModel cuestickmodel("../../data/objs/cuestick.obj");
    ComputeNormals(&cuestickmodel);
    BuildTrianglesAndAddToVirtualScene(&cuestickmodel);
    
    ObjModel ballmodel("../../data/objs/ball.obj");
    ComputeNormals(&ballmodel);
    BuildTrianglesAndAddToVirtualScene(&ballmodel);
}



void drawInitialScene(const std::vector<Ball>& vec_balls, const Cuestick& cuestick, const Camera& camera)
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
    glUniform1i(g_object_id_uniform, static_cast<int>(ObjectID::TABLE)); // TODO: AQUI´É CARREGADA A TEXTURA DA MESA
    // glUniform1i(g_material_id_uniform, g_VirtualScene["the_pooltable"].material_id);
    // DrawVirtualObject("the_pooltable");
    glUniform1i(g_material_id_uniform, 1);
    DrawVirtualObject("pooltable_mat0");
    glUniform1i(g_material_id_uniform, 0);
    DrawVirtualObject("pooltable_mat1");

    if (cuestick.shouldBeDrawn())
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