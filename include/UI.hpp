#pragma once

#include <SDL.h>
#include <imgui.h>

namespace ui
{
    struct Var {
        bool WantCaptureEvent = false;
        bool Window_NoBackground = false;

        struct Window {
            struct VertexEditor {
                bool Visible = true;
                int WindowFlags = 0;
            } VertexEditor;
            struct About {
                bool Visible = false;
                int WindowFlags = 0;
                ImVec2 ChildSize = ImVec2(350, 300);
            } About;
#ifndef NDEBUG
            struct Demo {
                bool Visible = false;
            } Demo;
#endif
        } Window;

        struct Preferences {
            enum Theme {
                Light,
                Dark,
                Classic,
            };
            Theme Theme = Theme::Dark;
        } Preferences;
    };

    extern struct Var Var;
    void create();
    void destroy();
    void render();

    namespace Event
    {
        void process(const SDL_Event& event);
    }
}
