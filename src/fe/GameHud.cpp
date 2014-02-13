#include "fe/GameHud.h"

using namespace std;
using namespace sf;

namespace fe
{

// =============================================================================
//	NON-MEMBER FUNCTIONS
// =============================================================================
void SetOriginToCenter(Text& text)
{
	auto lbounds = text.getLocalBounds();

	text.setOrigin(lbounds.left + lbounds.width  * 0.5,
				   lbounds.top  + lbounds.height * 0.5);
}

void OutlineTextWithRect(const Text& text, RectangleShape& rect, UI32 padding = 10)
{
	auto lbounds = text.getLocalBounds();

	rect.setSize(Vector2f(lbounds.width + padding, lbounds.height + padding));
	rect.setOrigin((lbounds.width + padding) * 0.5, (lbounds.height + padding) * 0.5);
	rect.setPosition(text.getPosition());
}

// =============================================================================
//	CONSTRUCTORS, COPY CONSTRUCTOR, DESTRUCTOR, ASSIGNMENT OPERATOR
// =============================================================================
GameHud::GameHud(const Vector2f& size) :
	m_Player1Name("PLAYER 1", GameConstants::HudFont, GameConstants::HudPlayerNameTextSize),
	m_Player2Name("PLAYER 2", GameConstants::HudFont, GameConstants::HudPlayerNameTextSize),
	m_Scoreboard("0 - 0", 	  GameConstants::HudFont, GameConstants::HudScoreboardTextSize),
	m_GameMessage("", 		  GameConstants::HudFont, GameConstants::HudGameMessageTextSize),
	m_Background(size),
	m_MessageTimeout(0),
	m_MessageCallback(nullptr)
{
	m_Background.setFillColor(GameConstants::HudBackgroundColor);
	m_Player1Name.setColor(GameConstants::HudPlayerNameColor);
	m_Player2Name.setColor(GameConstants::HudPlayerNameColor);
	m_Scoreboard.setColor(GameConstants::HudScoreboardColor);
	m_GameMessage.setColor(GameConstants::HudGameMessageColor);
	m_GameMessageBackground.setFillColor(GameConstants::HudBackgroundColor);

	m_Player1Name.scale(0.9, 0.9);
	m_Player2Name.scale(0.9, 0.9);
	m_Scoreboard.scale(0.9, 0.9);
	m_GameMessage.scale(0.9, 0.9);

	SetOriginToCenter(m_Player1Name);
	SetOriginToCenter(m_Player2Name);
	SetOriginToCenter(m_Scoreboard);
	SetOriginToCenter(m_GameMessage);

	m_Background.setPosition(0, 0);
	m_Player1Name.setPosition(m_Player1Name.getLocalBounds().width * 0.5 + size.x * 0.03,
		size.y * 0.5);
	m_Player2Name.setPosition(size.x - m_Player2Name.getLocalBounds().width * 0.5 - size.x * 0.03,
		size.y * 0.5);
	m_Scoreboard.setPosition(size.x * 0.5, size.y * 0.5);
	m_GameMessage.setPosition(GameConstants::HudGameMessagePosition);
}

GameHud::~GameHud()
{
}

// =============================================================================
//	REGULAR METHODS
// =============================================================================
void GameHud::Update()
{
	if (m_MessageTimeout == 0)
		return;

	UI32 elapsed = static_cast<UI32>(m_MessageClock.getElapsedTime().asMilliseconds());

	if (elapsed >= m_MessageTimeout)
	{
		m_MessageTimeout = 0;
		m_MessageClock.restart();

		if (m_MessageCallback != nullptr)
			m_MessageCallback();
		else
			UpdateGameMessage("");
	}
}

void GameHud::UpdateScoreboard(const UI16* score)
{
	m_Scoreboard.setString(to_string(score[0]) + " - " + to_string(score[1]));
	SetOriginToCenter(m_Scoreboard);
}

void GameHud::UpdateGameMessage(const string& message, UI32 timeout)
{
	UpdateGameMessage(message, timeout, nullptr);
}

void GameHud::UpdateGameMessage(const std::string& message, const sf::Color& color,
	UI32 timeout)
{
	UpdateGameMessage(message, color, timeout, nullptr);
}

void GameHud::UpdateGameMessage(const std::string& message, UI32 timeout,
	const Callback& callback)
{
	UpdateGameMessage(message, Color::White, timeout, callback);
}

void GameHud::UpdateGameMessage(const string& message, const Color& messageColor,
	UI32 timeout, const Callback& callback)
{
	if (m_MessageTimeout > 0)
		return;

	m_GameMessage.setString(message);
	m_GameMessage.setColor(messageColor);
	SetOriginToCenter(m_GameMessage);
	OutlineTextWithRect(m_GameMessage, m_GameMessageBackground, 15);

	m_MessageTimeout  = timeout;
	m_MessageCallback = callback;

	m_MessageClock.restart();
}

void GameHud::Draw(RenderWindow& window) const
{
	window.draw(m_Background);
	window.draw(m_Player1Name);
	window.draw(m_Player2Name);
	window.draw(m_Scoreboard);
	window.draw(m_GameMessageBackground);
	window.draw(m_GameMessage);
}

// =============================================================================
//	GETTERS & SETTERS
// =============================================================================
string GameHud::GetGameMessage() const
{
	return m_GameMessage.getString();
}

}
