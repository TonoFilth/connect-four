#ifndef __GAME_HUD_H__
#define __GAME_HUD_H__

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

#include "fe/GameConstants.h"
#include "fe/PlayerTypes.h"

namespace fe
{

typedef std::function<void()> Callback;

class GameHud
{
private:
	sf::Text m_Player1Name;
	sf::Text m_Player2Name;
	sf::Text m_Scoreboard;
	sf::Text m_GameMessage;

	sf::RectangleShape m_Background;
	sf::RectangleShape m_GameMessageBackground;

	UI32 	  m_MessageTimeout;
	sf::Clock m_MessageClock;
	Callback  m_MessageCallback;

public:
	GameHud(const sf::Vector2f& size);
	GameHud(const GameHud& toCopy);
	GameHud& operator=(const GameHud& toCopy);
	~GameHud();

	void Update();
	void UpdateScoreboard(const UI16* score);
	void UpdateGameMessage(const std::string& message, UI32 timeout = 0);
	void UpdateGameMessage(const std::string& message, const sf::Color& color, UI32 timeout = 0);
	void UpdateGameMessage(const std::string& message, UI32 timeout, const Callback& callback);
	void UpdateGameMessage(const std::string& message, const sf::Color& messageColor,
		UI32 timeout, const Callback& callback);
	void Draw(sf::RenderWindow& window) const;

	std::string GetGameMessage() const;
};

}

#endif
