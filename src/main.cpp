// Using SDL and standard IO
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

// Loads surface
SDL_Surface *loadSurface(bool &success, std::string path);

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface *gHelloWorld = NULL;

// Key press surfaces constants
enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface; 

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
        } else if (e.type == SDL_KEYDOWN) {
          // Select surfaces based on key press
          switch (e.key.keysym.sym) {
          case SDLK_UP:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
            break;

          case SDLK_DOWN:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
            break;

          case SDLK_LEFT:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
            break;

          case SDLK_RIGHT:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
            break;

          default:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            break;
          }
        }
      }
      SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
      SDL_UpdateWindowSurface(gWindow);
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
    // Get window surface
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
  }
}

bool loadMedia() {
  bool success = true;
  
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface(success, "resources/04_key_presses/press.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface(success, "resources/04_key_presses/up.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface(success, "resources/04_key_presses/down.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface(success, "resources/04_key_presses/left.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface(success, "resources/04_key_presses/right.bmp");
  return success;
}

void close() {
  // Deallocate surface
  SDL_FreeSurface(gHelloWorld);
  gHelloWorld = NULL;

  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}

SDL_Surface *loadSurface(bool &success, std::string path) {

  // Load splash image
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
    success = false;
  }
  return surface;
}
