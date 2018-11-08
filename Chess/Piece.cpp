#include "Piece.h"
#include "Sprite.h"

Piece::Piece(const Sprite& sprite, const bool _isWhite, PieceMovementStrategy* movement)
	:
isWhite(_isWhite)
{
    if(!movement)
        throw "Movement strategy cannot be null";
    this->movement = movement;
	movement->SetColor(isWhite);
    image = sprite;
    x = -1;
    y = -1;
}
Piece::~Piece()
{
    delete movement;
}
bool Piece::IsMoveLegal(const Vector2& from, const Vector2& to) const
{
    return movement->IsMoveLegal(from, to);
}

void Piece::SetMoved(const bool v)
{
	movement->SetMoved(v);
}

bool Piece::HasMoved() const
{
	return movement->HasMoved();
}

char Piece::GetType() const
{
	return movement->GetPieceType();
}
void Piece::Draw(const int x, const int y) const
{
    image.Draw(x, y);
}
void Piece::Draw(const Vec2D<float>& size) const
{
    image.Draw(Vec2D<unsigned int>(this->x, this->y), size);
}
Sprite Piece::GetSprite() const
{
    return image;
}
void Piece::SetPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Piece::IsWhite() const
{
	return isWhite;
}

int Piece::GetXPos() const
{
    return x;
}
int Piece::GetYPos() const
{
    return y;
}

Vector2 Piece::GetPos() const
{
	return {x, y};
}
