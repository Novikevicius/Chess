#pragma once

#include <SDL.h>
#include <vector>
#include "Tile.h"
#include "Piece.h"
#include "Resources.h"
#include "Vector2.h"
#include "EventSystem.h"
#include <SDL_ttf.h>
#include <string>

class Board
{
    public:
        Board(Resources* rs, const std::string& fen = defaultFEN);
        ~Board();
        // Deleting copy ctor and assignment operator
        Board(const Board & other) = delete;
        Board& operator=(const Board& other) = delete;
    private:
        Resources* rs;
        Tile* tiles;
        int tileWidth, tileHeight;
        std::vector<Piece*> pieces;
		std::vector<MoveInfo> movesHistory;
        static const Uint32 borderColor = 0x807B77FF;
        Sprite border;
        bool isWhiteMove;
		int xOffset;
		int yOffset;
		static const std::string defaultFEN;
		MoveInfo lastMove;
		bool canBlackCastleLong = true;
		bool canBlackCastleShort = true;
		bool canWhiteCastleLong = true;
		bool canWhiteCastleShort = true;		
		int movesCount = 0;
		bool isWhiteCheck = false;
		TTF_Font* font; 
		SDL_Color fontColor {180, 180, 200, 255};
		const int fontSize = 14;
    public:
        static const int width = 8;
        static const int height = 8;
		const Vector2 borderSize = {20, 20};
    public:
		void StartGame();
        void Draw(const Vector2& toSkip = {0, 0});
		bool IsStalemate();
		void DisplayMadeMoves(const unsigned int& maxMoveCount) const;
		std::string GetFEN() const;
        Piece* GetPieceAt(int x, int y) const;
		Piece* GetPieceAtIndex(int x, int y) const;
		Piece* GetPieceAtIndex(const Vector2& index) const;
		bool Move(Vector2 from, Vector2 to);
		bool HasPieceAt(const Vector2& pos);
		bool IsWhiteMove() const;
		bool IsCheck(const bool& isCheckForWhite);
		void RevertLastMove();
    private:
		void MovePiece(Piece* p, const Vector2& to);
        void InitializePieces(Resources* rs, const std::string& fen);
        void RemovePieceAt(Vector2 pos);
		void PlacePiece(Piece* p, int x, int y);
		bool TryCastle(const bool isKingSideCastle);
		bool IsCheck(const Piece* srcPiece);
		bool IsThreeMoveRuleDraw() const;
		//bool IsMate() const; // use stockfish to check for (none) move (draw or mate)
		void InvokeCheck();
		Piece* GetKing(bool isWhite);
	public:
		bool IsDraw();
		std::string MoveToChessNotation(const MoveInfo& move) const
		{
			std::string result;

			result += 'a' + move.GetSourcePos().x;
			result += '0' + width - move.GetSourcePos().y;
			result += 'a' + move.GetDestinationPos().x;
			result += '0' + width - move.GetDestinationPos().y;

			return result;
		}
		Vector2 GetPosInIndex(const int& x, const int& y) const
		{
			return { X_toIndex(x),  Y_toIndex(y) };
		}
		Vector2 GetPosInIndex(const Vector2& v) const
		{
			return { X_toIndex(v.x),  Y_toIndex(v.y) };
		}
		Vector2 GetIndexInCoord(const Vector2& index) const
		{
			return { Index_toX(index.x),  Index_toY(index.y) };
		}
        int X_toIndex(int x) const
        {
            return (x - borderSize.x - xOffset) / tileWidth;
        }
        int Y_toIndex(int y) const
        {
            return (y - borderSize.y - yOffset) / tileHeight;
        }
        int Index_toX(int i) const
        {
            return i * tileWidth + borderSize.x + xOffset;
        }
        int Index_toY(int i) const
        {
            return i * tileWidth + borderSize.y + yOffset;
        }
};
