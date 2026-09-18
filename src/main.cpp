#include <raylib.h>
#include <entt/entt.hpp>

int main()
{
    InitWindow(1280, 720, "MyGame");

    entt::registry registry;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            "Hello World",
            100,
            100,
            30,
            WHITE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}