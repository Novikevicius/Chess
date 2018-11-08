#include "RookMovement.h"

bool RookMovement::IsMoveLegal(const Vector2& from, const Vector2& to) const
{
    int deltaX = to.x - from.x;
    int deltaY = to.y - from.y;
    deltaX = deltaX > 0 ? deltaX : -deltaX;
    deltaY = deltaY > 0 ? deltaY : -deltaY;
    return ((deltaX == 0 && deltaY > 0) ||
            (deltaX > 0 && deltaY == 0));
}

char RookMovement::GetPieceType() const
{
	return 'R';
}

PossibleMoves RookMovement::GetAllPossibleMoves(const Vector2 & from)
{
	PossibleMoves moves;
	const char type = GetPieceType();

	Vector2 pos(from);
	pos.x--;
	while (pos.x >= 0)
	{
		moves.push_back(MoveInfo(from, pos, GetPieceType()));
		pos.x--;
	}
	pos = from;
	pos.x++;
	while (pos.x < 8)
	{
		moves.push_back(MoveInfo(from, pos, GetPieceType()));
		pos.x++;
	}
	pos = from;
	pos.y--;
	while (pos.y >= 0)
	{
		moves.push_back(MoveInfo(from, pos, GetPieceType()));
		pos.x--;
	}
	pos = from;
	pos.y++;
	while (pos.y < 8)
	{
		moves.push_back(MoveInfo(from, pos, GetPieceType()));
		pos.y++;
	}
	return moves;
}
PossibleMoves RookMovement::GetAllPossibleMoves(const Vector2 & from, const Vector2 & to) const
{
	if (!IsMoveLegal(from, to))
		return PossibleMoves();

	PossibleMoves moves;
	const char type = GetPieceType();

	Vector2 direction;
	if (from.x < to.x)
		direction.x = 1;
	else if (from.x > to.x)
		direction.x = -1;
	else
		direction.x = 0;

	if (from.y < to.y)
		direction.y = 1;
	else if (from.y > to.y)
		direction.y = -1;
	else
		direction.y = 0;

	Vector2 dest = from + direction;
	while (dest.x >= 0 && dest.x < 8 && dest.y >= 0 && dest.y < 8)
	{
		moves.push_back(MoveInfo(from, dest, type));
		if (dest == to)
		{
			break;
		}
		dest += direction;
	}
	return moves;
}
