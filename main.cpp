#include <iostream>
#include <queue>

#include <math.h>

#include <raylib.h>
#include <rlgl.h>

class Laserbeam {
public:
  float x;
  float y;
  float speed;
  float angle;
  float length;
  float width;
  float height;
  Laserbeam(float x, float y, float angle) : x(x), y(y), speed(7.0f), 
  angle(angle), length(10.0f), width(10.0f), height(2.0f) {}
};

class Player {
public:
  float x;
  float y;
  float x_velocity;
  float y_velocity;
  float angle;
  float acceleration;
  float max_velocity;
  std::queue<Laserbeam> lasers;
  Player(float x, float y) : x(x), y(y), x_velocity(0), y_velocity(0),
  angle(90.0f), acceleration(0.1f), max_velocity(10.0f), lasers() {}
};

int main() {
  Vector2 screenSize { 1400, 1050 };
  InitWindow(screenSize.x, screenSize.y, "GAME !!!");

  Player player{ screenSize.x/2, screenSize.y/2 };

  Camera2D camera{ 0 };
  camera.zoom = 1.0f;

  Laserbeam laserbeam {0, 0, 0};

  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    BeginMode2D(camera);

    if (IsKeyDown(KEY_W)) {
      player.x_velocity += std::cos(player.angle * PI/180.0) * player.acceleration;
      player.y_velocity -= std::sin(player.angle * PI/180.0) * player.acceleration;
    } 
    if (IsKeyDown(KEY_A)) {
      player.x_velocity -= std::sin(player.angle * PI/180.0) * player.acceleration;
      player.y_velocity -= std::cos(player.angle * PI/180.0) * player.acceleration;
    }
    if (IsKeyDown(KEY_S)) {
      player.x_velocity -= std::cos(player.angle * PI/180.0) * player.acceleration;
      player.y_velocity += std::sin(player.angle * PI/180.0) * player.acceleration;
    }
    if (IsKeyDown(KEY_D)) {
      player.x_velocity += std::sin(player.angle * PI/180.0) * player.acceleration;
      player.y_velocity += std::cos(player.angle * PI/180.0) * player.acceleration;
    }

    if (player.x_velocity > player.max_velocity) player.x_velocity = player.max_velocity;
    if (player.y_velocity > player.max_velocity) player.y_velocity = player.max_velocity;

    player.x += player.x_velocity;
    player.y += player.y_velocity;

    if (IsKeyDown(KEY_Q)) player.angle += 2.0;
    if (IsKeyDown(KEY_E)) player.angle -= 2.0;
    
    if (IsKeyDown(KEY_J)) laserbeam = Laserbeam(
      player.x + std::cos(laserbeam.angle * PI/180.0) * laserbeam.width,
      player.y - std::sin(laserbeam.angle * PI/180.0) * laserbeam.width,
      player.angle);
    laserbeam.x += std::cos(laserbeam.angle * PI/180.0) * laserbeam.speed;
    laserbeam.y -= std::sin(laserbeam.angle * PI/180.0) * laserbeam.speed;


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
      { 20.0f,   0.0f},
      {-20.0f, -20.0f},
      {-20.0f,  20.0f},
      BLACK
    );


    rlPopMatrix();

    rlPushMatrix();
    rlTranslatef(laserbeam.x, laserbeam.y, 0.0f);
    rlRotatef(laserbeam.angle, 0.0f, 0.0f, -1.0f);

    DrawRectangle(0, 0, laserbeam.width, laserbeam.height, RED);
    rlPopMatrix();

    EndMode2D();
    EndDrawing();
  }
  
  return 0;
}