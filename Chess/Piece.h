#ifndef PIECE_H
#define PIECE_H

#pragma once

#include "Sprite.h"
#include "Vector2.h"
#include "PieceMovementStrategy.h"

class Piece
{
    public:
		Piece(const Sprite& sprite, const bool _isWhite, PieceMovementStrategy* movement);
		~Piece();
        Piece(const Piece & piece) = delete;
		Piece& operator = (const Piece& rhs) = delete;
        void Draw(const int x, const int y) const;
        void Draw(const Vec2D<float>& size = {1, 1}) const;
        Sprite GetSprite() const;
        void SetPosition(int x, int y);
		bool IsWhite() const;
        int GetXPos() const;
        int GetYPos() const;
	Vector2 GetPos() const;
        bool IsMoveLegal(const Vector2& from, const Vector2& to) const;
		void SetMoved(const bool v);
		bool HasMoved() const;
		char GetType() const;
		PossibleMoves GetMoves(const Vector2& from) const { return movement->GetAllPossibleMoves(from); }
		PossibleMoves GetMoves(const Vector2& from, const Vector2& to) const { return movement->GetAllPossibleMoves(from, to); }
    private:
        const bool isWhite;
        int x;
        int y;
        Sprite image;
        PieceMovementStrategy* movement;
};

#endif // PIECE_H
