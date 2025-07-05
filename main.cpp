#include "picosystem.hpp"
#include <ctime>

using namespace picosystem;

enum state_t
{
  PLAYING,
  GAME_OVER
};
state_t state = PLAYING;

int score = 0;
bool button_a_was_pressed = false;

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
  uint32_t last_update = 0;

  void flap(uint32_t tick)
  {
    dir.y = -1;
    last_update = tick;
    body = next();
  }

  void fall()
  {
    dir.y = 1;
    body = next();
  }

  vec_t next()
  {
    return {.x = body.x + dir.x, .y = body.y + dir.y};
  }
} birb;

struct
{
  int32_t x_pos = bounds.x;
  int32_t gap_size = 4;
  int32_t gap_pos = generate_gap_pos();

  vec_t top_pipe_start()
  {
    return {.x = x_pos, .y = -1};
  }

  int32_t top_pipe_height = gap_pos;

  vec_t bottom_pipe_start()
  {
    return {.x = x_pos, .y = gap_pos + gap_size};
  }

  int32_t bottom_pipe_height = bounds.y - (gap_pos + gap_size);

  void next()
  {
    x_pos -= 1;
    if (x_pos == 0)
    {
      score++;
    }
    if (x_pos == -2)
    {
      reset();
    }
  }

  int32_t generate_gap_pos()
  {
    return rand() % (bounds.y - gap_size);
  }

  void reset()
  {
    x_pos = bounds.x;
    gap_pos = generate_gap_pos();
    top_pipe_height = gap_pos;
    bottom_pipe_height = bounds.y - (gap_pos + gap_size);
  }
} pipe;

vec_t transform(vec_t v)
{
  return {.x = (v.x * scale) + 6, .y = (v.y * scale) + 18};
}

bool collision() {
  return birb.body.y < 0 || birb.body.y >= bounds.y ||
         birb.body.x < 0 || birb.body.x >= bounds.x ||
         (birb.body.x == pipe.x_pos && 
          (birb.body.y < pipe.top_pipe_height - 1 || 
           birb.body.y > pipe.bottom_pipe_start().y));
          }

void init()
{
  state = PLAYING;

  birb.dir = {.x = 0, .y = 1};
  birb.body = {.x = 1, .y = 8};

  srand(time(0));
  pipe.reset();
}

void update(uint32_t tick)
{
  if (state == PLAYING)
  {
    if (!button_a_was_pressed && button(A))
    {
      birb.flap(tick);
    }

    if (tick % 10 == 0)
    {
      pipe.next();
      if (tick > (birb.last_update + 10))
      {
        birb.fall();
      }
    }

    // Check for collisions with the bounds
    if (collision())
    {
      score = 0;
      state = GAME_OVER;
    }

    button_a_was_pressed = button(A);
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
  vec_t birb_screen_pos = transform(birb.body);              // Transform grid position to screen position
  frect(birb_screen_pos.x, birb_screen_pos.y, scale, scale); // Draw the birb as a rectangle

  // Draw the pipes
  pen(0, 255, 0); // Green color for pipes
  vec_t top_pipe_screen_pos = transform(pipe.top_pipe_start());
  vec_t bottom_pipe_screen_pos = transform(pipe.bottom_pipe_start());
  frect(top_pipe_screen_pos.x, top_pipe_screen_pos.y, scale, pipe.top_pipe_height * scale);
  frect(bottom_pipe_screen_pos.x, bottom_pipe_screen_pos.y, scale, (pipe.bottom_pipe_height + 1) * scale);
}
