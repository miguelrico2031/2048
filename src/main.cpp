#include "core/app/App.h"

#include "app/Test.h"
#include "app/board/BackgroundTilesSystem.h"
#include "app/board/BoardTilesSystem.h"
#include "app/board/SpawnNewTilesSystem.h"
#include "app/session/StartSessionSystem.h"

int main()
{
    core::App app{
        .m_Data{
            .m_Name = "My App",
            .m_WindowWidth = 1280,
            .m_WindowHeight = 720,
            .m_ClearColor = RAYWHITE,
        }
    };

    app.m_Initializers.push_back(TestInit);
    app.m_Initializers.push_back(ttfe::board::BackgroundTilesInitializer);
    app.m_Initializers.push_back(ttfe::board::BoardTilesInitializer);

    app.m_Systems.push_back(TestSystem);
    app.m_Systems.push_back(ttfe::session::StartSessionSystem);
    app.m_Systems.push_back(ttfe::board::SpawnNewTilesSystem);
    app.m_Systems.push_back(ttfe::board::BackgroundTilesSystem);
    app.m_Systems.push_back(ttfe::board::BoardTilesSystem);

    core::Run(app);

    return 0;
}