#ifndef __FGAME_H__
#define __FGAME_H__

#include "fe/Board.h"
#include "fe/StateController.h"
#include "fe/GameConstants.h"
#include "fe/GameHud.h"

namespace fe
{

enum class TGameState
{
	PLAYING,
	WON,
	TIE
};

class FGame
{
private:
	Board* 	  		 m_Board;
	TPlayerID 		 m_CurrentPlayer;
	sf::Sprite		 m_CursorSprite;
	sf::Sprite 		 m_BackgroundSprite;
	UI16	  		 m_Score[2];
	StateController  m_InternalState;
	TGameState		 m_GameState;
	GameHud			 m_HUD;
	sf::RenderWindow m_Window;

	void MainLoop();
	void HandleInput(const sf::Event& event);
	void UpdateCursor();
	void NextPlayerTurn();
	void NextRound();
	void OnRoundEnd();

	bool CheckWin() const;
	bool CheckWinRow(const UI32 row, const TPlayerMovements& movements) const;
	bool CheckWinColumn(const UI32 col, const TPlayerMovements& movements) const;
	bool CheckWinLRDiagonal(const TPlayerMovements& movements) const;
	bool CheckWinRLDiagonal(const TPlayerMovements& movements) const;
	bool CheckTie() const;

public:
	FGame(const UI16 windowWidth = 500, const UI16 windowHeight = 500);
	FGame(const FGame& toCopy);
	FGame& operator=(const FGame& toCopy);
	~FGame();

	void Run();
};

}

#endif
