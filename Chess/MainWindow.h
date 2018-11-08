#pragma once

#include <SDL.h>

class MainWindow
{
    // Singleton
    private:
        static MainWindow* instance;
        MainWindow();
    public:
        static MainWindow* GetMainWindow();
		~MainWindow();
        MainWindow(const MainWindow & other) = delete;
        MainWindow& operator=(const MainWindow& other) = delete;
    public:
		static const int width = 550;
		static const int height = 440;
        int mouseX;
        int mouseY;
    private:
        SDL_Window* wnd;
        bool leftMouseClicked;
    public:
		void FreeMemory();
        static SDL_Renderer* renderer;
        SDL_Window* GetWindow() const;
        bool ProcEvents();
        int GetMousePosX() const;
        int GetMousePosY() const;
        void ClearScreen() const;
        static void DisplayScreen();
        static void ResetClearScreenColor();
        static SDL_Renderer* GetRenderer();


};
