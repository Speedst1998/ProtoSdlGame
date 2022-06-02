
#include <SDL2/SDL.h>
#include <string>

class LTexture
{
    public :
        //Initializes variables
        LTexture(SDL_Renderer *renderer);

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render( int x, int y );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture *mTexture;
        SDL_Renderer *renderer;
        //Image dimensions
        int mWidth;
        int mHeight;

};
