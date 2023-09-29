#include <iostream>
#include <deque>

#include <math.h>

#include <raylib.h>
#include <rlgl.h>

#define DDDD_DEBUG

class Laserbeam {
public:
  constexpr static float width { 10.0f };
  constexpr static float height { 2.0f };
  float x;
  float y;
  float speed;
  float angle;
  float length;
  Laserbeam(float x, float y, float angle) : x(x), y(y), speed(7.0f), 
  angle(angle), length(10.0f) {}
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
  std::deque<Laserbeam> lasers;
  Player(float x, float y) : x(x), y(y), x_velocity(0), y_velocity(0),
  angle(90.0f), acceleration(0.1f), max_velocity(10.0f), lasers() {}
};

int main() {
  std::cout << __func__ << '\n';

  Vector2 screenSize { 1400, 1050 };
  InitWindow(screenSize.x, screenSize.y, "GAME !!!");

  Player player{ screenSize.x/2, screenSize.y/2 };

  Camera2D camera{ 0 };
  camera.offset = {screenSize.x/2, screenSize.y/2};
  camera.target = camera.offset;
  camera.zoom = 1.0f;

  //Laserbeam laserbeam {0, 0, 0};

  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    BeginMode2D(camera);

    DrawRectangle(0, 0, screenSize.x, screenSize.y, WHITE);

    camera.zoom += ((float)GetMouseWheelMove()*0.05f);

#ifdef DDDD_DEBUG
    if (IsKeyDown(KEY_LEFT)) camera.target.x -= 3.0f;
    if (IsKeyDown(KEY_RIGHT)) camera.target.x += 3.0f;
    if (IsKeyDown(KEY_UP)) camera.target.y -= 3.0f;
    if (IsKeyDown(KEY_DOWN)) camera.target.y += 3.0f;
#endif

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
    
    if (IsKeyDown(KEY_J)) {
      //laserbeam =
      player.lasers.push_back(
        Laserbeam(
        player.x + std::cos(player.angle * PI/180.0) * Laserbeam::width,
        player.y - std::sin(player.angle * PI/180.0) * Laserbeam::width,
        player.angle)
      );
    }
    // laserbeam.x += std::cos(laserbeam.angle * PI/180.0) * laserbeam.speed;
    // laserbeam.y -= std::sin(laserbeam.angle * PI/180.0) * laserbeam.speed;
    std::erase_if(player.lasers, [&screenSize](Laserbeam &l){
      return l.x < 0 || l.x > screenSize.x || l.y < 0 || l.y > screenSize.y; });
    for (auto &l : player.lasers) {
      l.x += std::cos(l.angle * PI/180.0) * l.speed;
      l.y -= std::sin(l.angle * PI/180.0) * l.speed;
    }


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

    for (auto l : player.lasers) {
      rlPushMatrix();
      rlTranslatef(l.x, l.y, 0.0f);
      rlRotatef(l.angle, 0.0f, 0.0f, -1.0f);

      DrawRectangle(0, 0, l.width, l.height, RED);
      rlPopMatrix();
    }
    

    EndMode2D();
    EndDrawing();
  }
  
  return 0;
}