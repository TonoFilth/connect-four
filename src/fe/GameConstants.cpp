#include "fe/GameConstants.h"

using namespace std;
using namespace sf;

namespace fe
{

// =============================================================================
//	CONSTANTS
// =============================================================================
const string GameConstants::GameTitle("Connect Four");
const string GameConstants::GameBackgroundTextureFile("assets/images/background.jpg");
const string GameConstants::Player1TextureFile("assets/images/chip-a.png");
const string GameConstants::Player2TextureFile("assets/images/chip-b.png");
const string GameConstants::SquareTextureFile("assets/images/square.png");
const string GameConstants::CursorChipATextureFile("assets/images/cursor-chip-a.png");
const string GameConstants::CursorChipBTextureFile("assets/images/cursor-chip-b.png");
const string GameConstants::HudFontFile("assets/fonts/Exo-Black.otf");

const Vector2u GameConstants::WindowSize(600, 600);
const Vector2u GameConstants::BoardSize(7, 6);
const Vector2u GameConstants::SquareSize(64, 64);

const Color GameConstants::HudBackgroundColor(0, 0, 0, 200);
const Color GameConstants::HudPlayerNameColor(Color::White);
const Color GameConstants::HudScoreboardColor(Color::White);
const Color GameConstants::HudGameMessageColor(Color::Black);
const Color GameConstants::HudGameMessageBackgroundColor(255, 255, 255, 180);
const Color GameConstants::HudGameMessageOutlineColor(60, 67, 50);

const UI32 GameConstants::HudPlayerNameTextSize  = 24;
const UI32 GameConstants::HudScoreboardTextSize  = 24;
const UI32 GameConstants::HudGameMessageTextSize = 18;

const Vector2f GameConstants::HudGameMessagePosition(GameConstants::WindowSize.x * 0.5,
	GameConstants::WindowSize.y - GameConstants::WindowSize.y * 0.05);

// =============================================================================
//	STATIC FIELDS
// =============================================================================
Texture GameConstants::GameBackgroundTexture;
Texture GameConstants::Player1Texture;
Texture GameConstants::Player2Texture;
Texture GameConstants::SquareTexture;
Texture GameConstants::CursorChipATexture;
Texture GameConstants::CursorChipBTexture;

Font GameConstants::HudFont;

bool GameConstants::st_Initialized = false;

// =============================================================================
//	STATIC METHODS
// =============================================================================
bool GameConstants::Init()
{
	if (st_Initialized)
		return false;

	if (!GameBackgroundTexture.loadFromFile(GameBackgroundTextureFile) ||
		!Player1Texture.loadFromFile(Player1TextureFile) ||
		!Player2Texture.loadFromFile(Player2TextureFile) ||
		!SquareTexture.loadFromFile(SquareTextureFile)   ||
		!CursorChipATexture.loadFromFile(CursorChipATextureFile) ||
		!CursorChipBTexture.loadFromFile(CursorChipBTextureFile))
	{
		cerr << "Can't load textures" << endl;
		return false;
	}

	if (!HudFont.loadFromFile(HudFontFile))
	{
		cerr << "Can't load font file: " << HudFontFile << endl;
		return false;
	}

	st_Initialized = true;

	return true;
}

}
