#include "KingMovement.h"

bool KingMovement::IsMoveLegal(const Vector2& from, const Vector2& to) const
{
    int deltaX = to.x - from.x;
    int deltaY = to.y - from.y;
    deltaX = deltaX > 0 ? deltaX : -deltaX;
    deltaY = deltaY > 0 ? deltaY : -deltaY;
    return (deltaX <= 1 && deltaY <= 1);
}

char KingMovement::GetPieceType() const
{
	return 'K';
}

PossibleMoves KingMovement::GetAllPossibleMoves(const Vector2& from, const Vector2& to) const
{
	if (!IsMoveLegal(from, to))
		return PossibleMoves();

	PossibleMoves moves;
	const char type = GetPieceType();

	moves.push_back(MoveInfo(from, to, type));

	return moves;
}
