#include "Board.h"
#include <iostream>
#include <string>
#include <cassert>
#include "MoveInfo.h"
#include "RookMovement.h"
#include "KnightMovement.h"
#include "BishopMovement.h"
#include "QueenMovement.h"
#include "KingMovement.h"
#include "PawnMovement.h"
#include <sstream>
#include <cctype>
#include "Colors.h"
#include <algorithm>

const std::string Board::defaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Board::Board(Resources* res, const std::string& fen)
	:
	rs(res),
	pieces(width * height),
	lastMove({ -1, -1 }, { -1, -1 }, '-')
{
    std::clog << "Initializing board..." << std::endl;
    xOffset = 0;
    yOffset = 0;
    isWhiteMove = true;
    tiles = new Tile[width * height];
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if((x + y) % 2 == 0)
                tiles[y * width + x].SetColor(true);
            else
                tiles[y * width + x].SetColor(false);
            pieces[y * width + x] = nullptr;
        }
    }
    tileHeight = tiles[0].GetHeight();
    tileWidth  = tiles[0].GetWidth();
    InitializePieces(res, fen);
    border = Sprite(borderColor,
                    tileWidth  * width  + borderSize.x * 2,
                    tileHeight * height + borderSize.y * 2);
	font = TTF_OpenFont("Font3.ttf", fontSize);
    std::clog << "Done initializing board" << std::endl;
}
Board::~Board()
{
    std::clog << "Destroying board..." << std::endl;
    for(unsigned int i = 0; i < width*height; i++)
    {
        if(!pieces.empty() && pieces[i])
            delete pieces[i];
    }
    delete [] tiles;
	TTF_CloseFont(font);
	font = nullptr;
    std::clog << "Done destroying board" << std::endl;
}
Piece* Board::GetPieceAt(int x, int y) const
{
	int i = X_toIndex(x);
	int j = Y_toIndex(y);
    if(i >= 0 && i < width &&
       j >= 0 && j < height)
    {
        return pieces[j * width + i];
    }
    return nullptr;
}
Piece* Board::GetPieceAtIndex(int x, int y) const
{
	if (x >= 0 && x < width &&
		y >= 0 && y < height)
	{
		return pieces[y * width + x];
	}
	return nullptr;
}
Piece* Board::GetPieceAtIndex(const Vector2& index) const
{
	return GetPieceAtIndex(index.x, index.y);
}
void Board::PlacePiece(Piece* p, int x, int y)
{
    if(!p || x < 0 || y < 0)
    {
        return;
    }
    // Converting coordinates (in pixels) to piece indexes
    const int i = X_toIndex(x);
    const int j = Y_toIndex(y);
	if (i >= 0 && i < width &&
		j >= 0 && j < height)
	{
		if (pieces[j * width + i] != nullptr)
		{
			if (!movesHistory.empty())
			{
				movesHistory[movesHistory.size() - 1].SetCaptured(pieces[j*width + i]->GetType()); 
				movesHistory[movesHistory.size() - 1].SetCapturedPieceMoved(pieces[j*width + i]->HasMoved());
			}
			delete pieces[j * width + i];
			pieces[j * width + i] = nullptr;
		}
		if (p->GetXPos() != -1 && p->GetYPos() != -1)
		{
			const int oldX = X_toIndex(p->GetXPos());
			const int oldY = Y_toIndex(p->GetYPos());
			if (oldX >= 0 && oldX < width &&
				oldY >= 0 && oldY < height)
			{
				pieces[oldY * width + oldX] = nullptr;
			}
		}
		pieces[j * width + i] = p;
		p->SetPosition(Index_toX(i), Index_toY(j));
	}
	else
	{
		std::cout << x << " " << y << std::endl;
		throw std::invalid_argument("Cannot place piece at: " + std::to_string(i) + " " + std::to_string(j));
	}
}

/**
 * \brief 
 * \param isKingSideCastle true is player is trying to castle kingside
 * \return returns true if player has castled
 */
