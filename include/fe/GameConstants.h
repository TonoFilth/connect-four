#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

#include <iostream>
#include <SFML/Graphics.hpp>

#include "fe/BasicTypes.h"

namespace fe
{

class GameConstants
{
private:
	static const std::string GameBackgroundTextureFile;
	static const std::string Player1TextureFile;
	static const std::string Player2TextureFile;
	static const std::string SquareTextureFile;
	static const std::string CursorTextureFile;
	static const std::string HudFontFile;
	
	static bool st_Initialized;

public:
	GameConstants();
	GameConstants(const GameConstants& toCopy);
	GameConstants& operator=(const GameConstants& toCopy);
	~GameConstants();

	static const std::string GameTitle;

	static sf::Texture GameBackgroundTexture;
	static sf::Texture Player1Texture;
	static sf::Texture Player2Texture;
	static sf::Texture SquareTexture;
	static sf::Texture CursorTexture;

	static const sf::Vector2u WindowSize;
	static const sf::Vector2u BoardSize;
	static const sf::Vector2u SquareSize;

	static const sf::Color HudBackgroundColor;
	static const sf::Color HudPlayerNameColor;
	static const sf::Color HudScoreboardColor;
	static const sf::Color HudGameMessageColor;

	static const UI32 HudPlayerNameTextSize;
	static const UI32 HudScoreboardTextSize;
	static const UI32 HudGameMessageTextSize;

	static const sf::Vector2f HudGameMessagePosition;

	static sf::Font HudFont;

	static bool Init();
};

}

#endif
