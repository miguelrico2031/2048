#include "core/App.h"

#include "core/input/InputEventSystem.h"
#include "core/sprite/SpriteRenderingSystem.h"
#include <raylib.h>

void core::Run(core::App& app)
{
    InitWindow(app.m_WindowWidth, app.m_WindowHeight, app.m_Name.c_str());

    for (const auto& initializer : app.m_Initializers)
    {
        initializer(app.m_Registry);
    }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        core::input::InputEventSystem(app.m_Registry, dt);

        for (const auto& system : app.m_Systems)
        {
            system(app.m_Registry, dt);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        
        core::sprite::SpriteRenderingSystem(app.m_Registry, dt);

        EndDrawing();
    }
    CloseWindow();
}