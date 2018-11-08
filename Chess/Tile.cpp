#include "Tile.h"
#include "MainWindow.h"

Tile::Tile()
        :
    tileSprite(0xF0F0F0FF, 50, 50)
{

}
void Tile::Draw(const Vector2& pos, Uint32 color) const
{
	if(color != 0)
	{
		Sprite newSprite(color, 50, 50);
		newSprite.Draw(static_cast<unsigned>(pos.x), static_cast<unsigned>(pos.y));
	}
	else
		tileSprite.Draw(pos.x, pos.y);
}
void Tile::Draw(int x, int y) const
{
    tileSprite.Draw(x, y);
}
void Tile::SetColor(bool isWhite)
{
    if(isWhite)
        tileSprite = Sprite(whiteTileColor, 50, 50);
    else
        tileSprite = Sprite(blackTileColor, 50, 50);
}
int Tile::GetWidth() const
{
    return tileSprite.GetWidth();
}
int Tile::GetHeight() const
{
    return tileSprite.GetHeight();
}
