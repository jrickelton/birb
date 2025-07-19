#include <ctime>
#include "picosystem.hpp"

using namespace picosystem;

enum state_t
{
  PLAYING,
  GAME_OVER
};

enum notes
{
  C4 = 261,
  D4 = 294,
  E4 = 329,
  F4 = 349,
  G4 = 392,
  A4 = 440,
  B4 = 494,
  C5 = 523
};

color_t my_spritesheet[256] = {
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x44ff,
    0x44ff,
    0x44ff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x44ff,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0x0000,
    0x0000,
    0x0000,
    0x44ff,
    0xffff,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x44ff,
    0x44ff,
    0x44ff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x44ff,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0x99f9,
    0x99f9,
    0x99f9,
    0x99f9,
    0xeefe,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xffff,
    0x00f0,
    0xffff,
    0xe3fe,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x44ff,
    0xffff,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xffff,
    0xffff,
    0xffff,
    0x0000,
    0xeefe,
    0x99f9,
    0x99f9,
    0xeefe,
    0x99f9,
    0xeefe,
    0x99f9,
    0xeefe,
    0x0000,
    0x0000,
    0xffff,
    0xffff,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xffff,
    0xffff,
    0xffff,
    0x00f0,
    0xffff,
    0xe3fe,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0x00f0,
    0xffff,
    0x00f0,
    0xeefe,
    0xeefe,
    0x99f9,
    0x99f9,
    0x99f9,
    0x99f9,
    0x99f9,
    0x99f9,
    0xeefe,
    0x0000,
    0xeefe,
    0xffff,
    0xffff,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0xffff,
    0xe3fe,
    0xffff,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0x99f9,
    0xeefe,
    0x99f9,
    0xeefe,
    0xeefe,
    0x0000,
    0xeefe,
    0xffff,
    0xeefe,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0xeefe,
    0xffff,
    0xffff,
    0xffff,
    0xffff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xffff,
    0xeefe,
    0xffff,
    0x0000,
    0xe3f5,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0xeefe,
    0xe3f5,
    0xeefe,
    0x0000,
    0x0000,
    0x0000,
    0xc2fc,
    0xe3fe,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xfcff,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xe3f5,
    0xe3f5,
    0xeefe,
    0xe3f5,
    0xeefe,
    0xeefe,
    0xe3f5,
    0xeefe,
};

enum sprite_index_t
{
  BIRB_FLAP = 0,
  BIRB_HORIZONTAL = 1,
  BIRB_FALL = 2,
  BIRB_GRAVE = 3
};

// Create a buffer_t for your spritesheet
buffer_t my_spritesheet_buffer = {
    32,             // width
    8,              // height
    my_spritesheet, // data pointer
    false};

struct vec_t
{
  int32_t x, y;
};

struct Sound
{
  voice_t v;
  Sound(int32_t attack, int32_t decay, int32_t sustain, int32_t release, int32_t bend = 0, int32_t bend_ms = 0, int32_t reverb = 0, int32_t noise = 0, int32_t distort = 0) : v(voice(attack, decay, sustain, release, bend, bend_ms, reverb, noise, distort)) {}

  void play_sound(int32_t frequency, int32_t duration, int32_t volume = 100)
  {
    play(v, frequency, duration, volume);
  }
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
  Sound point_score_sound = Sound(5, 150, 0, 50);
  Sound speed_up_sound = Sound(50, 200, 50, 300);

  void increase_speed()
  {
    speed = std::min(speed + 0.1f, MAX_SPEED);
    speed_up_sound.play_sound(notes::C4, 100, 40);
  }
};

Game game;

struct Birb
{
  vec_t dir;
  vec_t body = {1, 8};
  uint32_t last_update = 0;
  Sound flap_sound = Sound(250, 100, 40, 0, 0, 0, 0, 0, 0);
  Sound collision_sound = Sound(5, 500, 400, 600, 0, 0, 0, 0, 0);
  sprite_index_t sprite = BIRB_HORIZONTAL;

  void flap(uint32_t tick)
  {
    dir.y = -1;
    last_update = tick;
    flap_sound.play_sound(notes::F4, 100, 40);
    body = next();
    sprite = BIRB_FLAP;
  }

  void fall()
  {
    dir.y = 1;
    body = next();
    sprite = BIRB_HORIZONTAL;
  }

  void death()
  {
    if (body.y < game.bounds.y)
    {
      dir.y = 1;
      body = next();
      sprite = BIRB_FALL;
    }
    else
    {
      sprite = BIRB_GRAVE;
    }
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
      if (game.score % 5 == 0 && game.score > 0)
      {
        game.increase_speed();
      }
      else
      {
        game.point_score_sound.play_sound(notes::C4, 100, 40);
      }
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
    }

    // Check for collisions with the bounds
    if (collision())
    {
      birb.collision_sound.play_sound(notes::C4, 1000, 40);
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
    birb.death();
  }
}

// draw the world
void draw(uint32_t tick)
{
  pen(6, 12, 15); // Sky blue color
  clear();

  // Set the spritesheet buffer
  spritesheet(&my_spritesheet_buffer);

  // Draw the pipes
  pen(0, 255, 0); // Green color for pipes
  vec_t top_pipe_screen_pos = transform(pipe.top_pipe_start());
  vec_t bottom_pipe_screen_pos = transform(pipe.bottom_pipe_start());
  frect(top_pipe_screen_pos.x, top_pipe_screen_pos.y, game.scale, pipe.top_pipe_height * game.scale);
  frect(bottom_pipe_screen_pos.x, bottom_pipe_screen_pos.y, game.scale, (pipe.bottom_pipe_height + 1) * game.scale);

  // Draw the birb
  vec_t birb_screen_pos = transform(birb.body); // Transform grid position to screen position
  int y_offset = (birb.sprite == BIRB_GRAVE) ? -2 : 0;
  sprite(
      birb.sprite,
      birb_screen_pos.x,
      birb_screen_pos.y + y_offset);

  // draw the scoreboard
  pen(0, 0, 0, 4);
  hline(2, 12, 116);
  text(std::to_string(game.score), 3, 3);
}
