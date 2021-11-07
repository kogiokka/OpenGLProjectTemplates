#include "UI.hpp"
#include "State.hpp"
#include "gl/Buffer.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <string>

namespace ui
{
    struct Var Var;

    void render();
    namespace MenuBar
    {
        void render();
        namespace Edit
        {
            void render();
            namespace Preferences
            {
                void render();
            }
        }
        namespace View
        {
            void render();
        }
        namespace Help
        {
            void render();
        }
    }
    namespace Window
    {
        void render();
        namespace VertexEditor
        {
            void render();
            namespace VertexTab
            {
                void render();
            }
            namespace ColorTab
            {
                void render();
            }
        }
        namespace About
        {
            void render();
            namespace AboutTab
            {
                void render();
            }
            namespace ComponentsTab
            {
                void render();
            }
            namespace AuthorsTab
            {
                void render();
            }
        }
#ifndef NDEBUG
        namespace Demo
        {
            void render();
        }
#endif
    }
}

void ui::create()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(state.window->handle, state.window->glContext);
    ImGui_ImplOpenGL3_Init();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("res/imgui/misc/fonts/DroidSans.ttf", 18);
}

void ui::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(state.window->handle);
    ImGui::NewFrame();

    switch (Var.Preferences.Theme) {
    case Var::Preferences::Theme::Light:
        ImGui::StyleColorsLight();
        state.world->skyColor = glm::vec3(0.969, 0.929, 0.831);
        break;
    case Var::Preferences::Theme::Dark:
        ImGui::StyleColorsDark();
        state.world->skyColor = glm::vec3(0.1, 0.1, 0.1);
        break;
    case Var::Preferences::Theme::Classic:
        ImGui::StyleColorsClassic();
        state.world->skyColor = glm::vec3(0.078, 0.078, 0.137);
        break;
    }

    ::ui::MenuBar::render();
    ::ui::Window::render();

    ImGui::Render(); // End of new frame
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ui::destroy() { }

void ui::Event::process(const SDL_Event& e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
    const ImGuiIO& io = ImGui::GetIO();

    Var.WantCaptureEvent = io.WantCaptureKeyboard || io.WantCaptureMouse;
}

void ui::MenuBar::render()
{
    if (ImGui::BeginMainMenuBar()) {
        ::ui::MenuBar::Edit::render();
        ::ui::MenuBar::View::render();
        ::ui::MenuBar::Help::render();
        ImGui::EndMainMenuBar();
    }
}

void ui::Window::render()
{
    Var.Window.VertexEditor.WindowFlags = 0;
    Var.Window.About.WindowFlags = ImGuiWindowFlags_NoResize;
    if (Var.Window_NoBackground) {
        Var.Window.VertexEditor.WindowFlags |= ImGuiWindowFlags_NoBackground;
        Var.Window.About.WindowFlags |= ImGuiWindowFlags_NoBackground;
    }

    ::ui::Window::VertexEditor::render();
    ::ui::Window::About::render();

#ifndef NDEBUG
    ::ui::Window::Demo::render();
#endif
}

void ui::MenuBar::Edit::render()
{
    if (ImGui::BeginMenu("Edit##menubar-edit")) {
        ::ui::MenuBar::Edit::Preferences::render();
        ImGui::EndMenu();
    }
}

void ui::MenuBar::Edit::Preferences::render()
{
    if (ImGui::BeginMenu("Preferences##menubar-edit-prefer")) {
        ImGui::MenuItem("Theme##menubar-edit-prefer-theme", nullptr, false, false);
        if (ImGui::MenuItem("Light", nullptr)) {
            Var.Preferences.Theme = ui::Var::Preferences::Theme::Light;
        };
        if (ImGui::MenuItem("Dark", nullptr)) {
            Var.Preferences.Theme = ui::Var::Preferences::Theme::Dark;
        };
        if (ImGui::MenuItem("Classic", nullptr)) {
            Var.Preferences.Theme = ui::Var::Preferences::Theme::Classic;
        };
        ImGui::Separator();
        ImGui::MenuItem("Window Options##menubar-edit-prefer-window_options", nullptr, false, false);
        ImGui::Checkbox("No background", &Var.Window_NoBackground);
        ImGui::EndMenu();
    }
}

void ui::MenuBar::View::render()
{
    if (ImGui::BeginMenu("View##menubar-view")) {
        ImGui::MenuItem("Vertex Editor", nullptr, &Var.Window.VertexEditor.Visible);
        ImGui::EndMenu();
    }
}

void ui::MenuBar::Help::render()
{
    if (ImGui::BeginMenu("Help")) {
#ifndef NDEBUG
        if (ImGui::MenuItem("Dear ImGui Demo", nullptr)) {
            Var.Window.Demo.Visible = true;
        }
#endif
        if (ImGui::MenuItem("About", nullptr)) {
            Var.Window.About.Visible = true;
        }
        ImGui::EndMenu();
    }
}

