#pragma once

#include "Vector2.h"

class MoveInfo
{
	Vector2 sourcePos;
	Vector2 destinationPos;
	char pieceType;
	int moveNum;
	char capturedPieceType;
	bool hasPieceMoved = false;
	bool hasCapturedPieceMoved = false;
	bool hasCastled = false;
public:
	MoveInfo(const Vector2& from, const Vector2& to, const char& pieceType, const char& capturedType = 0);
public:
	bool operator == (const MoveInfo& rhs) const;
	bool operator != (const MoveInfo& rhs) const;
	bool HasCastled() const
	{
		return hasCastled;
	}
	void SetCastled(const bool& castled = true)
	{
		hasCastled = castled;
	}
	void SetSourcePos(const Vector2& from);
	void SetDestinationPos(const Vector2& to);
	void SetCaptured(const char& type);
	void SetMoved(const bool& hasMoved = true)
	{
		hasPieceMoved = hasMoved;
	}
	int GetMoveNum() const
	{
		return moveNum;
	}
	bool HasMoved() const
	{
		return hasPieceMoved;
	}
	void SetCapturedPieceMoved(const bool& hasMoved = true)
	{
		hasPieceMoved = hasCapturedPieceMoved;
	}
	bool HasCapturedPieceMoved() const
	{
		return hasCapturedPieceMoved;
	}
	Vector2 GetSourcePos() const;
	Vector2 GetDestinationPos() const;
	char GetType() const
	{
		return pieceType;
	}
	char GetCapturedType() const
	{
		return capturedPieceType;
	}
};

