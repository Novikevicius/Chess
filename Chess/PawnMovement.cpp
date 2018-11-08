#include "PawnMovement.h"

bool PawnMovement::IsMoveLegal(const Vector2& from, const Vector2& to) const
{
    int deltaX = to.x - from.x;
    int deltaY = to.y - from.y;
    deltaX = deltaX >= 0 ? deltaX : deltaX * -1;
    deltaY = deltaY >= 0 ? deltaY : deltaY * -1;
	if (hasMoved)
		return (deltaX <= 0 && deltaY == 1);
	return (deltaX == 0 && deltaY <= 2);
}

char PawnMovement::GetPieceType() const
{
	return 'P';
}

PossibleMoves PawnMovement::GetAllPossibleMoves(const Vector2& from)
{
	PossibleMoves moves;
	const char type = GetPieceType();

	if (isWhite)
	{
		Vector2 to(from.x, from.y - 1);
		moves.push_back(MoveInfo(from, to, type));
		to = Vector2(from.x, from.y - 2);
		moves.push_back(MoveInfo(from, to, type));
		to = Vector2(from.x + 1, from.y - 1);
		moves.push_back(MoveInfo(from, to, type));
		to = Vector2(from.x - 1, from.y - 1);
		moves.push_back(MoveInfo(from, to, type));
	}
	else
	{
		Vector2 to(from.x, from.y + 1);
		moves.push_back(MoveInfo(from, to, type));
		to = Vector2(from.x, from.y + 2);
		moves.push_back(MoveInfo(from, to, type));
		to = Vector2(from.x + 1, from.y + 1);
		moves.push_back(MoveInfo(from, to, type));
		to = Vector2(from.x - 1, from.y + 1);
		moves.push_back(MoveInfo(from, to, type));
	}
	return moves;
}

PossibleMoves PawnMovement::GetAllPossibleMoves(const Vector2& from, const Vector2& to) const
{
	if (!IsMoveLegal(from, to))
		return PossibleMoves();

	PossibleMoves moves;
	const char type = GetPieceType();

	const Vector2 direction = { from.x, isWhite ? -1 : 1 };
	moves.push_back(MoveInfo(from, Vector2(from.x, from.y + direction.y), type));
	if(!hasMoved && to == Vector2(from.x, from.y + direction.y * 2))
		moves.push_back(MoveInfo(from, Vector2(from.x, from.y + direction.y * 2), type));

	return moves;
}

