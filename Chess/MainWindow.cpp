#include "MainWindow.h"
#include "EventSystem.h"

MainWindow* MainWindow::instance = nullptr;
SDL_Renderer* MainWindow::renderer = nullptr;

MainWindow* MainWindow::GetMainWindow()
{
    if(!instance)
        instance = new MainWindow();
    return instance;
}
MainWindow::MainWindow()
{
    wnd = SDL_CreateWindow(
        "Chess",                           // window title
        SDL_WINDOWPOS_CENTERED,            // initial x position
        SDL_WINDOWPOS_CENTERED,            // initial y position
        width,                             // width, in pixels
        height,                            // height, in pixels
		SDL_WINDOW_OPENGL                  // flags
    );
    if (!wnd)
    {
        throw std::runtime_error("Could not create window");
    }
    renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
        throw std::runtime_error("Could not create renderer for main window");

    ResetClearScreenColor();
}
MainWindow::~MainWindow()
{
	if(renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;		
	}
	if(wnd)
	{
		SDL_DestroyWindow(wnd);
		wnd = nullptr;		
	}
}
SDL_Renderer* MainWindow::GetRenderer()
{
    if(!renderer)
        throw std::runtime_error("Someone is trying to access renderer but it is NULL");
    return renderer;
}
bool MainWindow::ProcEvents()
{
    leftMouseClicked = false;
    SDL_Event event;
    SDL_KeyboardEvent e;
    SDL_MouseButtonEvent mouseClick; 
	EventSystem* system = EventSystem::GetInstance();
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_APP_TERMINATING:
			system->Invoke(IsQuitting);
            return true;
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            e = event.key;
			if (e.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				return true;
			}
			if (e.keysym.scancode == SDL_SCANCODE_C)
			{
				system->Invoke(ButtonDown_c);
				break;
			}
			if (e.keysym.scancode == SDL_SCANCODE_S)
			{
				system->Invoke(ButtonDown_S);
				break;
			}
			if (e.keysym.scancode == SDL_SCANCODE_E)
			{
				system->Invoke(ButtonDown_E);
				break;
			}
			if (e.keysym.scancode == SDL_SCANCODE_P)
			{
				system->Invoke(ButtonDown_P);
				break;
			}
        case SDL_MOUSEBUTTONDOWN:
			mouseClick = event.button;
			mouseX = mouseClick.x;
			mouseY = mouseClick.y;
			system->Invoke(LeftMouseDown);
            break;
        case SDL_MOUSEMOTION:
			mouseClick = event.button;
			mouseX = mouseClick.x;
			mouseY = mouseClick.y;
			system->Invoke(LeftMouseMoved);
            break;
        case SDL_MOUSEBUTTONUP:
            mouseClick = event.button;
			mouseX = mouseClick.x;
			mouseY = mouseClick.y;
			system->Invoke(LeftMouseUp);
            break;
        }
    }
    return false;
}
int MainWindow::GetMousePosX() const
{
    return mouseX;
}
int MainWindow::GetMousePosY() const
{
    return mouseY;
}

void MainWindow::FreeMemory()
{
	delete instance;
	instance = nullptr;
}

SDL_Window* MainWindow::GetWindow() const
{
    return wnd;
}
void MainWindow::ClearScreen() const
{
    if(renderer)
    {
        ResetClearScreenColor();
        SDL_RenderClear(renderer);
    }
}
void MainWindow::DisplayScreen()
{
    SDL_SetRenderDrawBlendMode(GetRenderer(), SDL_BLENDMODE_NONE);
    if(renderer)
        SDL_RenderPresent(renderer);
}
void MainWindow::ResetClearScreenColor()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
