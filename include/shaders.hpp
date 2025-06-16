#pragma once


GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
void LoadShadersFromFiles();
void LoadShader(const char* filename, GLuint shader_id);
GLuint LoadShader_Vertex(const char* filename);
GLuint LoadShader_Fragment(const char* filename);

void LoadTextureImage(const char* filename);
