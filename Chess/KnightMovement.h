#ifndef KNIGHTMOVEMENT_H
#define KNIGHTMOVEMENT_H

#include "PieceMovementStrategy.h"


class KnightMovement : public PieceMovementStrategy
{
    public:
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const override;
		char GetPieceType() const override;
		PossibleMoves GetAllPossibleMoves(const Vector2& from, const Vector2& to) const override;
};

#endif // KNIGHTMOVEMENT_H