void ui::Window::VertexEditor::render()
{
    if (!Var.Window.VertexEditor.Visible)
        return;

    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
    ImGui::Begin("Vertex Editor", &Var.Window.VertexEditor.Visible, Var.Window.VertexEditor.WindowFlags);
    if (ImGui::BeginTabBar("TabBar##Window_VertexEditor")) {
        ::ui::Window::VertexEditor::ColorTab::render();
        ::ui::Window::VertexEditor::VertexTab::render();
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void ui::Window::VertexEditor::VertexTab::render()
{
    auto& verts = state.world->triangle->vertices;

    if (ImGui::BeginTabItem("Position")) {
        for (std::size_t i = 0; i < verts.size(); i++) {
            Vertex::Position& pos = verts[i].position;
            const GLintptr offset = i * sizeof(Vertex) + 0 * sizeof(Vertex::Position);
            const GLsizei size = sizeof(Vertex::Position);

            const std::string name = "Vertex " + std::to_string(i);
            if (ImGui::SliderFloat2(name.c_str(), pos.ptr(), -1.50f, 1.50f)) {
                gl::Buffer::subData(GL_ARRAY_BUFFER, offset, size, pos.ptr());
            }
        }
        ImGui::EndTabItem();
    }
}

void ui::Window::VertexEditor::ColorTab::render()
{
    auto& verts = state.world->triangle->vertices;

    if (ImGui::BeginTabItem("Color")) {
        for (std::size_t i = 0; i < verts.size(); i++) {
            Vertex::Color& color = verts[i].color;
            const GLintptr offset = i * sizeof(Vertex) + 1 * sizeof(Vertex::Position);
            const GLsizei size = sizeof(Vertex::Color);

            const std::string name = "Vertex " + std::to_string(i);
            if (ImGui::ColorEdit3(name.c_str(), color.ptr())) {
                gl::Buffer::subData(GL_ARRAY_BUFFER, offset, size, color.ptr());
            }
        }
        ImGui::EndTabItem();
    }
}

void ui::Window::About::render()
{
    if (!Var.Window.About.Visible)
        return;

    ImGui::Begin("About##Window_About", &Var.Window.About.Visible, Var.Window.About.WindowFlags);
    ImGui::SetWindowFontScale(1.2f);
    ImGui::Text("NTOU OpenGL Template - Triangle");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Text("Version %s", EXAMPLE_VERSION);
    if (ImGui::BeginTabBar("TabBar##Window_About")) {
        ::ui::Window::About::AboutTab::render();
        ::ui::Window::About::ComponentsTab::render();
        ::ui::Window::About::AuthorsTab::render();
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void ui::Window::About::AboutTab::render()
{
    if (ImGui::BeginTabItem("About##About")) {
        ImGui::BeginChild("Child##AboutAbout", Var.Window.About.ChildSize, true);
        ImGui::TextWrapped("An OpenGL template program for students learning Computer Graphics.\n"
                           "\n"
                           "Developed at:\n"
                           "Nation Taiwan Ocean University\n"
                           "\n"
                           "Copyright 2021, NTOU CSE 503 Authors\n");
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void ui::Window::About::ComponentsTab::render()
{
    if (ImGui::BeginTabItem("Components##About")) {
        ImGui::BeginChild("Child##AboutComponents", Var.Window.About.ChildSize, true);
        ImGui::Text("SDL2\n"
                    "Version %d.%d.%d\n"
                    "https://www.libsdl.org/\n",
                    SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
        ImGui::Dummy(ImVec2(10, 10));
        ImGui::Separator();
        ImGui::Text("Dear ImGui\n"
                    "Version %s\n"
                    "https://github.com/ocornut/imgui\n",
                    IMGUI_VERSION);
        ImGui::Dummy(ImVec2(10, 10));
        ImGui::Separator();
        ImGui::Text("Glad\n"
                    "Version 0.1.34\n"
                    "OpenGL 4.6 Core profile\n"
                    "https://glad.dav1d.de/\n");
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void ui::Window::About::AuthorsTab::render()
{
    if (ImGui::BeginTabItem("Authors##About")) {
        ImGui::BeginChild("Child##AboutAuthors", Var.Window.About.ChildSize, true);
        ImGui::Text("Kao Yü-chia <kogiokka.frms@gmail.com>\n");
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

#ifndef NDEBUG
void ui::Window::Demo::render()
{
    if (!Var.Window.Demo.Visible)
        return;
    ImGui::ShowDemoWindow(&Var.Window.Demo.Visible);
}
#endif
