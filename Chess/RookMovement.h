#ifndef ROOKMOVEMENT_H
#define ROOKMOVEMENT_H

#include "PieceMovementStrategy.h"


class RookMovement : public PieceMovementStrategy
{
    public:
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const override;
		char GetPieceType() const override;
		PossibleMoves GetAllPossibleMoves(const Vector2& from);
		PossibleMoves GetAllPossibleMoves(const Vector2& from, const Vector2& to) const override;
};

#endif // ROOKMOVEMENT_H
