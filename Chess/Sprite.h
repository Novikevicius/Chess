#pragma once
#include <string>
#include <SDL.h>
#include "Vector2.h"

class Sprite
{
public:
    Sprite ();
    Sprite(const std::string& fileLocation);
    Sprite(SDL_Surface& surf);
    Sprite(Uint32 color, int w, int h);
    Sprite(const Sprite& sprite);
    ~Sprite();
private:
    SDL_Texture* image;
    int width;
    int height;
    SDL_Texture* CopyTexture(SDL_Texture* src, int w, int h);
public:
    Sprite& operator=(const Sprite& s);
	void Draw(const Vec2D<unsigned>& pos, const Vec2D<float>& sizeMultiplier = Vec2D<float>(1, 1)) const;
    void Draw(unsigned int x, unsigned int y) const;
    int  GetWidth()		    const;
    int  GetHeight()	    const;
    const SDL_Texture* GetSurface() const;
};

