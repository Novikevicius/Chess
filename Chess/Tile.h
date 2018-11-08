#pragma once

#include "Sprite.h"

class Tile
{
    public:
        Tile();
private:
        Sprite tileSprite;
        static const Uint32 whiteTileColor = 0xFADAC5FF;
        static const Uint32 blackTileColor = 0xB97C54FF;
    public:
		void Draw(const Vector2& pos, Uint32 color = 0) const;
        void Draw(int x, int y) const;
        void SetColor(bool isWhite);
        int GetWidth() const ;
        int GetHeight() const;
};