bool Board::TryCastle(const bool isKingSideCastle)
{
	const int y = isWhiteMove ? 7 : 0;
	// check if between tiles are not occupied
	if(isKingSideCastle)
	{
		if (GetPieceAtIndex(6, y) ||
			GetPieceAtIndex(5, y))
		{
			return false;
		}
	}
	else
	{
		if (GetPieceAtIndex(1, y) ||
			GetPieceAtIndex(2, y) ||
			GetPieceAtIndex(3, y))
		{
			return false;
		}
	}
	auto king = GetPieceAtIndex(4, y);
	if (!king)
		return false;
	auto rook = GetPieceAtIndex( isKingSideCastle ? 7 : 0, y);
	if(!rook)
		return false;
	// has king or rook moved?
	if(king->HasMoved() || rook->HasMoved())
		return false;
	// castle rook and king
	Vector2 kingDestination, rookDestination;
	// move king to new position
	kingDestination.y = y;
	kingDestination.x = isKingSideCastle ? 6 : 2;
	MovePiece(king, kingDestination);
	// rook king to new position
	rookDestination.y = y;
	rookDestination.x = isKingSideCastle ? 5 : 3;
	MovePiece(rook, rookDestination);
	movesHistory[movesHistory.size() - 1].SetCastled();
	isWhiteMove = !isWhiteMove;
	return true;
}

bool Board::IsCheck(const Piece* srcPiece)
{
	if (!srcPiece)
		return false;
	const auto king = GetKing(!srcPiece->IsWhite());
	const Vector2 kingPos = {X_toIndex(king->GetXPos()), Y_toIndex(king->GetYPos())};
	const Vector2 piecePos = {X_toIndex(srcPiece->GetXPos()), Y_toIndex(srcPiece->GetYPos())};
	if (srcPiece->GetType() == 'P')
	{
		if (isWhiteMove)
		{
			if (kingPos == Vector2{piecePos.x - 1, piecePos.y - 1} ||
				kingPos == Vector2{piecePos.x + 1, piecePos.y - 1})
			{
				return true;
			}
		}
		else
		{
			if (kingPos == Vector2{piecePos.x - 1, piecePos.y + 1} ||
				kingPos == Vector2{piecePos.x + 1, piecePos.y + 1})
			{
				return true;
			}
		}
		return false;
	}
	auto moves = srcPiece->GetMoves(piecePos, kingPos);
	if (moves.empty())
		return false;
	for (int i = 0; i < moves.size() - 1; i++)
	{
		if (GetPieceAtIndex(moves[i].GetDestinationPos()))
		{
			return false;
		}
	}
	if (moves.size() == 1)
	{
		if (!srcPiece->GetType() == 'N')
		{
			Vector2 delta = kingPos - srcPiece->GetPos();
			if (delta.x == -1)
				delta.x = 1;
			if (delta.y == -1)
				delta.y = 1;
			if (delta != Vector2{1, 1} && delta != Vector2{-1, -1})
			{
				return false;
			}
		}
	}
	return true;
}

bool Board::IsStalemate()
{
	for(auto piece : pieces)
	{
		if(piece && piece->IsWhite() == isWhiteMove)
		{
			auto pos = GetPosInIndex(piece->GetPos());
			auto moves = piece->GetMoves(pos);
			for(int i = 0; i < moves.size(); ++i)
			{

				return false;
			}
		}
	}
	return true;
}

bool Board::IsDraw()
{

	return IsThreeMoveRuleDraw();
}

bool Board::IsThreeMoveRuleDraw() const
{
	// white has to repeat 3 moves + black has to repeat 3 moves
	if(movesHistory.size() < 10)
		return false;
	const int last = movesHistory.size()-1;
	const bool hasOneSideRepeated = movesHistory[last] == movesHistory[last - 4] && movesHistory[last] == movesHistory[last - 8];
	const bool hasAnotherSideRepeated = movesHistory[last - 1] == movesHistory[last - 5] && movesHistory[last - 1] == movesHistory[last - 9];
	return hasOneSideRepeated && hasAnotherSideRepeated;
}

