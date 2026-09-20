#include "core/app/App.h"

#include "core/input/InputEventSystem.h"
#include "core/helpers/EnttHelpers.h"
#include "core/gfx/GfxComponents.h"
#include "core/gfx/RenderingSystem.h"
#include <raylib.h>

void core::Run(core::App& app)
{
    InitWindow(app.m_Data.m_WindowWidth, app.m_Data.m_WindowHeight, app.m_Data.m_Name.c_str());
    SetTargetFPS(60);

    auto& dataSingleton = entt::get_singleton<core::app::DataSingletonComponent>(app.m_Registry);
    dataSingleton.m_Data = app.m_Data;

    auto& fontsSingleton = entt::get_singleton<core::gfx::FontsSingletonComponent>(app.m_Registry);
    fontsSingleton.m_LoadedFonts[0] = GetFontDefault();

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
        ClearBackground(app.m_Data.m_ClearColor);
        
        core::gfx::RenderingSystem(app.m_Registry, dt);

        EndDrawing();
    }
    CloseWindow();
}