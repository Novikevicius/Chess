#include "BishopMovement.h"

bool BishopMovement::IsMoveLegal(const Vector2& from, const Vector2& to) const
{
    int deltaX = to.x - from.x;
    int deltaY = to.y - from.y;
    deltaX = deltaX < 0 ? deltaX : -deltaX;
    deltaY = deltaY < 0 ? deltaY : -deltaY;
    return (deltaX == deltaY);
}

char BishopMovement::GetPieceType() const
{
	return 'B';
}

PossibleMoves BishopMovement::GetAllPossibleMoves(const Vector2& from)
{
	PossibleMoves moves;
	const char type = GetPieceType();
	int i = 1;
	while (from.x + i < 8 && from.y + i < 8)
	{
		moves.push_back(MoveInfo(from, Vector2(from.x + i, from.y + i), type));
		i++;
	}
	i = 1;
	while (from.x - i >= 0 && from.y - i >= 0)
	{
		moves.push_back(MoveInfo(from, Vector2(from.x - i, from.y - i), type));
		i++;
	}
	i = 1;
	while (from.x + i < 8 && from.y - i >= 0)
	{
		moves.push_back(MoveInfo(from, Vector2(from.x + i, from.y - i), type));
		i++;
	}
	i = 1;
	while (from.x - i >= 0 && from.y + i < 8)
	{
		moves.push_back(MoveInfo(from, Vector2(from.x - i, from.y + i), type));
		i++;
	}
	return moves;
}

PossibleMoves BishopMovement::GetAllPossibleMoves(const Vector2& from, const Vector2& to) const
{
	if (!IsMoveLegal(from, to))
		return PossibleMoves();

	PossibleMoves moves;
	const char type = GetPieceType();

	const Vector2 direction = { from.x < to.x ? 1 : -1,
						  from.y < to.y ? 1 : -1 };

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
