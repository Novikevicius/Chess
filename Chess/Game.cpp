#include <iostream>
#include "Game.h"
#include "Events.h"

Game::Game(MainWindow *window) : wnd(window), board(&res), engine(ChessEngine::GetInstance()), backgroundImg(res.GetSprite("Background"))
{
    activePiece = NULL;
    if(!wnd)
    {
        throw "Could not get window obj in class Game ctor";
    }
}
Game::~Game()
{
	EventSystem* system = EventSystem::GetInstance();
	system->UnregisterEvent(this);
	engine->FreeMemory();
    std::clog << "Game controller destroyed" << std::endl;
}
void Game::HandleEvent(const Event& e)
{
	if (e == LeftMouseDown)
	{
		int mX = wnd->GetMousePosX();
		int mY = wnd->GetMousePosY();
		activePiecePos = { mX, mY };		
		if (!activePiece)
		{
			// null if piece at position (mX, mY) doesn't exist
			activePiece = board.GetPieceAt(activePiecePos.x, activePiecePos.y);
		}
		else
		{
			auto tempPiece =  board.GetPieceAt(activePiecePos.x, activePiecePos.y);
			if (tempPiece)
				activePiece = tempPiece->IsWhite() == board.IsWhiteMove() ? tempPiece : activePiece;
		}
	}
	else if(e == LeftMouseMoved)
	{
		int mX = wnd->GetMousePosX();
		int mY = wnd->GetMousePosY();
		activePiecePos = { mX, mY };
		
	}
	else if(e == LeftMouseUp)
	{
		// Set clicked piece as active piece and if active piece already exist move that piece
		if(!hasGameStarted)
			return;
		if (activePiece && (isPvsP || isPvsAI))
		{
			Vector2 piecePos(activePiece->GetXPos(), activePiece->GetYPos());
			// returns false if move is illegal
			if (board.Move(piecePos, activePiecePos))
			{
				if (board.IsDraw())
				{
					hasGameStarted = false;
					isDraw = true;
					std::cout << "Draw!!!\n";
					EventSystem::GetInstance()->Invoke(Draw);
					EventSystem::GetInstance()->Invoke(GameOver);
				}
				if (board.IsCheck(!board.IsWhiteMove()))
					board.RevertLastMove(); // Illegal move
				std::string fen;
				fen = board.GetFEN();
				auto best = engine->GetBestmove(fen);
				while(best == "w")
				{
					best = engine->GetBestmove(fen);
				}
				if(best == "none")
				{
					/*if (board.IsStalemate())
					{
						isDraw = true;
						hasGameStarted = false;
						EventSystem::GetInstance()->Invoke(Draw);
						EventSystem::GetInstance()->Invoke(GameOver);
						return;
					}*/
					if (board.IsWhiteMove())
					{
						std::cout << "White are mated!\n";
						whiteMated = true;
					}
					else
					{
						std::cout << "Black are mated!\n";
						whiteMated = false;
					}
				}
			}
			activePiece = nullptr;
		}
	}
	else if(e == Check)
	{
		std::cout << "Check!!!\n";
		EventSystem::GetInstance()->Invoke(GameOver);
	}
	else if (e == ButtonDown_c)
	{
		board.RevertLastMove();
		board.RevertLastMove();
	}
	else if (e == IsQuitting)
	{
		engine->FreeMemory();
	}
	else if(e == ButtonDown_S)
	{
		if(!hasGameStarted)
		{
			engine->SendMsg("stop\n");
			engine->SendMsg("ucinewgame\n");
			board.StartGame();
			hasGameStarted = true;
			isMate = false;
			isDraw = false;
		}
	}
	else if(e == GameOver)
	{
		hasGameStarted = false;
	}
	else if(e == ButtonDown_E)
	{
		isPvsAI = !isPvsAI;
		std::cout << "PvAI mode turned " << (isPvsAI ? "on" : "off") << std::endl;
	}
	else if(e == ButtonDown_P)
	{
		isPvsP = !isPvsP;
		isPvsAI = false;
		activePiece = nullptr;
		std::cout << "PvP mode turned " << ( isPvsP ? "on" : "off") << std::endl;
	}
}
void Game::DisplayText(const std::string & txt, int fontSize, const Vec2D<unsigned>& position, SDL_Color color) const
{
		auto font = TTF_OpenFont("Font3.ttf", fontSize);

		SDL_Surface* surf = TTF_RenderText_Solid(font, txt.c_str(), color);

		Sprite drawImg(*surf);

		SDL_FreeSurface(surf);
		TTF_CloseFont(font);

		drawImg.Draw(position);
}
void Game::OnStart()
{
	EventSystem* system = EventSystem::GetInstance();
	system->RegisterEvent(this, IsQuitting);
	system->RegisterEvent(this, LeftMouseDown);
	system->RegisterEvent(this, LeftMouseUp);
	system->RegisterEvent(this, LeftMouseMoved);
	system->RegisterEvent(this, Check);
	system->RegisterEvent(this, ButtonDown_c);
	system->RegisterEvent(this, ButtonDown_S);
	system->RegisterEvent(this, ButtonDown_E);
	system->RegisterEvent(this, ButtonDown_P);
}
void Game::RunLogic()
{
	if (hasGameStarted && ((!isPvsP && !isPvsAI) ||  (isPvsAI && !board.IsWhiteMove())))
	{
		if(isMate)
		{
			EventSystem::GetInstance()->Invoke(Checkmate);
			EventSystem::GetInstance()->Invoke(GameOver);
			hasGameStarted = false;
			return;
		}
		std::string fen;
		if(!engine->HasRequestedMove())
		{
			fen = board.GetFEN();
		}
		auto move = engine->GetBestmove(fen);
		if (move.find("none") != std::string::npos)
		{
			/*if(board.IsStalemate())
			{
				isDraw = true;
				hasGameStarted = false;
				EventSystem::GetInstance()->Invoke(Draw);
				EventSystem::GetInstance()->Invoke(GameOver);
				return;
			}*/
			if (board.IsWhiteMove())
			{
				std::cout << "White is mated!\n";
				whiteMated = true;
			}
			else
			{
				std::cout << "Black is mated!\n";
				whiteMated = false;
			}
			isMate = true;
			return;
		}
		Vector2 from;
		Vector2 to;

		auto fX = move[0] - 'a';
		auto fY = 8 - static_cast<int>(move[1] - '0');
		auto tX = move[2] - 'a';
		auto tY = 8 - static_cast<int>(move[3] - '0');
		from.x = board.Index_toX(fX);
		from.y = board.Index_toY(fY);
		to.x = board.Index_toX(tX);
		to.y = board.Index_toY(tY);
		if (!board.Move(from, to))
		{
			std::cout << "Engine made illegal move: " << move << '\n';
		}
		if (board.IsDraw())
		{
			hasGameStarted = false;
			isDraw = true;
			std::cout << "Draw!!!\n";
			EventSystem::GetInstance()->Invoke(Draw);
			EventSystem::GetInstance()->Invoke(GameOver);
		}

		// is checkmate?
		if (!engine->HasRequestedMove())
		{
			fen = board.GetFEN();
		}
		move = engine->GetBestmove(fen, 15);
		if (move.find("none") != std::string::npos)
		{
			if (board.IsStalemate())
			{
				isDraw = true;
				hasGameStarted = false;
				EventSystem::GetInstance()->Invoke(Draw);
				EventSystem::GetInstance()->Invoke(GameOver);
				return;
			}
			if (board.IsWhiteMove())
			{
				std::cout << "White is mated!\n";
				whiteMated = true;
			}
			else
			{
				std::cout << "Black is mated!\n";
				whiteMated = false;
			}
			hasGameStarted = false;
			isMate = true;
		}
	}
}
void Game::DrawFrame()
{
	if (hasGameStarted || isMate)
	{
		board.Draw(activePiece ? Vector2(activePiece->GetXPos(), activePiece->GetYPos()) : Vector2(0, 0));
		if (activePiece)
		{
			activePiece->Draw(activePiecePos.x - activePiece->GetSprite().GetWidth() / 2, activePiecePos.y - activePiece->GetSprite().GetHeight() / 2);
		}
	}
	else if(!isMate)
	{
		backgroundImg.Draw({0, 0}, {1.63f, 1.8f});
		DisplayText("Press S to start new game", 20, {100, 20}, {200, 15, 100});
		DisplayText("Press E to switch AI mode (in game)", 20, {100, 40}, {200, 15, 100});
	}
	if(isMate)
	{
		auto wnd = MainWindow::GetMainWindow();
		const auto fontSize = 52;
		DisplayText("Checkmate", fontSize, { wnd->width / 2 - fontSize - 130, 150 }, { 159, 35, 4, 255 });
	}
	if(isDraw)
	{
		const auto fontSize = 50;
		auto font = TTF_OpenFont("Font3.ttf", fontSize);
		SDL_Surface* surf = TTF_RenderText_Solid(font, "Draw", SDL_Color{ 0, 0, 0, 255 });
		Sprite drawImg(*surf);
		SDL_FreeSurface(surf);
		TTF_CloseFont(font);
		auto wnd = MainWindow::GetMainWindow();
		drawImg.Draw({ wnd->width / 2 - fontSize, 50});
	}
	
}
