#ifndef KINGMOVEMENT_H
#define KINGMOVEMENT_H

#include "PieceMovementStrategy.h"


class KingMovement : public PieceMovementStrategy
{
    public:
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const override;
		char GetPieceType() const override;
	    PossibleMoves GetAllPossibleMoves(const Vector2& from, const Vector2& to) const override;
};

#endif // KINGMOVEMENT_H
