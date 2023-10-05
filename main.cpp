#include <iostream>
#include <vector>
#include <deque>
#include <tuple>

#include <cstdlib>
#include <math.h>

#include <raylib.h>
#include <rlgl.h>

#define DDDD_DEBUG

Vector2 screenSize { 1400, 1050 };

std::tuple<float, float> normalize_vec(float max_velo, float x_velo, float y_velo) {
  float total_velo = std::abs(x_velo) + std::abs(y_velo);
  float norm_factor = (total_velo > 10.0) ? std::abs(max_velo) / (total_velo) : 1.0f;
  return { x_velo * norm_factor, y_velo * norm_factor };
}

inline float deg2rad(float angle) {
  return angle * PI/180.0;
}

enum class Directions : int {
  forward = 0,
  left = 90,
  backward = 180,
  right = 270
};

class Laserbeam {
public:
  float x;
  float y;
  float width;
  float height;
  float speed;
  float angle;
  float length;
  Laserbeam(float x, float y, float width, float height, float angle) :
    x(x), y(y), width(width), height(height), speed(7.0f), 
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
  int laser_width;
  int laser_height;
  std::deque<Laserbeam> lasers;
  Player(float x, float y) : x(x), y(y), x_velocity(0), y_velocity(0),
    angle(90.0f), acceleration(0.1f), max_velocity(10.0f), laser_cooldown(0),
    laser_recharge_time(20), laser_width(10.0f), laser_height(2.0f), lasers() {}
  void control() {
    if (IsKeyDown(KEY_Q)) angle += 2.0;
    if (IsKeyDown(KEY_E)) angle -= 2.0;

    float next_x_velo { x_velocity };
    float next_y_velo { y_velocity };
    bool key_pressed { false };

    std::vector<Directions> thrusters;
    if (IsKeyDown(KEY_W)) {
      thrusters.push_back(Directions::forward);
      key_pressed = true;
    }
    if (IsKeyDown(KEY_A)) {
      thrusters.push_back(Directions::left);
      key_pressed = true;
    }
    if (IsKeyDown(KEY_S)) {
      thrusters.push_back(Directions::backward);
      key_pressed = true;
    }
    if (IsKeyDown(KEY_D)) {
      thrusters.push_back(Directions::right);
      key_pressed = true;
    }

    for (Directions d : thrusters) {
      next_x_velo += std::cos(deg2rad(angle + (float)d)) * (acceleration / thrusters.size());
      next_y_velo -= std::sin(deg2rad(angle + (float)d)) * (acceleration / thrusters.size());
    }

    if (key_pressed) {
      std::tie(x_velocity, y_velocity) = normalize_vec(max_velocity, next_x_velo, next_y_velo);
    }

    if (!laser_cooldown && IsKeyDown(KEY_J)) {
      lasers.push_back(
        Laserbeam(
        x + std::cos(angle * PI/180.0) * laser_width,
        y - std::sin(angle * PI/180.0) * laser_width,
        laser_width,
        laser_height,
        angle)
      );
      laser_cooldown = laser_recharge_time;
    } else if (laser_cooldown) {
      --laser_cooldown;
    }

  }
};

class Asteroid {
public:
  constexpr static float max_velocity {10.0f};
  float x;
  float y;
  float x_velocity;
  float y_velocity;
  float radius;
  float hp;
  Asteroid() : x(0.0f), y(0.0f), x_velocity(1.0f), y_velocity(2.0f),
    radius(3.0f), hp(1) {}
  Asteroid(float x, float y, float x_velocity, float y_velocity, float radius) :
    x(x), y(y), x_velocity(x_velocity), y_velocity(y_velocity), radius(radius),
    hp((radius >= 3.0f) ? radius / 3.0f : 1.0f) {}
  void hit() {
    --hp;
    radius -= 3.0f;   //TODO: I think this is potentially dangerous
  }
};

Asteroid randomAsteroid(float max_radius) {
  float x { rand() % (int)screenSize.x };
  float y { rand() % (int)screenSize.y };
  float x_velocity { (rand() % 3) + 1};   //TODO: make this better.
  float y_velocity { (rand() % 3) + 1};
  float radius { (rand() % (int)max_radius) + 1} ;
  return Asteroid(x, y, x_velocity, y_velocity, radius);
}


template <typename T>
void update_position(T &object) {
  object.x += object.x_velocity;
  object.y += object.y_velocity;
}

template <typename T>
void screenwrap(T &object) {
  if (object.x > screenSize.x) object.x -= screenSize.x;
  else if (object.x < 0.0f) object.x = screenSize.x - object.x;

  if (object.y > screenSize.y) object.y -= screenSize.y;
  else if (object.y < 0.0f) object.y = screenSize.y - object.y;
}


int main() {
  std::cout << __func__ << '\n';

  InitWindow(screenSize.x, screenSize.y, "GAME !!!");

  Player player{ screenSize.x/2, screenSize.y/2 };

  Camera2D camera{ 0 };
  camera.offset = {screenSize.x/2, screenSize.y/2};
  camera.target = camera.offset;
  camera.zoom = 1.0f;

  //Asteroid asteroid;

  std::vector<Asteroid> asteroids;
  for (int i = 0; i < 3; ++i) {
    asteroids.push_back(randomAsteroid(10.0f));
  }



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

    player.control();

    //std::cout << "x_velocity: " << player.x_velocity << " y_velocity: " << player.y_velocity << " abs(total): " << std::abs(player.x_velocity) + std::abs(player.y_velocity) << "\n";

    // player.x += player.x_velocity;
    // player.y += player.y_velocity;


    update_position(player);



    std::erase_if(player.lasers, [/*&screenSize*/](Laserbeam &l){
      return l.x < 0 || l.x > screenSize.x || l.y < 0 || l.y > screenSize.y; });


    for (auto &l : player.lasers) {
      l.x += std::cos(l.angle * PI/180.0) * l.speed;
      l.y -= std::sin(l.angle * PI/180.0) * l.speed;

      //if (CheckCollisionCircleRec({asteroid.x, asteroid.y}, asteroid.radius, {l.x, l.y, l.width, l.height})) {
      //  --asteroid.hp;
      //}
      for (Asteroid &a : asteroids) {
        if (CheckCollisionCircleRec({a.x, a.y}, a.radius, {l.x, l.y, l.width, l.height})) {
          a.hit();
        }
      }
    }
    

    screenwrap(player);

    //screenwrap(asteroid);
    //update_position(asteroid);

    std::erase_if(asteroids, [](Asteroid &a){ return a.hp <= 0.0f; });
    for (Asteroid &a : asteroids) {
      screenwrap(a);
      update_position(a);
    }


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
    
    //if (asteroid.hp) DrawCircle(asteroid.x, asteroid.y, asteroid.radius, BLACK);
    for (Asteroid &a : asteroids) {
      DrawCircle(a.x, a.y, a.radius, BLACK);
    }

    EndMode2D();
    EndDrawing();
  }
  
  return 0;
}