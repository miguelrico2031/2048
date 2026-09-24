#include "core/app/App.h"

#include "app/board/BackgroundTilesSystem.h"
#include "app/board/BoardTilesSystem.h"
#include "app/board/SpawnNewTilesSystem.h"
#include "app/movement/MoveBoardSystem.h"
#include "app/movement/MoveTilesSystem.h"
#include "app/session/StartSessionSystem.h"
#include "app/turn/CheckGameOverSystem.h"
#include "app/turn/ProcessTurnActionSystem.h"

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

    app.m_Initializers.push_back(ttfe::board::BackgroundTilesInitializer);
    app.m_Initializers.push_back(ttfe::board::BoardTilesInitializer);

    app.m_Systems.push_back(ttfe::session::StartSessionSystem);
    app.m_Systems.push_back(ttfe::board::BackgroundTilesSystem);
    app.m_Systems.push_back(ttfe::board::BoardTilesSystem);
    app.m_Systems.push_back(ttfe::turn::ProcessTurnActionSystem);
    app.m_Systems.push_back(ttfe::move::MoveBoardSystem);
    app.m_Systems.push_back(ttfe::turn::CheckGameOverSystem);
    app.m_Systems.push_back(ttfe::board::SpawnNewTilesSystem); //dependency: must be after BoardTilesSystem to have the updated tiles to check availables
    app.m_Systems.push_back(ttfe::move::MoveTilesSystem); //dependency: must be after SpawnNewTilesSystem so the spawn reads the updated moved coords

    core::Run(app);

    return 0;
}