#include <iostream>

#include <raylib.h>
#include <rlgl.h>

class Player {
public:
  float x;
  float y;
  float angle;
  Player(float x, float y) : x(x), y(y), angle(0) {}
};

int main() {
  Vector2 screenSize { 800, 600 };
  InitWindow(screenSize.x, screenSize.y, "GAME !!!");

  Player player{ screenSize.x/2, screenSize.y/2 };

  Camera2D camera{ 0 };
  //camera.target = {player.x() - screenSize.x/2, player.y() - screenSize.y/2};
  //camera.target = {player.x(), player.y()};
  camera.zoom = 1.0f;


  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    BeginMode2D(camera);

    if (IsKeyDown(KEY_A)) player.x -= 1;
    if (IsKeyDown(KEY_D)) player.x += 1;
    if (IsKeyDown(KEY_W)) player.y -= 1;
    if (IsKeyDown(KEY_S)) player.y += 1;

    if (IsKeyDown(KEY_Q)) player.angle += 1.3;
    if (IsKeyDown(KEY_E)) player.angle -= 1.3;


    std::cout << player.x << ' ' << player.y << ' ' << player.angle << '\n';

    //DrawRectangle(-10, 10, 100, 100, BLACK);

    rlPushMatrix();
    
    rlTranslatef(player.x, player.y, 0.0f);
    rlRotatef(player.angle, 0.0f, 0.0f, -1.0f);

    DrawTriangle(
      {-20.0f,  20.0f},
      { 20.0f,  20.0f},
      {  0.0f, -20.0f},
      BLACK
    );

    // DrawTriangle(
    //   {player.x - 20.0f, player.y + 20.0f},
    //   {player.x + 20.0f, player.y + 20.0f},
    //   {player.x        , player.y - 20.0f},
    //   BLACK
    // );
    
    // DrawTriangle(
    //   {50 + player.x        , player.y - 20.0f},
    //   {50 + player.x - 20.0f, player.y + 20.0f},
    //   {50 + player.x + 20.0f, player.y + 20.0f},
    
    //   GREEN
    // );
    
    // DrawTriangle(
    //   {-50 + player.x + 20.0f, player.y + 20.0f},
    //   {-50 + player.x        , player.y - 20.0f},
    //   {-50 + player.x - 20.0f, player.y + 20.0f},
    //   RED
    // );



    // DrawCircle(player.x, player.y, 10.0f, RED);

    rlPopMatrix();

    //DrawRectangle(-10, 300, 100, 100, BLACK);


    EndMode2D();
    EndDrawing();
  }
  
  return 0;
}