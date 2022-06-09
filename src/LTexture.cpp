
#include "LTexture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

LTexture::LTexture(SDL_Renderer *renderer) {
  this->renderer = renderer;
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture() { free(); }

// Loads image at specified path
bool LTexture::loadFromFile(std::string path) {
  free();

  SDL_Texture *texture = NULL;
  // Load splash image
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
    return false;
  }
  SDL_SetColorKey(surface, SDL_TRUE,
                  SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL) {
    printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
    return false;
  }
  mWidth = surface->w;
  mHeight = surface->h;
  SDL_FreeSurface(surface);

  mTexture = texture;

  return true;
}

// Deallocates texture
void LTexture::free() {
  if (mTexture != NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
  }
  mWidth = 0;
  mHeight = 0;
}

// Renders texture at given point
void LTexture::render(int x, int y, SDL_Rect *clip) {
  SDL_Rect destRect = {x, y, mWidth, mHeight};

  if(clip != NULL){
    destRect.h = clip->h;
    destRect.w = clip->w;
  }
  SDL_RenderCopy(renderer, mTexture, clip, &destRect);
}

// Gets image dimensions
int LTexture::getWidth() { return mWidth; }

int LTexture::getHeight() { return mHeight; }
