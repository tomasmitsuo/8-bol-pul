#version 330 core

#define BALL0 0
#define BALL1 1
#define BALL2 2
#define BALL3 3
#define BALL4 4
#define BALL5 5
#define BALL6 6
#define BALL7 7
#define BALL8 8
#define BALL9 9
#define BALL10 10
#define BALL11 11
#define BALL12 12
#define BALL13 13
#define BALL14 14
#define BALL15 15
#define TABLE 16 // TODO: FAZER A TEXTURA CORRETA DA MESA
#define CUESTICK 17

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Parâmetros de iluminação enviados via uniform
uniform vec3 Ka;           // coeficiente ambiente
uniform vec3 Kd;           // coeficiente difuso
uniform vec3 Ks;           // coeficiente especular
uniform vec3 light_color;  // cor da luz
uniform float shininess;   // brilho especular


uniform int interpolation_mode;
uniform int object_id;

// TEXTURAS DA MESA
uniform sampler2D TableTexture0;
uniform sampler2D TableTexture1;
// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;

out vec2 texcoords;
out vec3 gouraud_color; 

void main()
{

    gl_Position = projection * view * model * model_coefficients;
    position_world = model * model_coefficients;
    position_model = model_coefficients;

    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;
    texcoords = texture_coefficients;

    // Cálculo da iluminação para Gouraud
    if (object_id == TABLE) 
    {
        float U = (position_model.x - bbox_min.x) / (bbox_max.x - bbox_min.x);
        float V = (position_model.y - bbox_min.y) / (bbox_max.y - bbox_min.y);

        vec3 texture_color = texture(TableTexture0, vec2(U,V)).rgb;

        vec3 Ia = vec3(0.1,0.1,0.1);
        vec3 ambient = Ka * Ia;

        vec4 n = normalize(normal);
        vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

        float lambert = max(0.0, dot(n, l));
        vec3 I = vec3(1.0,1.0,1.0);

        vec3 diffuse = texture_color * I * (lambert + ambient);

        gouraud_color =  diffuse + ambient;
    }
    else
    {
        texcoords = texture_coefficients;
        gouraud_color = vec3(0.0,0.0,0.0);
    }
}

