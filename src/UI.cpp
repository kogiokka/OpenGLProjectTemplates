#include "UI.hpp"
#include "State.hpp"
#include "gl/Buffer.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include <string>

namespace UI
{
    struct Var Var;
    void render();

    namespace MenuBar
    {
        void render();
    }
    namespace VertexEditor
    {
        namespace VertexTab
        {
            void render();
        }
        namespace ColorTab
        {
            void render();
        }
        void render();
    }

#ifndef NDEBUG
    namespace DemoWindow
    {
        void render();
    }
#endif
}

void UI::create()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(state.window->handle, state.window->glContext);
    ImGui_ImplOpenGL3_Init();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
}

void UI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(state.window->handle);

    ImGui::NewFrame();
    ::UI::MenuBar::render();

    Var.VertexEditor.WindowFlags = 0;
    if (Var.Window_NoBackground)
    {
        Var.VertexEditor.WindowFlags |= ImGuiWindowFlags_NoBackground;
    }

#ifndef NDEBUG
    if (Var.DemoWindow.Visible)
    {
        ::UI::DemoWindow::render();
    }
#endif

    if (Var.VertexEditor.Visible)
    {
        ::UI::VertexEditor::render();
    }

    ImGui::Render(); // End of new frame
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::processEvent(const SDL_Event& e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
    const ImGuiIO& io = ImGui::GetIO();

    Var.GrabbingEvent = io.WantCaptureKeyboard || io.WantCaptureMouse;
}

void UI::destroy() { }

void UI::MenuBar::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Preferences"))
            {
                ImGui::MenuItem("ImGui Window", nullptr, false, false);
                ImGui::Checkbox("No background", &Var.Window_NoBackground);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Vertex Editor", nullptr, &Var.VertexEditor.Visible);
#ifndef NDEBUG
            ImGui::MenuItem("Dear ImGui Demo", nullptr, &Var.DemoWindow.Visible);
#endif
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

#ifndef NDEBUG
void UI::DemoWindow::render() { ImGui::ShowDemoWindow(&Var.DemoWindow.Visible); }
#endif

void UI::VertexEditor::render()
{
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
    ImGui::Begin("Vertex Editor", &Var.VertexEditor.Visible, Var.VertexEditor.WindowFlags);
    if (ImGui::BeginTabBar("##vertex-editor-tabs"))
    {
        ColorTab::render();
        VertexTab::render();
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void UI::VertexEditor::VertexTab::render()
{
    auto& triangle = state.scene->triangle;

    if (ImGui::BeginTabItem("Position"))
    {
        for (std::size_t i = 0; i < triangle.size(); i++)
        {
            Vertex::Position& pos = triangle[i].position;
            const GLintptr offset = i * sizeof(Vertex) + 0 * sizeof(Vertex::Position);
            const GLsizei size = sizeof(Vertex::Position);

            const std::string name = "Vertex " + std::to_string(i);
            if (ImGui::SliderFloat2(name.c_str(), pos.ptr(), -1.50f, 1.50f))
            {
                gl::Buffer::subData(GL_ARRAY_BUFFER, offset, size, pos.ptr());
            }
        }
        ImGui::EndTabItem();
    }
}

void UI::VertexEditor::ColorTab::render()
{
    auto& triangle = state.scene->triangle;

    if (ImGui::BeginTabItem("Color"))
    {
        for (std::size_t i = 0; i < triangle.size(); i++)
        {
            Vertex::Color& color = triangle[i].color;
            const GLintptr offset = i * sizeof(Vertex) + 1 * sizeof(Vertex::Position);
            const GLsizei size = sizeof(Vertex::Color);

            const std::string name = "Vertex " + std::to_string(i);
            if (ImGui::ColorEdit3(name.c_str(), color.ptr()))
            {
                gl::Buffer::subData(GL_ARRAY_BUFFER, offset, size, color.ptr());
            }
        }
        ImGui::EndTabItem();
    }
}
