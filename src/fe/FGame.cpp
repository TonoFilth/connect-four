#include <iostream>
#include "fe/FGame.h"

using namespace std;
using namespace sf;

namespace fe
{

// =============================================================================
//	CONSTRUCTORS, COPY CONSTRUCTOR, DESTRUCTOR, ASSIGNMENT OPERATOR
// =============================================================================
FGame::FGame(const UI16 windowWidth, const UI16 windowHeight) :
	m_Board(nullptr),
	m_CurrentPlayer(TPlayerID::PLAYER2),
	m_CursorSprite(GameConstants::CursorTexture),
	m_BackgroundSprite(GameConstants::GameBackgroundTexture),
	m_Score{ 0, 0 },
	m_GameState(TGameState::PLAYING),
	m_HUD(Vector2f(windowWidth, windowWidth * 0.08)),
	m_Window(VideoMode(windowWidth, windowHeight), GameConstants::GameTitle)
{
	m_Window.setMouseCursorVisible(false);

	m_InternalState.SetRunCallback(bind(&FGame::MainLoop, this));
	m_InternalState.SetStopCallback(bind(&FGame::OnRoundEnd, this));
	
	m_Board = new Board(GameConstants::BoardSize,   GameConstants::SquareSize,
						Vector2i(windowWidth * 0.5, windowHeight * 0.5),
						GameConstants::SquareTexture);

	m_Board->SetPlayerChip(TPlayerID::PLAYER1, Sprite(GameConstants::Player1Texture));
	m_Board->SetPlayerChip(TPlayerID::PLAYER2, Sprite(GameConstants::Player2Texture));

	GameConstants::GameBackgroundTexture.setSmooth(true);
	m_BackgroundSprite.setOrigin(GameConstants::GameBackgroundTexture.getSize().x * 0.5,
								 GameConstants::GameBackgroundTexture.getSize().y * 0.5);
	m_BackgroundSprite.setPosition(windowWidth * 0.5, windowHeight * 0.5);

	NextPlayerTurn();
}

FGame::~FGame()
{
	if (m_Board != nullptr)
		delete m_Board;
	
	m_Board = nullptr;
}

// =============================================================================
//	PRIVATE AND PROTECTED METHODS
// =============================================================================
void FGame::MainLoop()
{
	Clock gameClock;
	F32 dt = 0;

	while (m_Window.isOpen())
    {
    	dt = gameClock.restart().asSeconds();

        Event event;
        while (m_Window.pollEvent(event))
        {
            HandleInput(event);
        }

        m_HUD.Update();
        m_BackgroundSprite.rotate(3 * dt);
        UpdateCursor();

        m_Window.clear(Color::Black);
        m_Window.draw(m_BackgroundSprite);
        m_Board->Draw(m_Window);
        m_HUD.Draw(m_Window);
        m_Window.draw(m_CursorSprite);
        m_Window.display();

        if (gameClock.getElapsedTime().asSeconds() < 0.016)
        	sleep(seconds(0.016 - gameClock.getElapsedTime().asSeconds()));
    }
}

void FGame::HandleInput(const Event& event)
{
	if (event.type == Event::Closed)
    	m_Window.close();

    if (m_InternalState.IsPaused())
		return;

	if (m_InternalState.IsStopped())
	{
		if (event.type == Event::MouseButtonPressed)
			NextRound();

		return;
	}

    if (event.type == Event::MouseButtonPressed)
    {
    	if (event.mouseButton.button == Mouse::Left)
        {
        	Vector2u bCoords;
            if (Window2BoardCoords(Vector2i(m_CursorSprite.getPosition().x, m_Board->GetBounds().top),
            	bCoords, *m_Board) && m_Board->GetMovement(bCoords.x, bCoords.y) == TPlayerID::NONE)
            {
            	bool put = false;
            	for (I32 i = m_Board->GetSize().y - 1; i >= 0; --i)
            	{
            		if (m_Board->GetMovement(i, bCoords.y) == TPlayerID::NONE)
            		{
            			m_Board->PutChip(i, bCoords.y, m_CurrentPlayer);
            			put = true;
            			break;
            		}
            	}

            	if (!put)
            		return;
            	
            	if (CheckWin())
            	{
            		m_GameState = TGameState::WON;
            		m_InternalState.Stop();
            		return;
            	}

            	if (CheckTie())
            	{
            		m_GameState = TGameState::TIE;
            		m_InternalState.Stop();
            		return;
            	}
           		
           		NextPlayerTurn();
            }
            else
            {
            	string prevMessage = m_HUD.GetGameMessage();
            	m_HUD.UpdateGameMessage("Invalid movement",
            							1000,
            							[&, prevMessage] () { m_HUD.UpdateGameMessage(prevMessage); });
            }
        }
    }
}

void FGame::UpdateCursor()
{
	auto mousePos 	 = Mouse::getPosition(m_Window);
	auto boardBounds = m_Board->GetBounds();

	UI32 midSquare = m_Board->GetSquareSize().x * 0.5;
	UI32 midCursor = m_CursorSprite.getLocalBounds().width * 0.5;

	if (mousePos.x <= boardBounds.left)
		mousePos.x = boardBounds.left + 1;

	if (mousePos.x >= boardBounds.left + boardBounds.width)
		mousePos.x = boardBounds.left + boardBounds.width - 1;

	if (mousePos.y <= boardBounds.top)
		mousePos.y = boardBounds.top + 1;

	if (mousePos.y >= boardBounds.top + boardBounds.height)
		mousePos.y = boardBounds.top + boardBounds.height - 1;

	Vector2u newPos(0, 0);
	Window2BoardCoords(mousePos, newPos, *m_Board);

	m_CursorSprite.setPosition(
		boardBounds.left + (newPos.y * m_Board->GetSquareSize().x) + midSquare - midCursor,
		boardBounds.top - boardBounds.top * 0.55);
}

void FGame::NextPlayerTurn()
{
	if (m_CurrentPlayer == TPlayerID::PLAYER1)
    {
    	m_CurrentPlayer = TPlayerID::PLAYER2;
    	m_HUD.UpdateGameMessage("Player's 2 turn");
    }
    else
    {
    	m_CurrentPlayer = TPlayerID::PLAYER1;
    	m_HUD.UpdateGameMessage("Player's 1 turn");
    }
}

void FGame::NextRound()
{
	m_GameState = TGameState::PLAYING;
	m_Board->Reset();
	NextPlayerTurn();
	m_InternalState.Run();
}

void FGame::OnRoundEnd()
{
	if (m_GameState == TGameState::WON)
	{
		if (m_CurrentPlayer == TPlayerID::PLAYER1)
		{
			++m_Score[0];
			m_HUD.UpdateGameMessage("Player 1 wins!", Color(12, 118, 232));
		}
		else
		{
			++m_Score[1];
			m_HUD.UpdateGameMessage("Player 2 wins!", Color(232, 12, 41));
		}

		m_HUD.UpdateScoreboard(m_Score);
	}

	if (m_GameState == TGameState::TIE)
		m_HUD.UpdateGameMessage("Tie!", Color::Red);
}

bool FGame::CheckWin() const
{
	TPlayerMovements movements = m_Board->GetMovements();

	for (UI32 i = 0; i < m_Board->GetSize().y; ++i)
		if (CheckWinRow(i, movements))
			return true;

	for (UI32 i = 0; i < m_Board->GetSize().x; ++i)
		if (CheckWinColumn(i, movements))
			return true;

	if (CheckWinLRDiagonal(movements) || CheckWinRLDiagonal(movements))
		return true;

	return false;
}

bool FGame::CheckWinRow(const UI32 row, const TPlayerMovements& movements) const
{
	UI32 consecutives = 0;

	for (auto& movement : movements[row])
	{
		++consecutives;

		if (movement != m_CurrentPlayer)
			consecutives = 0;

		if (consecutives == 4)
			break;
	}

	return consecutives == 4;
}

bool FGame::CheckWinColumn(const UI32 column, const TPlayerMovements& movements) const
{
	UI32 consecutives = 0;

	for (auto& row : movements)
	{
		++consecutives;

		if (row[column] != m_CurrentPlayer)
			consecutives = 0;

		if (consecutives == 4)
			break;
	}

	return consecutives == 4;
}

bool FGame::CheckTie() const
{
	auto movements = m_Board->GetMovements();

	for (auto& row : movements)
		for (auto& movement : row)
			if (movement == TPlayerID::NONE)
				return false;

	return true;
}

bool FGame::CheckWinLRDiagonal(const TPlayerMovements& movements) const
{
	if (movements.empty())
		return false;

	UI32 consecutives = 0;
	UI32 curRow = 0, curCol = 0;
	UI32 numRows = movements.size(), numCols = movements[0].size();

	for (UI32 i = 0; i < numCols; ++i)
	{
		curRow = 0;
		curCol = i;
		consecutives = 0;

		for (UI32 j = 0; j < numRows; ++j)
		{
			++consecutives;

			if (movements[curRow][curCol] != m_CurrentPlayer)
				consecutives = 0;

			++curRow;
			++curCol;

			if (curCol >= numCols || consecutives == 4)
				break;
		}

		if (consecutives == 4)
			return true;
	}

	for (UI32 i = 1; i < numRows; ++i)
	{
		curRow = i;
		curCol = 0;
		consecutives = 0;

		for (UI32 j = 0; j < numCols; ++j)
		{
			++consecutives;

			if (movements[curRow][curCol] != m_CurrentPlayer)
				consecutives = 0;

			++curRow;
			++curCol;

			if (curRow >= numRows || consecutives == 4)
				break;
		}

		if (consecutives == 4)
			return true;
	}

	return false;
}

bool FGame::CheckWinRLDiagonal(const TPlayerMovements& movements) const
{
	if (movements.empty())
		return false;

	I32 consecutives = 0;
	I32 curRow = 0, curCol = 0;
	I32 numRows = movements.size(), numCols = movements[0].size();

	for (I32 i = numCols - 1; i >= 0; --i)
	{
		curRow = 0;
		curCol = i;
		consecutives = 0;

		for (I32 j = 0; j < numRows; ++j)
		{
			++consecutives;

			if (movements[curRow][curCol] != m_CurrentPlayer)
				consecutives = 0;

			++curRow;
			--curCol;

			if (curCol < 0 || consecutives == 4)
				break;
		}

		if (consecutives == 4)
			return true;
	}

	for (I32 i = 1; i < numRows; ++i)
	{
		curRow = i;
		curCol = numCols - 1;
		consecutives = 0;

		for (I32 j = numCols - 1; j >= 0; --j)
		{
			++consecutives;

			if (movements[curRow][curCol] != m_CurrentPlayer)
				consecutives = 0;

			++curRow;
			--curCol;

			if (curRow >= numRows || consecutives == 4)
				break;
		}

		if (consecutives == 4)
			return true;
	}

	return false;
}

// =============================================================================
//	REGULAR METHODS
// =============================================================================
void FGame::Run()
{
	m_InternalState.Run();
}

}
