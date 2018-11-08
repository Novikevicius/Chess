#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "MainWindow.h"
#include "Game.h"
#include "SDL_ttf.h"
#include <string>

#include "SDL_config_windows.h"
#ifndef _WINDLL
#define HAVE_LIBC
#endif

#define DEBUG 1

static const int FPS = 60;


int main(int argc, char* argv[])
{
	try
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		IMG_Init(IMG_INIT_PNG);
		TTF_Init();
		MainWindow* mainWnd = MainWindow::GetMainWindow();
		Game game(mainWnd);
		game.OnStart();

		while (!mainWnd->ProcEvents())
		{
			mainWnd->ClearScreen();
			game.DrawFrame();
			MainWindow::DisplayScreen();
			Uint32 ticksCount = SDL_GetTicks();
			game.RunLogic();

			ticksCount = SDL_GetTicks() - ticksCount;
			if (ticksCount < 1000 / FPS)
				SDL_Delay(1000 / FPS - ticksCount);
		}
		mainWnd->FreeMemory();
	}
	catch (const std::string& msg)
	{
		std::cerr << "Error: " << msg << std::endl;
		std::cin.get();
	}
	catch (const char * msg)
	{
		std::cerr << "Error: " << msg << std::endl;
		std::cin.get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		std::cin.get();
	}
	catch (...)
	{
		std::cerr << "Unknown error occurred" << std::endl;
		std::cin.get();
	}
	SDL_Quit();
	return 0;
}
