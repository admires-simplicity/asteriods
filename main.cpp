#include <iostream>

#include <raylib.h>
#include <rlgl.h>

#define MAX_VELOCITY 10.0f

class Player {
public:
  float x;
  float y;
  float x_velocity;
  float y_velocity;
  float angle;
  Player(float x, float y) : x(x), y(y), x_velocity(0), y_velocity(0), angle(0) {}
};

int main() {
  Vector2 screenSize { 800, 600 };
  InitWindow(screenSize.x, screenSize.y, "GAME !!!");

  Player player{ screenSize.x/2, screenSize.y/2 };

  Camera2D camera{ 0 };
  camera.zoom = 1.0f;


  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    BeginMode2D(camera);

    if (IsKeyDown(KEY_A)) player.x_velocity -= 1;
    if (IsKeyDown(KEY_D)) player.x_velocity += 1;
    if (IsKeyDown(KEY_W)) player.y_velocity -= 1;
    if (IsKeyDown(KEY_S)) player.y_velocity += 1;

    if (IsKeyDown(KEY_Q)) player.angle += 1.3;
    if (IsKeyDown(KEY_E)) player.angle -= 1.3;


    player.x += player.x_velocity;
    player.y += player.y_velocity;

    //player.x = ((int)player.x % (int)screenSize.x) + (player.x - (float)(int)player.x);
    //player.y = ((int)player.y % (int)screenSize.y) + (player.y - (float)(int)player.y);

    if (player.x > screenSize.x) player.x -= screenSize.x;
    else if (player.x < 0.0f) player.x = screenSize.x - player.x;

    if (player.y > screenSize.y) player.y -= screenSize.y;
    else if (player.y < 0.0f) player.y = screenSize.y - player.y;


    //std::cout << player.x << ' ' << player.y << ' ' << player.angle << '\n';

    rlPushMatrix();
    
    rlTranslatef(player.x, player.y, 0.0f);
    rlRotatef(player.angle, 0.0f, 0.0f, -1.0f);

    DrawTriangle(
      {-20.0f,  20.0f},
      { 20.0f,  20.0f},
      {  0.0f, -20.0f},
      BLACK
    );


    rlPopMatrix();


    EndMode2D();
    EndDrawing();
  }
  
  return 0;
}