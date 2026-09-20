#include "core/App.h"
#include "app/Test.h"

int main()
{
    core::App app{
        .m_Name = "My App",
        .m_WindowWidth = 1280,
        .m_WindowHeight = 720,
    };

    app.m_Initializers.push_back(TestInit);
    app.m_Systems.push_back(TestSystem);

    core::Run(app);

    return 0;
}