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
  int laser_cooldown;
  int laser_recharge_time;
  std::deque<Laserbeam> lasers;
  Player(float x, float y) : x(x), y(y), x_velocity(0), y_velocity(0),
    angle(90.0f), acceleration(0.1f), max_velocity(10.0f), laser_cooldown(0),
    laser_recharge_time(20), lasers() {}
};

std::tuple<float, float> normalize_vec(float max_velo, float x_velo, float y_velo) {
  float total_velo = std::abs(x_velo) + std::abs(y_velo);
  float norm_factor = (total_velo > 10.0) ? std::abs(max_velo) / (total_velo) : 1.0f;
  return { x_velo * norm_factor, y_velo * norm_factor };
}

int main() {
  std::cout << __func__ << '\n';

  Vector2 screenSize { 1400, 1050 };
  InitWindow(screenSize.x, screenSize.y, "GAME !!!");

  Player player{ screenSize.x/2, screenSize.y/2 };

  Camera2D camera{ 0 };
  camera.offset = {screenSize.x/2, screenSize.y/2};
  camera.target = camera.offset;
  camera.zoom = 1.0f;


  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    BeginMode2D(camera);

    DrawRectangle(0, 0, screenSize.x, screenSize.y, WHITE);


#ifdef DDDD_DEBUG
    camera.zoom += ((float)GetMouseWheelMove()*0.05f);

    if (IsKeyDown(KEY_LEFT)) camera.target.x -= 3.0f;
    if (IsKeyDown(KEY_RIGHT)) camera.target.x += 3.0f;
    if (IsKeyDown(KEY_UP)) camera.target.y -= 3.0f;
    if (IsKeyDown(KEY_DOWN)) camera.target.y += 3.0f;
#endif

    float next_x_velo { 0 };
    float next_y_velo { 0 };
    bool key_pressed { false };
    if (IsKeyDown(KEY_W)) {
      next_x_velo = player.x_velocity + std::cos(player.angle * PI/180.0) * player.acceleration;
      next_y_velo = player.y_velocity - std::sin(player.angle * PI/180.0) * player.acceleration;
      key_pressed = true;
    } 
    if (IsKeyDown(KEY_A)) {
      next_x_velo = player.x_velocity - std::sin(player.angle * PI/180.0) * player.acceleration;
      next_y_velo = player.y_velocity - std::cos(player.angle * PI/180.0) * player.acceleration;
      key_pressed = true;
    }
    if (IsKeyDown(KEY_S)) {
      next_x_velo = player.x_velocity - std::cos(player.angle * PI/180.0) * player.acceleration;
      next_y_velo = player.y_velocity + std::sin(player.angle * PI/180.0) * player.acceleration;
      key_pressed = true;
    }
    if (IsKeyDown(KEY_D)) {
      next_x_velo = player.x_velocity + std::sin(player.angle * PI/180.0) * player.acceleration;
      next_y_velo = player.y_velocity + std::cos(player.angle * PI/180.0) * player.acceleration;
      key_pressed = true;
    }

    if (key_pressed) {
      std::tie(player.x_velocity, player.y_velocity) = normalize_vec(player.max_velocity, next_x_velo, next_y_velo);
    }

    std::cout << "x_velocity: " << player.x_velocity << " y_velocity: " << player.y_velocity << " abs(total): " << std::abs(player.x_velocity) + std::abs(player.y_velocity) << "\n";

    player.x += player.x_velocity;
    player.y += player.y_velocity;

    if (IsKeyDown(KEY_Q)) player.angle += 2.0;
    if (IsKeyDown(KEY_E)) player.angle -= 2.0;
    
    if (!player.laser_cooldown && IsKeyDown(KEY_J)) {
      player.lasers.push_back(
        Laserbeam(
        player.x + std::cos(player.angle * PI/180.0) * Laserbeam::width,
        player.y - std::sin(player.angle * PI/180.0) * Laserbeam::width,
        player.angle)
      );
      player.laser_cooldown = player.laser_recharge_time;
    } else if (player.laser_cooldown) {
      --player.laser_cooldown;
    }

    std::erase_if(player.lasers, [&screenSize](Laserbeam &l){
      return l.x < 0 || l.x > screenSize.x || l.y < 0 || l.y > screenSize.y; });
    for (auto &l : player.lasers) {
      l.x += std::cos(l.angle * PI/180.0) * l.speed;
      l.y -= std::sin(l.angle * PI/180.0) * l.speed;
    }



    if (player.x > screenSize.x) player.x -= screenSize.x;
    else if (player.x < 0.0f) player.x = screenSize.x - player.x;

    if (player.y > screenSize.y) player.y -= screenSize.y;
    else if (player.y < 0.0f) player.y = screenSize.y - player.y;



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