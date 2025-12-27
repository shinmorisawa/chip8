#include <SDL3/SDL.h>

#include "render.h"
#include "chip.h"
#include "glad.h"

extern SDL_Window* window;
extern Chip8 chip8;

static GLuint screen_tex;
static GLuint shader;
static GLuint quad_vao;
static GLuint quad_vbo;

static int w,h;

/* vertex_src = minified shaders/basic.vsh
 * fragment_src = minified shaders/basic.fsh
 */

static const char* vertex_src =
"#version 330 core\n"
"layout (location = 0) in vec2 pos;\n"
"layout (location = 1) in vec2 uv;\n"
"out vec2 v_uv;\n"
"void main() {\n"
"   v_uv = uv;\n"
"   gl_Position = vec4(pos, 0.0, 1.0);\n"
"}\n";

static const char* fragment_src = 
"#version 330 core\n"
"in vec2 v_uv;\n"
"out vec4 color;\n"
"uniform sampler2D screen;\n"
"void main() {\n"
"   vec2 uv = vec2(v_uv.x, 1.0 - v_uv.y);\n"
"   vec4 onColor = vec4(1.0, 0.5, 1.0, 1.0);\n"
"   vec4 offColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
"   float p = texture(screen, uv).r;\n"
"   color = mix(offColor, onColor, p);\n"
"}\n";

void init_render() {
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    /* quad_vao & quad_vbo init */
    float quad[] = {
        // pos      // uv
        -1.f, -1.f, 0.f, 0.f,
         1.f, -1.f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
        
        -1.f, -1.f, 0.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 1.f,
    };

    glGenVertexArrays(1, &quad_vao);
    glGenBuffers(1, &quad_vbo);

    glBindVertexArray(quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    /* screen_tex init */
    glGenTextures(1, &screen_tex);
    glBindTexture(GL_TEXTURE_2D, screen_tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R8,
        64, 32,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        NULL
    );

    /* nearest neighbor go brrr */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 

    /* compile shaders */
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_src, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_src, NULL);
    glCompileShader(fs);

    /* create shader program */
    shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);

    /* declare screen uniform */
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "screen"), 0);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, screen_tex);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0, 0,
        64, 32,
        GL_RED,
        GL_UNSIGNED_BYTE,
        chip8.framebuffer
    );

    glUseProgram(shader);
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(window);
}