void Board::InvokeCheck()
{
	EventSystem* system = EventSystem::GetInstance();
	system->Invoke(Event(Check));
	if (isWhiteMove)
		isWhiteCheck = true;
}

Piece* Board::GetKing(bool isWhite)
{
	auto kingIterator = std::find_if(pieces.begin(), 
									 pieces.end(), 
									[&](Piece* p) -> bool {
										if(!p)
										{
											return false;
										}
										if(p->GetType() != 'K')
										{
											return false;
										}
										return p->IsWhite() == isWhite;
									}
	);
	if(kingIterator == pieces.end())
		return nullptr;
	return *kingIterator;
}

bool Board::IsCheck(const bool& isCheckForWhite)
{
	for (auto piece : pieces)
	{
		if (!piece || piece->IsWhite() == isCheckForWhite)
			continue;
		if (IsCheck(piece))
		{
			return true;
		}
	}
	return false;
}

void Board::StartGame()
{
	for (unsigned i = 0; i < pieces.size(); i++)
	{
		delete pieces[i];
		pieces[i] = nullptr;
	}
	movesHistory.clear();
	InitializePieces(rs, defaultFEN);
}

void Board::Draw(const Vector2& toSkip)
{
    border.Draw(xOffset, yOffset);
    Sprite moveColor;
    Sprite moveColorBackground(0x807B70FF, tileWidth, tileHeight);
    if(isWhiteMove)
    {
        moveColor = Sprite(0xFFFFFFFF, tileWidth - 20, tileHeight - 20);
    }
    else
    {
        moveColor = Sprite(0x000000FF, tileWidth - 20, tileHeight - 20);
    }
	moveColorBackground.Draw({ static_cast<unsigned int>(borderSize.x * 2 + width * tileWidth + xOffset),
		static_cast<unsigned int>(yOffset) });
	moveColor.Draw({ static_cast<unsigned int>(borderSize.x * 2 + width * tileWidth + 10 + xOffset),
		static_cast<unsigned int>(borderSize.y / 2 + yOffset) });
    // Drawing background Tiles
	Vector2 oldPos(-1, -1);
	Vector2 newPos(-1, -1);
	if (!movesHistory.empty())
	{
		MoveInfo lastMove = movesHistory[movesHistory.size() - 1];
		oldPos = lastMove.GetSourcePos();
		newPos = lastMove.GetDestinationPos();
	}
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
			if(x == oldPos.x && y == oldPos.y)
			{
				tiles[y * width + x].Draw(GetIndexInCoord(oldPos), COLOR_GREY);
			}
        	else if(x == newPos.x && y == newPos.y)
			{
				tiles[y * width + x].Draw(GetIndexInCoord(newPos), COLOR_GREY - 0x55555500);
			}
			else
				tiles[y * width + x].Draw(Index_toX(x),
										  Index_toY(y));
        }
    }
	for (auto piece : pieces)
	{
		if(!piece)
			continue;
		if(piece->GetXPos() == toSkip.x && piece->GetYPos() == toSkip.y)
			continue;
		piece->Draw();
	};
	DisplayMadeMoves(20);
}
void Board::DisplayMadeMoves(const unsigned int& maxMoveCount) const
{
	if (movesHistory.empty())
		return;
	unsigned int i = 0;
	if (maxMoveCount < movesHistory.size())
		i = movesHistory.size() - maxMoveCount;
	for (; i < movesHistory.size(); i++)
	{
		std::stringstream msg;
		msg << i + 1 << " " << MoveToChessNotation(movesHistory[i]);
		const auto moveInfo = TTF_RenderText_Solid(font, msg.str().c_str(), fontColor);
		Sprite img(*moveInfo);
		Vec2D<unsigned> pos(440, 50);
		if (maxMoveCount < movesHistory.size())
			pos += Vec2D<unsigned>(0, (i - (movesHistory.size() - maxMoveCount)) * fontSize);
		else
			pos += Vec2D<unsigned>(0, i * fontSize);
		img.Draw(pos);
		SDL_FreeSurface(moveInfo);
	}	
}
std::string Board::GetFEN() const
{
	std::string fen;
	for(int i = 0; i < height; i++)
	{
		int emptyTiles = 0;
		for(int j = 0; j < width; j++)
		{
			auto piece = GetPieceAtIndex(j, i);
			if (!piece)
				emptyTiles++;
			else
			{
				if(emptyTiles)
				{
					fen += emptyTiles + '0';
					emptyTiles = 0;
				}
				auto type = piece->GetType();
				if (piece->IsWhite())
					type = std::toupper(type);
				else
					type = std::tolower(type);
				fen += type;
			}
		}
		if (emptyTiles)
		{
			fen += emptyTiles + '0';
		}
		fen += '/';
	}

	fen += isWhiteMove ? " w " : " b ";
	bool canWhiteCastle = canWhiteCastleLong || canWhiteCastleShort;
	bool canBlackCastle = canBlackCastleLong || canBlackCastleShort;
	if (!canBlackCastle && !canWhiteCastle)
		fen += "- - ";
	else
	{
		if (!canWhiteCastle && canBlackCastle)
		{
			if (canBlackCastleShort)
				fen += "k";
			if (canBlackCastleLong)
				fen += "q";
			fen += " - ";
		}
		else if(canWhiteCastle && !canBlackCastle)
		{
			if (canWhiteCastleShort)
				fen += "K";
			if (canWhiteCastleLong)
				fen += "Q";
			fen += " - ";
		}
		else
		{
			if (canWhiteCastleShort)
				fen += "K";
			if (canWhiteCastleLong)
				fen += "Q";
			if (canBlackCastleShort)
				fen += "k";
			if (canBlackCastleLong)
				fen += "q";
			fen += ' ';
		}
	}
	// En passant position
	if(movesHistory.empty())
	{
		fen += " - ";
	}
	else if(movesHistory[movesHistory.size() - 1].GetType() == 'P')
	{
		auto deltaY = movesHistory[movesHistory.size()-1].GetDestinationPos().y - movesHistory[movesHistory.size() - 1].GetSourcePos().y;
		if(deltaY == -2 || deltaY == 2)
		{
			fen += 'a' + movesHistory[movesHistory.size() - 1].GetDestinationPos().x;
			fen += '0' + 8 - movesHistory[movesHistory.size() - 1].GetDestinationPos().y + (deltaY < 0 ? -1 : 1);
		}
	}
	// non - capture moves count
	fen += " 0 ";
	fen += std::to_string(movesHistory.size());
	fen += '\n';
	return fen;
}
bool Board::Move(const Vector2 from, const Vector2 to)
{
	// Convert mouse position to piece index
	const Vector2 srcPos = { X_toIndex(from.x), Y_toIndex(from.y) };
	const Vector2 destPos = { X_toIndex(to.x), Y_toIndex(to.y) };

	Piece* srcPiece = GetPieceAtIndex(srcPos.x, srcPos.y);
	if(!srcPiece)
		return false;

	if (srcPiece->IsWhite() != isWhiteMove)
		return false;

	// is pawn capturing or promoting
	if (srcPiece->GetType() == 'P')
	{
		// is trying to capture on en passant?
		if (!movesHistory.empty())
		{
			const MoveInfo lastMove = movesHistory.back();
			if (lastMove.GetType() == 'P')
			{
				Vector2 enPassantSquare(lastMove.GetDestinationPos());
				if (isWhiteMove)
					--enPassantSquare.y;
				else
					++enPassantSquare.y;
				if (destPos == enPassantSquare)
				{
					RemovePieceAt(lastMove.GetDestinationPos());
					isWhiteMove = !isWhiteMove;
					MovePiece(srcPiece, destPos);
					// Increament moves count
					if (!isWhiteMove)
						movesCount++;
					if (IsCheck(GetPieceAtIndex(destPos)))
					{
						InvokeCheck();
					}
					return true;
				}
			}
		}
		// is pawn trying to capture
		if(GetPieceAtIndex(destPos.x, destPos.y) != nullptr)
		{
			const Piece* destPiece = GetPieceAtIndex(destPos.x, destPos.y);
			// are pieces same color?
			if (destPiece->IsWhite() == srcPiece->IsWhite())
				return false;
			// maybe trying to capture king?
			if (destPiece->GetType() == 'K')
				return false;
			// is capture possible (pawn can move only diagonally forward)
			if ( !((srcPos.x - destPos.x == 1  || srcPos.x - destPos.x == -1) &&
				 ( (srcPiece->IsWhite() && srcPos.y - destPos.y == 1) || (!srcPiece->IsWhite() && srcPos.y - destPos.y == -1))) )
				return false;
			// move pawn to destination
			// is pawn on last rank now?
			if(srcPiece->IsWhite() && destPos.y == 0)
			{
				MovePiece(new Piece(rs->GetSprite("WhiteQueen"), srcPiece->IsWhite(), new QueenMovement()), destPos);
			}
			else if (!srcPiece->IsWhite() && destPos.y == 7)
			{
				MovePiece(new Piece(rs->GetSprite("BlackQueen"), srcPiece->IsWhite(), new QueenMovement()), destPos);
			}
			isWhiteMove = !isWhiteMove;
			MovePiece(srcPiece, destPos);
			// Increament moves count
			if (!isWhiteMove)
				movesCount++;
			if (IsCheck(GetPieceAtIndex(destPos)))
			{
				InvokeCheck();
			}
			return true;
		}
	}
	// check if player is trying to castle
	if(srcPiece->GetType() == 'K')
	{
		if (destPos.y == srcPos.y)
		{
			// Kingside castle
			if (destPos.x == srcPos.x + 2)
			{
				if (TryCastle(true))
				{
					if (!isWhiteMove)
						movesCount++;
					if(IsCheck(GetPieceAtIndex({ 5, destPos.y})))
					{
						InvokeCheck();
					}
					return true;
				}
			}
			// Queenside castle
			if(destPos.x == srcPos.x - 2)
			{
				if (TryCastle(false))
				{
					if (!isWhiteMove)
						movesCount++;
					if (IsCheck(GetPieceAtIndex({ 3, destPos.y })))
					{
						InvokeCheck();
					}
					return true;
				}
			}
		}
	}
	auto moves = srcPiece->GetMoves(srcPos, destPos);
	if (moves.empty())
		return false;
	for (auto& move : moves)
	{
		const auto piece = GetPieceAtIndex(move.GetDestinationPos());
		if (piece && move.GetDestinationPos() != destPos)
		{
			// Capture piece if possible
			return false;
		}
	}
	const Piece* destPiece = GetPieceAt(to.x, to.y);
	if (destPiece)
	{
		if (destPiece->IsWhite() == srcPiece->IsWhite())
			return false;
		if (destPiece->GetType() == 'K')
			return false;
	}
	MovePiece(srcPiece, destPos);
	if (srcPiece->GetType() == 'P' && srcPiece->IsWhite() && destPos.y == 0)
	{
		PlacePiece(new Piece(rs->GetSprite("WhiteQueen"), srcPiece->IsWhite(), new QueenMovement()), to.x, to.y);
	}
	else if (srcPiece->GetType() == 'P' && !srcPiece->IsWhite() && destPos.y == 7)
	{
		PlacePiece(new Piece(rs->GetSprite("BlackQueen"), srcPiece->IsWhite(), new QueenMovement()), to.x, to.y);
	}
	if (IsCheck(GetPieceAtIndex(destPos)))
	{
		InvokeCheck();
	}
	isWhiteMove = !isWhiteMove;
	pieces[srcPos.y * width + srcPos.x] = nullptr;
	if (!isWhiteMove)
		movesCount++;
	return true;
}
void Board::MovePiece(Piece * p, const Vector2& to)
{
	Vector2 oldPos = { X_toIndex(p->GetXPos()), Y_toIndex(p->GetYPos()) };
	auto lastMove = MoveInfo(oldPos, to, p->GetType());
	lastMove.SetMoved(p->HasMoved());
	movesHistory.push_back(lastMove);
	if(movesHistory.size() == 42)
	{
		std::cout << "42 move made\n";
	}
	p->SetMoved(true);

	if(p->GetType() == 'R')
	{
		if (!isWhiteMove)
		{
			if (oldPos == Vector2(0, 0))
			{
				canBlackCastleLong = false;
			}
			else if (oldPos == Vector2(7, 0))
			{
				canBlackCastleShort = false;
			}
		}
		else
		{
			if (oldPos == Vector2(0, 7))
			{
				canWhiteCastleLong = false;
			}
			else if (oldPos == Vector2(7, 7))
			{
				canWhiteCastleShort = false;
			}
		}
	}
	else if(p->GetType() == 'K')
	{
		if(p->IsWhite())
		{
			canWhiteCastleLong = false;
			canWhiteCastleShort = false;
		}
		else
		{
			canBlackCastleLong = false;
			canBlackCastleShort = false;
		}
	}
	PlacePiece(p, Index_toX(to.x), Index_toY(to.y));
}

