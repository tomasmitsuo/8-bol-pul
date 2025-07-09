
#version 330 core

// Identificador que define qual objeto está sendo desenhado no momento
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
#define TABLE 16 
#define CUESTICK 17

in vec4 position_world;
in vec4 normal;
in vec4 position_model;
in vec2 texcoords;

in vec3 gouraud_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// TEXTURAS

uniform int object_id;
uniform int material_id;
uniform int interpolation_mode; 



// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura

// BOLAS DE 1 A 15
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;
uniform sampler2D TextureImage8;
uniform sampler2D TextureImage9;
uniform sampler2D TextureImage10;
uniform sampler2D TextureImage11;
uniform sampler2D TextureImage12;
uniform sampler2D TextureImage13;
uniform sampler2D TextureImage14;
uniform sampler2D TextureImage15;

// TEXTURAS DA MESA
uniform sampler2D TableTexture0;
uniform sampler2D TableTexture1;

// TEXTURA DO TACO;
uniform sampler2D TextureImage18;



// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{   

    vec3 Kd, Ks, Ka;
    float shininess;
    int interpolation_mode;

    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;
    vec4 p = position_world;
    vec4 n = normalize(normal);
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));
    vec4 v = normalize(camera_position - p);


    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    if ( object_id == BALL0 || object_id == BALL1 || object_id == BALL2 || object_id == BALL3 ||
         object_id == BALL4 || object_id == BALL5 || object_id == BALL6 ||
         object_id == BALL7 || object_id == BALL8 || object_id == BALL9 ||
         object_id == BALL10 || object_id == BALL11 || object_id == BALL12 ||
         object_id == BALL13 || object_id == BALL14 || object_id == BALL15 )
    {
        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        vec4 delta = position_model - bbox_center;
        float raio = length(delta);

        float ro = asin(position_model[1]/raio);
        float theta = atan(position_model[0], position_model[2]);

        U = (theta + M_PI) / (2*M_PI);
        V = (ro + M_PI_2) / M_PI;
    }


    if(object_id == BALL0)
    {
        Kd = texture(TextureImage0, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL1)
    {
        Kd = texture(TextureImage1, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL2)
    {
        Kd = texture(TextureImage2, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL3)
    {
        Kd = texture(TextureImage3, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL4)
    {    
        Kd = texture(TextureImage4, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL5)
    {
        Kd = texture(TextureImage5, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL6)
    {
        Kd = texture(TextureImage6, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL7)
    {
        Kd = texture(TextureImage7, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL8)
    {
        Kd = texture(TextureImage8, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL9)
    {
        Kd = texture(TextureImage9, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL10)
    {
        Kd = texture(TextureImage10, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL11)
    {
        Kd = texture(TextureImage11, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL12)
    {
        Kd = texture(TextureImage12, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL13)
    {
        Kd = texture(TextureImage13, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL14)
    {
        Kd = texture(TextureImage14, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
    else if(object_id == BALL15)
    {
        Kd = texture(TextureImage15, vec2(U,V)).rgb; 
        Ks = vec3(0.5f); 
        Ka = vec3(0.2f); 
        shininess = 64.0f; 
        interpolation_mode = 1;
    }
        // ADICIONAR AS TEXTURAS AQUI

    else if(object_id == TABLE)
    {
        U = texcoords.x;
        V = texcoords.y;

        if (material_id == 0)
        {
            Kd = texture(TableTexture0, vec2(U, V)).rgb; Ks = vec3(0.1f); Ka = vec3(0.3f); shininess = 16.0f; interpolation_mode = 0;
        }
        else if (material_id == 1)
        {
            Kd = texture(TableTexture1, vec2(U, V)).rgb; Ks = vec3(0.1f); Ka = vec3(0.3f); shininess = 16.0f; interpolation_mode = 0;
        }
    }

    else if(object_id == CUESTICK)
    {
        Kd = texture(TextureImage18, vec2(U,V)).rgb; Ks = vec3(0.4f); Ka = vec3(0.15f); shininess = 128.0f; interpolation_mode = 1; 
    }

    color.rgb = Kd;
    color.a = 1;


    vec3 Ia = vec3(0.1,0.1,0.1);
    vec3 ambient = Ka * Ia;

    float lambert = max(0.0, dot(n, l));
    vec3 I = vec3(1.0,1.0,1.0);
    vec3 diffuse = Kd * I * (lambert + ambient);

    vec4 h = normalize(v + l);
    float spec = pow(max(dot(n, h), 0.0), shininess);
    vec3 specular = Ks * spec;

    if (interpolation_mode == 0) 
    {
        color.rgb = gouraud_color * color.rgb;
    }
    else //BLING_PHONG
    {  
        if( object_id == BALL0 || object_id == BALL1 || object_id == BALL2 || object_id == BALL3 ||
         object_id == BALL4 || object_id == BALL5 || object_id == BALL6 ||
         object_id == BALL7 || object_id == BALL8 || object_id == BALL9 ||
         object_id == BALL10 || object_id == BALL11 || object_id == BALL12 ||
         object_id == BALL13 || object_id == BALL14 || object_id == BALL15 )
        {
            color.rgb = diffuse + ambient + specular;
        }
        else
        {
            color.rgb = diffuse + ambient + specular;
        }
    }


    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}
