#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

enum {N_POINTS = 9*9*9};
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

float fov_factor = 640;

bool is_running = false;

void setup(void) {
  // TODO:
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

  color_buffer_texture = SDL_CreateTexture (
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  int point_count = 0;
  // start loading array of vectors
  // from -1 to 1 (in 9x9x9 cube)
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t new_point = {.x = x, .y = y, .z = z};
        cube_points[point_count++] = new_point;
      }
    }
  }
}

void process_input(void) {
  // TODO:
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      }
      break;
    default:
      break;
  }

}

// Function that receives a 3d vector and returns a projected 2d point
vec2_t project(vec3_t point) {
  vec3_t camera_location = {.x = 0, .y = 0, .z = -5};

  // point.x/(cam.z - point.z) = point.x'/(cam.z - screen.z)
  // point.x' = point.x * (cam.z - screen.z) / (cam.z - point.z)
  vec2_t projected_point = {
    .x = (fov_factor * ( 1 / (camera_location.z - point.z)) * point.x),
    .y = (fov_factor * ( 1 / (camera_location.z - point.z)) * point.y)
  };

  return projected_point;
}

void update(void) {
  // TODO:
  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = cube_points[i];

    vec2_t projected_point = project(point); 

    // save the projected 2d vector in the array of projected points
    projected_points[i] = projected_point;
  }
}

void render(void) {
  draw_grid();

  // loop all projected and render them
  for (int i = 0; i < N_POINTS; i++) {
    vec2_t projected_point = projected_points[i];
    draw_rect(projected_point.x + window_width / 2, projected_point.y + window_height / 2, 2, 2, 0xffffff00);
  }

  render_color_buffer();
  clear_color_buffer(0xff000000);

  SDL_RenderPresent(renderer);
}

int main() {
  is_running = initialize_window();

  setup();
  while(is_running){
    process_input();
    update();
    render();
  }

  destroy_window();
  
  return 0;
}
