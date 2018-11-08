#include "Resources.h"
#include <iostream>
#include <stdexcept>

Resources::Resources()
{
    std::clog << "Loading resources..." << std::endl;
    LoadResource("WhiteKing",   "Sprites\\whiteKing.png");
    LoadResource("WhiteQueen",  "Sprites\\whiteQueen.png");
    LoadResource("WhiteRook",   "Sprites\\whiteRook.png");
    LoadResource("WhiteKnight", "Sprites\\whiteKnight.png");
    LoadResource("WhiteBishop", "Sprites\\whiteBishop.png");
    LoadResource("WhitePawn",   "Sprites\\whitePawn.png");
    LoadResource("BlackKing",   "Sprites\\blackKing.png");
    LoadResource("BlackQueen",  "Sprites\\blackQueen.png");
    LoadResource("BlackRook",   "Sprites\\blackRook.png");
    LoadResource("BlackKnight", "Sprites\\blackKnight.png");
    LoadResource("BlackBishop", "Sprites\\blackBishop.png");
    LoadResource("BlackPawn",   "Sprites\\blackPawn.png");
    LoadResource("Background",  "Sprites\\background.jpg");
    std::clog << "Done loading resources" << std::endl;
}

Resources::~Resources()
{
    std::clog << "Resources destroyed" << std::endl;
}

void Resources::LoadResource(const std::string& tag, const std::string& path)
{
	if (sprites.find(tag) == sprites.end())
	{
		Sprite s(path);
		sprites.insert(std::pair<std::string, Sprite>(tag, s));
	}
}

Sprite Resources::GetSprite(const std::string& tag) const
{
    try
    {
        return sprites.at(tag);
    }
    catch(const std::out_of_range& error)
    {
        std::string msg = "Sprite with tag: " + tag + " not found";
        throw msg;
    }
    catch (...)
    {
        throw;
    }
}
