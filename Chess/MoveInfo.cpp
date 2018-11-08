#include "MoveInfo.h"

MoveInfo::MoveInfo(const Vector2& from, const Vector2& to, const char& pieceType, const char& capturedType)
{
	this->sourcePos = from;
	this->destinationPos = to;
	this->pieceType = pieceType;
	this->capturedPieceType = capturedType;
}

bool MoveInfo::operator==(const MoveInfo& rhs) const
{
	if( sourcePos != rhs.sourcePos)
		return false;
	if ( destinationPos != rhs.destinationPos)
		return false;
	if( pieceType != rhs.pieceType)
		return false;
	if( capturedPieceType != rhs.capturedPieceType)
		return false;
	/*if( hasPieceMoved != rhs.hasCapturedPieceMoved)
		return false;*/
	if( hasCapturedPieceMoved != rhs.hasCapturedPieceMoved)
		return false;
	if( hasCastled != rhs.hasCastled)
		return false;
	return true;
}

bool MoveInfo::operator!=(const MoveInfo& rhs) const
{
	return !(*this == rhs);
}

void MoveInfo::SetSourcePos(const Vector2& from)
{
	sourcePos = from;
}
void MoveInfo::SetDestinationPos(const Vector2& to)
{
	destinationPos = to;
}

void MoveInfo::SetCaptured(const char& type)
{
	capturedPieceType = type;
}

Vector2 MoveInfo::GetSourcePos() const
{
	return sourcePos;
}
Vector2 MoveInfo::GetDestinationPos() const
{
	return destinationPos;
}
