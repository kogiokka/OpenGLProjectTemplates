#pragma once

#include <SDL.h>

namespace UI
{
    struct Var
    {
        bool GrabbingEvent = false;
        bool Window_NoBackground = false;

#ifndef NDEBUG
        struct DemoWindow
        {
            bool Visible = false;
        } DemoWindow;
#endif
        struct VertexEditor
        {
            bool Visible = false;
            int WindowFlags = 0;
        } VertexEditor;
    };

    extern struct Var Var;
    void create();
    void destroy();
    void render();
    void processEvent(const SDL_Event& event);
}
