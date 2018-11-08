#ifndef BISHOPMOVEMENT_H
#define BISHOPMOVEMENT_H

#include "PieceMovementStrategy.h"

class BishopMovement : public PieceMovementStrategy
{
    public:
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const;
		char GetPieceType() const override;
		PossibleMoves GetAllPossibleMoves(const Vector2& from);
		PossibleMoves GetAllPossibleMoves(const Vector2 & from, const Vector2 & to) const override;
};

#endif // BISHOPMOVEMENT_H
