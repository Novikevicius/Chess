#ifndef PAWNMOVEMENT_H
#define PAWNMOVEMENT_H

#include "PieceMovementStrategy.h"


class PawnMovement : public PieceMovementStrategy
{
    public:
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const override;
		char GetPieceType() const override;
		PossibleMoves GetAllPossibleMoves(const Vector2& from);
		PossibleMoves GetAllPossibleMoves(const Vector2& from, const Vector2& to) const override;
};

#endif // PAWNMOVEMENT_H

