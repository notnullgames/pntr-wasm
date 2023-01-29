// temporary: just making sure all the plumbing works

#include "null0.h"

#include <SDL.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <CART>\n", argv[0]);
    return 1;
  }

  int status = null0_load_cart(argv[1]);
  if (status != 0) {
    return status;
  }

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  SDL_Window* window = SDL_CreateWindow("pntr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, canvas->width, canvas->height, SDL_WINDOW_SHOWN);
  SDL_Event event;
  SDL_Surface* screen = SDL_GetWindowSurface(window);
  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(canvas->data, canvas->width, canvas->height, 8, canvas->pitch, SDL_PIXELFORMAT_ARGB8888);

  bool shouldClose = false;
  while (!shouldClose) {
    null0_update();
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
        case SDL_QUIT:
          shouldClose = true;
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              shouldClose = true;
              break;
          }
          break;
      }
    }

    // Render to the screen.
    SDL_BlitSurface(surface, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
  }

  SDL_FreeSurface(surface);
  null0_unload();
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
