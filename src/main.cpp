#include "Shader.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

static SDL_Rect viewport = {
    0,   // x
    0,   // y
    800, // width
    600, // height
};

static std::vector<float> verts = {
    -0.5f,
    -0.5f,
    0.0f, // left
    0.5f,
    -0.5f,
    0.0f, // right
    0.0f,
    0.5f,
    0.0f // top
};

float aspectRatio(int width, int height) { return static_cast<float>(width) / static_cast<float>(height); }

glm::mat4 orthoProjectionMatrix(int width, int height, float fov)
{
    float const top = fov;
    float const right = aspectRatio(width, height) * fov;
    float const bottom = -top;
    float const left = -right;

    return glm::ortho(left, right, bottom, top, 0.0f, 10.0f);
}

GLuint initShaderProgram()
{
    using namespace shader;

    GLuint program = createProgram();

    attachShader(program, GL_VERTEX_SHADER, "shaders/default.vert");
    attachShader(program, GL_FRAGMENT_SHADER, "shaders/default.frag");
    linkProgram(program);

    return program;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

#ifdef __linux__
    // Hints for Linux
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif

    auto const flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window =
        SDL_CreateWindow("OpenGL with SDL2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowMinimumSize(window, 400, 300);
    auto glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    std::cout << "OpenGL Version:        " << glGetString(GL_VERSION) << "\n"
              << "GLSL Version:          " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
              << "Renderer:              " << glGetString(GL_RENDERER) << "\n"
              << "Vendor:                " << glGetString(GL_VENDOR) << std::endl;

    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;

    bool isDone = false;
    GLuint const program = initShaderProgram();

    glm::mat4 const identityMat = glm::mat4(1.0f);
    glm::mat4 const modelMatrix = identityMat;
    glm::mat4 const viewMatrix = identityMat;

    while (!isDone)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: isDone = true; break;
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_q:
                            if (KMOD_CTRL & event.key.keysym.mod)
                            {
                                isDone = true;
                            }
                            break;
                    }
                }
                break;
                case SDL_WINDOWEVENT: SDL_GetWindowSize(window, &viewport.w, &viewport.h); break;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        auto const& [x, y, w, h] = viewport;
        glViewport(x, y, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glUseProgram(program);

        shader::setUniformMatrix4fv(program, "modelMat", modelMatrix);
        shader::setUniformMatrix4fv(program, "viewProjMat", orthoProjectionMatrix(w, h, 1.0f) * viewMatrix);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    shader::deleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
