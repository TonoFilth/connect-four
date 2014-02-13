#include "fe/Board.h"

using namespace std;
using namespace sf;

namespace fe
{

// =============================================================================
//	NON-MEMBER FUNCTIONS
// =============================================================================
bool Window2BoardCoords(const sf::Vector2i& windowCoords,
						sf::Vector2u& boardCoords, const Board& board)
{
	IntRect boardBounds(board.GetBounds());

	if (!boardBounds.contains(windowCoords))
		return false;

	boardCoords.x = (windowCoords.y - boardBounds.top)   / board.GetSquareSize().y;
	boardCoords.y = (windowCoords.x - boardBounds.left)  / board.GetSquareSize().x;

	return true;
}

// =============================================================================
//	CONSTRUCTORS, COPY CONSTRUCTOR, DESTRUCTOR, ASSIGNMENT OPERATOR
// =============================================================================
Board::Board(const Vector2u& boardSize, const Vector2u& squareSize,
		     const Vector2i& position,  const Texture& squareTexture) :
	m_BoardSize(boardSize),
	m_SquareSize(squareSize),
	m_Position(position),
	m_Bounds(position.x - (boardSize.x * 0.5 * squareSize.x),
			 position.y - (boardSize.y * 0.5 * squareSize.y),
			 boardSize.x * squareSize.x,
			 boardSize.y * squareSize.y)
{
	UI32 curX = m_Bounds.left;
	UI32 curY = m_Bounds.top;
	
	F32 scaleX = squareSize.x / static_cast<F32>(squareTexture.getSize().x);
	F32 scaleY = squareSize.y / static_cast<F32>(squareTexture.getSize().y);
	
	Sprite square(squareTexture);
	square.setScale(scaleX, scaleY);

	for (UI32 i = 0; i < boardSize.y; ++i)
	{
		for (UI32 j = 0; j < boardSize.x; ++j)
		{
			square.setPosition(curX, curY);
			m_Squares.push_back(square);
			curX += squareSize.x;
		}

		curX  = m_Bounds.left;
		curY += squareSize.y;
	}

	for (UI32 i = 0; i < boardSize.y; ++i)
		m_Movements.push_back(vector<TPlayerID>(boardSize.x, TPlayerID::NONE));
}

Board::~Board()
{
}

// =============================================================================
//	REGULAR METHODS
// =============================================================================
bool Board::PutChip(UI32 row, UI32 col, const TPlayerID playerId)
{
	if (m_PlayerChipMap.find(playerId) == m_PlayerChipMap.end() ||
		row >= m_BoardSize.y || col >= m_BoardSize.x)
		return false;

	Sprite* s = &m_PlayerChipMap[playerId];
	I32 xPos = m_Bounds.left + col * m_SquareSize.x;
	I32 yPos = m_Bounds.top  + row * m_SquareSize.y;

	xPos += m_SquareSize.x * 0.5 - s->getGlobalBounds().width * 0.5;
	yPos += m_SquareSize.y * 0.5 - s->getGlobalBounds().height  * 0.5;

	s->setPosition(xPos, yPos);
	
	m_Chips.push_back(*s);
	m_Movements[row][col] = playerId;

	return true;
}

void Board::Draw(RenderWindow& window) const
{
	for (auto& chip : m_Chips)
		window.draw(chip);

	for (auto& square : m_Squares)
		window.draw(square);
}

void Board::Reset()
{
	m_Chips.clear();
	m_Movements.clear();

	for (UI32 i = 0; i < m_BoardSize.y; ++i)
		m_Movements.push_back(vector<TPlayerID>(m_BoardSize.x, TPlayerID::NONE));
}

// =============================================================================
//	GETTERS & SETTERS
// =============================================================================
const Vector2u& Board::GetSize() const
{
	return m_BoardSize;
}

const Vector2u& Board::GetSquareSize() const
{
	return m_SquareSize;
}

const IntRect& Board::GetBounds() const
{
	return m_Bounds;
}

TPlayerID Board::GetMovement(UI32 row, UI32 col) const
{
	if (row >= m_Movements.size() ||
		col >= m_Movements[row].size())
		return TPlayerID::NONE;

	return m_Movements[row][col];
}

TPlayerMovements Board::GetMovements() const
{
	return m_Movements;
}

void Board::SetPlayerChip(TPlayerID id, const Sprite& chipSprite)
{
	m_PlayerChipMap[id] = chipSprite;
}

}
