#include "Sprite.h"
#include <SDL_image.h>
#include <windows.h>
#include "MainWindow.h"
#include "Vector2.h"

Sprite::Sprite()
{
    image  = nullptr;
    width  = -1;
    height = -1;
}
Sprite::Sprite(const std::string& fileLocation)
{
    if(fileLocation.empty())
    {
        throw std::invalid_argument("Image file path cannot be an empty string");
    }
    SDL_Surface* unconverted = nullptr;
    char *dataPath = SDL_GetBasePath();
    unconverted = IMG_Load((dataPath + fileLocation).c_str());
    if(!unconverted)
    {
		throw std::invalid_argument("Could not load image: " + std::string(dataPath) + std::string(SDL_GetError()) );
    }
    image = SDL_CreateTextureFromSurface(MainWindow::GetRenderer(), unconverted);
    width  = unconverted->w;
    height = unconverted->h;
    SDL_FreeSurface(unconverted);
    SDL_free(dataPath);
}

Sprite::Sprite(SDL_Surface& surf)
{
	image = SDL_CreateTextureFromSurface(MainWindow::GetRenderer(), &surf);
	if(!image)
	{
		throw std::runtime_error("Error: " + std::string(SDL_GetError()));
	}
	width = surf.w;
	height = surf.h;
}

Sprite::Sprite(Uint32 color, int w, int h)
{
    if(w <= 0 || h <= 0)
    {
        throw std::out_of_range("Image's width and height must be positive numbers but they are: " + std::to_string(w) + " " + std::to_string(h) + "\n");
    }
    width  = w;
    height = h;
    SDL_Surface* temp = nullptr;
    // masks to get right color
    Uint32 r;
    Uint32 g;
    Uint32 b;
    Uint32 a;
#if SDL_BYTEORDER != SDL_BIG_ENDIAN
    r = 0xff000000;
    g = 0x00ff0000;
    b = 0x0000ff00;
    a = 0x000000ff;
#else
    r = 0x000000ff;
    g = 0x0000ff00;
    b = 0x00ff0000;
    a = 0xff000000;
#endif
    temp = SDL_CreateRGBSurface(0, w, h, 32, r, g, b, a);
    SDL_FillRect(temp, nullptr, color);
    if(!temp)
    {
        throw std::runtime_error("Could not create RGB surface: " + std::string(SDL_GetError()) + "\n");
    }
    image = SDL_CreateTextureFromSurface(MainWindow::GetRenderer(), temp);
    if(!image)
    {
		throw std::runtime_error("Could not create texture from surface in Sprite(Uint32 color, int w, int h):" + std::string(SDL_GetError()) + "\n");
    }
    SDL_FreeSurface(temp);
}
Sprite::Sprite(const Sprite & sprite)
{
    width  = sprite.width;
    height = sprite.height;
    image = CopyTexture(sprite.image, sprite.width, sprite.height);
    if(!image)
    {
        throw std::runtime_error("Could not copy texture");
    }
}
Sprite& Sprite::operator=(const Sprite& s)
{
    if(this != &s)
    {
        if(image)
        {
            SDL_DestroyTexture(image);
            image = nullptr;
        }
        width  = s.width;
        height = s.height;
        image = CopyTexture(s.image, s.width, s.height);
        if(!image)
        {
            throw std::runtime_error("Could not copy image with an assignment operator");
        }
    }
    return *this;
}
Sprite::~Sprite()
{
    if(image)
    {
        SDL_DestroyTexture(image);
        image = nullptr;
    }
}
int Sprite::GetWidth() const
{
    return width;
}
int Sprite::GetHeight() const
{
    return height;
}
const SDL_Texture* Sprite::GetSurface() const
{
    return image;
}
void Sprite::Draw(const Vec2D<unsigned int>& position, const Vec2D<float>& sizeMultiplier) const
{
	if (image && width > 0 && height > 0 && sizeMultiplier.x > 0 && sizeMultiplier.y > 0)
	{
		SDL_Rect pos = { static_cast<int>(position.x), static_cast<int>(position.y), width * sizeMultiplier.x, height * sizeMultiplier.y };
		SDL_SetRenderDrawBlendMode(MainWindow::GetRenderer(), SDL_BLENDMODE_NONE);
		SDL_RenderCopy(MainWindow::GetRenderer(), image, nullptr, &pos);
	}
	else if (!image)
	{
		throw std::invalid_argument("Cannot draw image: image is null\n");
	}
}
void Sprite::Draw(unsigned int xPos, unsigned int yPos) const
{
	Draw(Vec2D<unsigned int>(xPos, yPos), Vec2D<float>(1, 1));
}
SDL_Texture* Sprite::CopyTexture(SDL_Texture* src, int w, int h)
{
    if(!src)
        throw std::invalid_argument("Error copying texture: src is NULL\n");
    SDL_Texture* result = SDL_CreateTexture(MainWindow::GetRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, w, h);
    if(!result)
        throw std::runtime_error("Failed to create texture for copying\n");
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(MainWindow::GetRenderer(), result);
    SDL_RenderClear(MainWindow::GetRenderer());
    SDL_RenderCopy(MainWindow::GetRenderer(), src, nullptr, nullptr);
    SDL_SetRenderTarget(MainWindow::GetRenderer(), nullptr);
    return result;
}
