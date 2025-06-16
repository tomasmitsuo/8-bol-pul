
#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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
#define BUNNY 16 // TODO: Mudar para TABLE E CUESTICK
#define PLANE 17

uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
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

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
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
        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        vec4 delta = position_model - bbox_center;
        float raio = length(delta);

        float ro = asin(position_model[1]/raio);
        float theta = atan(position_model[0], position_model[2]);

        U = (theta + M_PI) / (2*M_PI);
        V = (ro + M_PI_2) / M_PI;
    }
    else if ( object_id == BUNNY )
    {
        // PREENCHA AQUI as coordenadas de textura do coelho, computadas com
        // projeção planar XY em COORDENADAS DO MODELO. Utilize como referência
        // o slides 99-104 do documento Aula_20_Mapeamento_de_Texturas.pdf,
        // e também use as variáveis min*/max* definidas abaixo para normalizar
        // as coordenadas de textura U e V dentro do intervalo [0,1]. Para
        // tanto, veja por exemplo o mapeamento da variável 'p_v' utilizando
        // 'h' no slides 158-160 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // Veja também a Questão 4 do Questionário 4 no Moodle.

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = (position_model.x - minx) / (maxx - minx);
        V = (position_model.y - miny) / (maxy - miny);   
    }
    else if ( object_id == PLANE )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;
    }

    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 Kd = vec3(0.5, 0.5, 0.5); // Valor padrão

    if(object_id == BALL0)
        Kd = texture(TextureImage0, vec2(U,V)).rgb;
    else if(object_id == BALL1)
        Kd = texture(TextureImage1, vec2(U,V)).rgb;
    else if(object_id == BALL2)
        Kd = texture(TextureImage2, vec2(U,V)).rgb;
    else if(object_id == BALL3)
        Kd = texture(TextureImage3, vec2(U,V)).rgb;
    else if(object_id == BALL4)
        Kd = texture(TextureImage4, vec2(U,V)).rgb;
    else if(object_id == BALL5)
        Kd = texture(TextureImage5, vec2(U,V)).rgb;
    else if(object_id == BALL6)
        Kd = texture(TextureImage6, vec2(U,V)).rgb;
    else if(object_id == BALL7)
        Kd = texture(TextureImage7, vec2(U,V)).rgb;
    else if(object_id == BALL8)
        Kd = texture(TextureImage8, vec2(U,V)).rgb;
    else if(object_id == BALL9)
        Kd = texture(TextureImage9, vec2(U,V)).rgb;
    else if(object_id == BALL10)
        Kd = texture(TextureImage10, vec2(U,V)).rgb;
    else if(object_id == BALL11)
        Kd = texture(TextureImage11, vec2(U,V)).rgb;
    else if(object_id == BALL12)
        Kd = texture(TextureImage12, vec2(U,V)).rgb;
    else if(object_id == BALL13)
        Kd = texture(TextureImage13, vec2(U,V)).rgb;
    else if(object_id == BALL14)
        Kd = texture(TextureImage14, vec2(U,V)).rgb;
    else if(object_id == BALL15)
        Kd = texture(TextureImage15, vec2(U,V)).rgb;
    else if(object_id == BUNNY)
        Kd = texture(TextureImage0, vec2(U,V)).rgb;
    else if(object_id == PLANE)
        Kd = texture(TextureImage0, vec2(U,V)).rgb;

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));
    Kd = Kd * lambert; // INTERPOLAÇÃO DAS TEXTURAS

    color.rgb = Kd;

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}