void Board::RemovePieceAt(Vector2 pos)
{
    if(pieces[pos.y * width + pos.x])
    {
        delete pieces[pos.y * width + pos.x];
        pieces[pos.y * width + pos.x] = nullptr;
    }
}

bool Board::HasPieceAt(const Vector2& pos)
{
    int i = X_toIndex(pos.x);
    int j = Y_toIndex(pos.y);
    return pieces[i * width + j] != nullptr;
}

bool Board::IsWhiteMove() const
{
	return isWhiteMove;
}

void Board::RevertLastMove()
{
	if (movesHistory.empty())
		return;
	auto last = movesHistory[movesHistory.size() - 1];
 	auto oldPos = last.GetSourcePos();
	auto newPos = last.GetDestinationPos();
	auto movedPiece = GetPieceAtIndex(newPos);
	pieces[oldPos.y * width + oldPos.x] = movedPiece;
	pieces[newPos.y * width + newPos.x] = nullptr;
	auto oldPosCoordinates = GetIndexInCoord(oldPos);
	if (movedPiece) 
	{
		movedPiece->SetPosition(oldPosCoordinates.x, oldPosCoordinates.y);
		movedPiece->SetMoved(last.HasMoved());
	}
	// place king in correct place in case of castling
	if(last.HasCastled())
	{
		Piece* king = nullptr;
		auto defautKingPos = Vector2(4, isWhiteMove ? 0 : 7);
		Vector2 kingPos = { 2, isWhiteMove ? 0 : 7 };
		king = GetPieceAtIndex(kingPos);
		if(!king || king->GetType() != 'K')
		{
			kingPos.x = 6;
			king = GetPieceAtIndex(kingPos);
		}
		pieces[defautKingPos.y * width + defautKingPos.x] = king;
		pieces[kingPos.y * width + kingPos.x] = nullptr;
		king->SetPosition(Index_toX(defautKingPos.x), Index_toY(defautKingPos.y));
		king->SetMoved(false);
	}
	else
	{		
		Piece* capturedPiece = nullptr;
		bool wasCapturedWhite = isWhiteMove;
		std::string color = wasCapturedWhite ? "White" : "Black";

		switch (last.GetCapturedType())
		{
		case 'Q':
			capturedPiece = new Piece(rs->GetSprite( color + "Queen"), wasCapturedWhite, new QueenMovement());
			break;
		case 'B':
			capturedPiece = new Piece(rs->GetSprite(color + "Bishop"), wasCapturedWhite, new BishopMovement());
			break;
		case 'N':
			capturedPiece = new Piece(rs->GetSprite(color + "Knight"), wasCapturedWhite, new KnightMovement());
			break;
		case 'R':
			capturedPiece = new Piece(rs->GetSprite(color + "Rook"), wasCapturedWhite, new RookMovement());
			break;
		case 'P':
			capturedPiece = new Piece(rs->GetSprite(color + "Pawn"), wasCapturedWhite, new PawnMovement());
			break;
		}

		pieces[newPos.y * width + newPos.x] = capturedPiece;
		if(capturedPiece)
		{
			auto newPosCoordinates = GetIndexInCoord(newPos);
			capturedPiece->SetPosition(newPosCoordinates.x, newPosCoordinates.y);
			capturedPiece->SetMoved(last.HasCapturedPieceMoved());
		}
	}
	isWhiteMove = !isWhiteMove;
	movesHistory.pop_back();
	
}

