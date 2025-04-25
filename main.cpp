#include "picosystem.hpp"

using namespace picosystem;

enum state_t
{
  PLAYING,
  GAME_OVER
};
state_t state = PLAYING;

int score = 0;

struct vec_t
{
  int32_t x, y;
};

constexpr vec_t bounds{.x = 18, .y = 16};
constexpr int scale = 6;

struct
{
  vec_t dir;
  vec_t body = {1, 8};

  vec_t next()
  {
    return {.x = body.x + dir.x, .y = body.y + dir.y};
  }
} birb;

vec_t transform(vec_t v)
{
  return {.x = (v.x * scale) + 6, .y = (v.y * scale) + 18};
}

void init()
{
  state = PLAYING;

  birb.dir = {.x = 0, .y = 1};
  birb.body = {.x = 1, .y = 8};
}

void update(uint32_t tick)
{
  if (state == PLAYING)
  {
    if (tick % 10 == 0)
    {
      birb.dir.y = 1;
      if (button(A))
      {
        score++;
        birb.dir.y = -1;
      }
      // Update the birb's position
      birb.body = birb.next();

      // Check for collisions with the bounds
      if (birb.body.x < 0 || birb.body.x >= bounds.x || birb.body.y < 0 || birb.body.y >= bounds.y)
      {
        state = GAME_OVER;
      }
    }
  }
  else
  {
    // game over, if user presses A then restart
    if (pressed(A))
    {
      init();
    }
  }
}

// draw the world
void draw(uint32_t tick)
{
  pen(6, 12, 15); // Sky blue color
  clear();
  pen(0, 0, 0, 4);

  // draw the scoreboard
  hline(2, 12, 116);
  text(std::to_string(score), 3, 3);

  // Draw the birb
  pen(15, 14, 1);
  vec_t screen_pos = transform(birb.body);         // Transform grid position to screen position
  frect(screen_pos.x, screen_pos.y, scale, scale); // Draw the birb as a rectangle
}