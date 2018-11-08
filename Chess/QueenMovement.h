#ifndef QUEENMOVEMENT_H
#define QUEENMOVEMENT_H

#include "PieceMovementStrategy.h"


class QueenMovement : public PieceMovementStrategy
{
    public:
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const override;
		char GetPieceType() const override;
		PossibleMoves GetAllPossibleMoves(const Vector2& from, const Vector2& to) const override;
};

#endif // QUEENMOVEMENT_H
