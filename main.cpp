#include "picosystem.hpp"
#include <ctime>

using namespace picosystem;

enum state_t
{
  PLAYING,
  GAME_OVER
};

struct vec_t
{
  int32_t x, y;
};

struct Game 
{
  state_t state = PLAYING;
  bool button_a_was_pressed = false;
  int32_t score = 0;
  vec_t bounds = {18, 16};
  int32_t scale = 6;
  float speed = 0.0f;
  static constexpr float MAX_SPEED = 5.0f;

  void increase_speed() {
    speed = std::min(speed + 0.1f, MAX_SPEED);
  }
};

Game game;

struct Sound
{
  voice_t v;
  Sound(int32_t attack, int32_t decay, int32_t sustain, int32_t release, int32_t bend = 0, int32_t bend_ms = 0, int32_t reverb = 0, int32_t noise = 0, int32_t distort = 0) : v(voice(attack, decay, sustain, release, bend, bend_ms, reverb, noise, distort)) {}
  
  void play_sound(int32_t frequency, int32_t duration, int32_t volume = 100) {
    play(v, frequency, duration, volume);
  }
};

struct Birb
{
  vec_t dir;
  vec_t body = {1, 8};
  uint32_t last_update = 0;
  Sound flap_sound = Sound(250, 100, 40, 0, 0, 0, 0, 0, 0);
  Sound collision_sound = Sound(5, 500, 400, 600, 0, 0, 0, 0, 0);

  void flap(uint32_t tick)
  {
    dir.y = -1;
    last_update = tick;
    flap_sound.play_sound(500, 100, 40);
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
};

struct Pipe
{
  int32_t x_pos = game.bounds.x;
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

  int32_t bottom_pipe_height = game.bounds.y - (gap_pos + gap_size);

  void next()
  {
    x_pos -= 1;
    if (x_pos == 0)
    {
      game.score++;
    }
    if (x_pos == -2)
    {
      reset();
    }
  }

  int32_t generate_gap_pos()
  {
    return rand() % (game.bounds.y - gap_size);
  }

  void reset()
  {
    x_pos = game.bounds.x;
    gap_pos = generate_gap_pos();
    top_pipe_height = gap_pos;
    bottom_pipe_height = game.bounds.y - (gap_pos + gap_size);
  }
};




vec_t transform(vec_t v)
{
  return {.x = (v.x * game.scale) + 6, .y = (v.y * game.scale) + 18};
}

Birb birb = Birb();
Pipe pipe = Pipe();

bool collision()
{
  return birb.body.y < 0 || birb.body.y >= game.bounds.y ||
         birb.body.x < 0 || birb.body.x >= game.bounds.x ||
         (birb.body.x == pipe.x_pos &&
          (birb.body.y < pipe.top_pipe_height - 1 ||
           birb.body.y > pipe.bottom_pipe_start().y));
}

void init()
{
  game.state = PLAYING;
  game.score = 0;
  game.speed = 0;

  birb.dir = {.x = 0, .y = 1};
  birb.body = {.x = 1, .y = 8};

  srand(time(0));
  pipe.reset();
}

void update(uint32_t tick)
{
  if (game.state == PLAYING)
  {
    if (!game.button_a_was_pressed && button(A))
    {
      birb.flap(tick);
    }

    if (tick % int(10 - game.speed) == 0)
    {
      pipe.next();
      if (tick > (birb.last_update + 10))
      {
        birb.fall();
      }
      if(game.score % 5 == 0 && game.score > 0) {
        game.increase_speed();
      }
    }

    // Check for collisions with the bounds
    if (collision())
    {
      birb.collision_sound.play_sound(200, 1000, 40);
      game.state = GAME_OVER;
    }

    game.button_a_was_pressed = button(A);
  }
  else
  {
    text("GAME OVER!", 15, 30);
    text("Score: " + std::to_string(game.score), 15, 40);
    text("Press A to restart", 15, 50);
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

  // Draw the birb
  pen(15, 14, 1);
  vec_t birb_screen_pos = transform(birb.body);              // Transform grid position to screen position
  frect(birb_screen_pos.x, birb_screen_pos.y, game.scale, game.scale); // Draw the birb as a rectangle

  // Draw the pipes
  pen(0, 255, 0); // Green color for pipes
  vec_t top_pipe_screen_pos = transform(pipe.top_pipe_start());
  vec_t bottom_pipe_screen_pos = transform(pipe.bottom_pipe_start());
  frect(top_pipe_screen_pos.x, top_pipe_screen_pos.y, game.scale, pipe.top_pipe_height * game.scale);
  frect(bottom_pipe_screen_pos.x, bottom_pipe_screen_pos.y, game.scale, (pipe.bottom_pipe_height + 1) * game.scale);

  // draw the scoreboard
  pen(0, 0, 0, 4);
  hline(2, 12, 116);
  text(std::to_string(game.score), 3, 3);
}
