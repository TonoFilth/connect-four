#ifndef __PLAYER_TYPES_H__
#define __PLAYER_TYPES_H__

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

enum class TPlayerID
{
	NONE 	= 0,
	PLAYER1 = 1,
	PLAYER2 = 2
};

typedef std::map<TPlayerID, sf::Sprite> TPlayerChipMap;
typedef std::vector<std::vector<TPlayerID> > TPlayerMovements;

#endif