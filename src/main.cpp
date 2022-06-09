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

LTexture *gSpriteSheetTexture = NULL;
SDL_Rect gSpriteClips[4];

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
      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

      SDL_RenderClear(gRenderer);
      // Render top left sprite
      gSpriteSheetTexture->render(0, 0, &gSpriteClips[0]);

      // Render top right->sprite
      gSpriteSheetTexture->render(SCREEN_WIDTH - gSpriteClips[1].w, 0,
                                  &gSpriteClips[1]);

      // Render bottom let sprite
      gSpriteSheetTexture->render(0, SCREEN_HEIGHT - gSpriteClips[2].h,
                                  &gSpriteClips[2]);

      // Render bottom right sprite
      gSpriteSheetTexture->render(SCREEN_WIDTH - gSpriteClips[3].w,
                                  SCREEN_HEIGHT - gSpriteClips[3].h,
                                  &gSpriteClips[3]);
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

    gSpriteSheetTexture = new LTexture(gRenderer);
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

  if (!gSpriteSheetTexture->loadFromFile(
          "resources/11_clip_rendering_and_sprite_sheets/dots.png")) {
    printf("Failed to load background texture image!\n");
    success = false;
  } else {
    // Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 100;
    gSpriteClips[0].h = 100;

    // Set top right sprite
    gSpriteClips[1].x = 100;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = 100;
    gSpriteClips[1].h = 100;

    // Set bottom left sprite
    gSpriteClips[2].x = 0;
    gSpriteClips[2].y = 100;
    gSpriteClips[2].w = 100;
    gSpriteClips[2].h = 100;

    // Set bottom right sprite
    gSpriteClips[3].x = 100;
    gSpriteClips[3].y = 100;
    gSpriteClips[3].w = 100;
    gSpriteClips[3].h = 100;
  }

  return success;
}

void close() {

  gSpriteSheetTexture->free();
  gSpriteSheetTexture = NULL;

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
