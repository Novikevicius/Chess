#pragma once

#include "MainWindow.h"
#include "Board.h"
#include "Resources.h"
#include "Piece.h"
#include "EventSystem.h"
#include "ChessEngine.h"

class Game : public Listener
{
    public:
        Game(MainWindow *window);
        ~Game();
    private:
        MainWindow* wnd;
        Resources res;
        Board board;
		ChessEngine* engine;
        Piece* activePiece;
		Vector2 activePiecePos;
		bool isMate = false;
		bool whiteMated = false;
		bool isDraw = false;
		bool hasRequestedMove = false;
		bool hasGameStarted = false;
		bool isPvsP = false;
		bool isPvsAI = true;
		Sprite backgroundImg;
    public:
        void OnStart();
        void RunLogic();
        void DrawFrame();
        void HandleEvent(const Event& e);
		void DisplayText(const std::string & txt, int fontSize, const Vec2D<unsigned>& position, SDL_Color color) const;
};
