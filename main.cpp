#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    // Set Window Dimensions (Width / Height)
    const int windowWidth{384};
    const int windowHeight{384};

    // Initialize the Window
    InitWindow(windowWidth, windowHeight, "Top Down RPG");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight(windowWidth, windowHeight);

    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")},
    };

    // Create an instance of Enemy Class goblin
    Enemy goblin(
        Vector2{800.f, 300.f},
        Texture2D(LoadTexture("characters/goblin_idle_spritesheet.png")),
        Texture2D(LoadTexture("characters/goblin_run_spritesheet.png")));

    // Create an instance of Enemy Class slime
    Enemy slime(
        Vector2{500.f, 700.f},
        Texture2D(LoadTexture("characters/slime_idle_spritesheet.png")),
        Texture2D(LoadTexture("characters/slime_run_spritesheet.png")));

    // Create array of enemies
    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // Set Map position to opposite of Knight World Pos (move the map opposite direction so knight looks to be moving)
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // Draw the Map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // Draw the props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        // Draw the character health
        if (!knight.getAlive()) // character is not alive case
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else // character is alive case
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        // Draw and animate knight
        knight.tick(GetFrameTime());

        // Check Map Bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // Check Prop Collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        // Draw and animate enemy
        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        // Check for weapon collision
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}