void Board::InitializePieces(Resources* rs, const std::string& fen)
{
	Vector2 pos = { 0, 0 };
	Piece* whiteKing = nullptr;
	Piece* blackKing = nullptr;
	int i = 0;
	for(; i < fen.size(); i++)
	{
		if (fen[i] == ' ')
		{
			i++;
			break;
		}
		if(fen[i] > '0' && fen[i] <= '9')
		{
			pos.x -= fen[i] - '0' - 1;
			continue;
		}
		Piece* piece = nullptr;
		switch (fen[i])
		{
			case '/':
				pos.y++;
				pos.x = 0;
				break;
			case 'r':
				piece = new Piece(rs->GetSprite("BlackRook"), false, new RookMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'R':
				piece = new Piece(rs->GetSprite("WhiteRook"), true, new RookMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'n':
				piece = new Piece(rs->GetSprite("BlackKnight"), false, new KnightMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'N':
				piece = new Piece(rs->GetSprite("WhiteKnight"), true, new KnightMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'b':
				piece = new Piece(rs->GetSprite("BlackBishop"), false, new BishopMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'B':
				piece = new Piece(rs->GetSprite("WhiteBishop"), true, new BishopMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'q':
				piece = new Piece(rs->GetSprite("BlackQueen"), false, new QueenMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'Q':
				piece = new Piece(rs->GetSprite("WhiteQueen"), true, new QueenMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'k':
				blackKing = new Piece(rs->GetSprite("BlackKing"), false, new KingMovement());
				piece = blackKing;
				PlacePiece(blackKing, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'K':
				whiteKing = new Piece(rs->GetSprite("WhiteKing"), true, new KingMovement());
				piece = whiteKing;
				PlacePiece(whiteKing, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'p':
				piece = new Piece(rs->GetSprite("BlackPawn"), false, new PawnMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			case 'P':
				piece = new Piece(rs->GetSprite("WhitePawn"), true, new PawnMovement());
				PlacePiece(piece, Index_toX(pos.x), Index_toY(pos.y));
				pos.x++;
				break;
			default:
				throw std::invalid_argument(std::string("Unrecognized symbol in fen: ") + fen[i]);
		}
	}
	// get which side has to move
	isWhiteMove = (fen[i] == 'w');
	i += 2;

	// check if can castle
	bool doneCheckingCastling = false;
	bool bks = false; // black king side
	bool bqs = false; // black queen side
	bool wks = false; // white king side
	bool wqs = false; // white queen side
	int j = i;
	while(!doneCheckingCastling && j < fen.size())
	{
		switch (fen[j])
		{
		case '-':
			if (fen[j + 2] >= '0' && fen[j + 2] <= '9')
				doneCheckingCastling = true;
			break;
		case 'K':
			wks = true;
			break;
		case 'k':
			bks = true;
			break;
		case 'Q':
			wqs = true;
			break;
		case 'q':
			bqs = true;
			break;
		}
		j++;
	}

	// TODO: set castling ability
}
