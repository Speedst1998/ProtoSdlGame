// Using SDL and standard IO
#include "LTexture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
LTexture *gBackgroundTexture = NULL;
LTexture *gForegroundTexture = NULL;

int main(int argc, char *args[]) {
  bool quit = false;
  SDL_Event e;

  if (!init()) {
    printf("Initialization has failed\n");
  } else if (!loadMedia()) {
    printf("Loading Media Failed :(\n ");
  } else {

    while (!quit) {

      while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
          quit = true;
        }
      }

      SDL_RenderClear(gRenderer);
      gBackgroundTexture->render(0, 0);
      gForegroundTexture->render(240, 190);
      SDL_RenderPresent(gRenderer);
    }
  }

  // Destroy window
  close();

  return 0;
}

bool init() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  } else {
    printf("SDL Initialized. Creating Window\n");
    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == NULL) {
      printf("Renderer could not be created ! SDL_Error:%s\n", SDL_GetError());
      return false;
    }

    printf("Created Renderer.\n");

    gForegroundTexture = new LTexture(gRenderer);
    gBackgroundTexture = new LTexture(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
      printf("SDL_image could not initialize! SDL_image Error: %s\n",
             IMG_GetError());
      return false;
    }
    printf("SDL_IMAGE Initialized\n");

    return true;
  }
}

bool loadMedia() {
  bool success = true;

  // Load Foo' texture
  if (!gForegroundTexture->loadFromFile("resources/10_color_keying/foo.png")) {
    printf("Failed to load Foo' texture image!\n");
    success = false;
  }

  // Load background texture
  if (!gBackgroundTexture->loadFromFile("resources/10_color_keying/background.png")) {
    printf("Failed to load background texture image!\n");
    success = false;
  }

  return success;
}

void close() {

  gForegroundTexture->free();
  gForegroundTexture = NULL;

  gBackgroundTexture->free();
  gBackgroundTexture = NULL;

  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

SDL_Texture *loadTexture(std::string path) {

  SDL_Texture *texture = NULL;
  // Load splash image
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
    return NULL;
  } else {
    texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == NULL) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
      return NULL;
    }
  }

  return texture;
}
