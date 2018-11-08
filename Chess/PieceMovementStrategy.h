#ifndef PIECEMOVEMENTSTRATEGY_H
#define PIECEMOVEMENTSTRATEGY_H

#pragma once
#include "Vector2.h"
#include "MoveInfo.h"
#include <vector>

using PossibleMoves = std::vector<MoveInfo>;

class PieceMovementStrategy
{
	protected:
		bool hasMoved = false;
		bool isWhite = false;
    public:
		void SetMoved(bool v) { hasMoved = v; }
		void SetColor(bool _isWhite) { isWhite = _isWhite; }
		bool HasMoved() const;
        virtual ~PieceMovementStrategy() {}
        virtual bool IsMoveLegal(const Vector2& from, const Vector2& to) const = 0; 
		virtual char GetPieceType() const = 0;
		PossibleMoves GetAllPossibleMoves(const Vector2& from)
		{
			std::vector<MoveInfo> moves;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
					if (Vector2(i, j) != from && IsMoveLegal(from, { i, j }))
						moves.push_back(MoveInfo(from, { i, j }, GetPieceType()));

			}
			return moves;
		}
		virtual PossibleMoves GetAllPossibleMoves(const Vector2& from, const Vector2& to) const = 0;
};

#endif // PIECEMOVEMENTSTRATEGY_H
