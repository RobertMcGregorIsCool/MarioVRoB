/// <summary>
/// @author RoBert McGregor LOGIN: c00302210
/// START:  19th Jan 2024	END: ???
/// EST:	4 hours?		ACTUAL:
/// KNOWN BUGS: 
///
/// you need to change the above lines or lose marks <= Roger, wilco.
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <array>
#include <iostream>
#include <cmath>

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void move();
	void setuMovement();
	void eBallMovement();
	void eBallCollision();
	void setuCollision();
	void safeCollision();

	sf::Vector2f normalizeV2f(const sf::Vector2f source);
	float vector2fSqrMag(const sf::Vector2f posA, const sf::Vector2f posB, const bool printDist = false);

	void checkDirection();
	void setupFontAndText();
	void setupSprite();
	void setupSounds();
	void changeCharacter();

	void centerText();

	bool collidingWithBounds(float bot, float top, float left, float rigt);

	void reset();

	const float			m_moveSpeed = 1.0f;					// Player movement speed

	sf::RenderWindow	m_window;							// main SFML window
	sf::Font			m_mariofont;						// font used by message
	sf::Text			m_characterName;					// ROB: Hmm. Change from Pete's first version (ie. Github) here?
	sf::Text			m_welcomeMessage;					// text used for message on screen
	sf::Texture			m_logoTexture;						// texture used for sfml logo
	sf::Texture			m_playfieldTexture;					// texture used for playfield
	sf::Sprite			m_logoSprite;						// sprite used for sfml logo
	sf::Sprite			m_playfieldSprite;					// sprite used for playfield

	// TopLeft 80.0f, 10.0f, TopRigt 600.0f, 10.0f, BotLef 80.0f, 440.0f, BotRit 600.0f, 440.0f
	std::array<sf::Vector2f, 4> m_setuWaypoints = { sf::Vector2f(144.0f, 74.0f), sf::Vector2f(664.0f, 74.0f), sf::Vector2f(664.0f, 504.0f), sf::Vector2f(144.0f, 504.0f) };
	const float			M_SETU_WAYPOINT_THRESHOLD = 8.0f;
	const float			M_SETU_HIT_THRESH = 9000.0f;		// At or less than this distance to the player, the ball hits the player.
	sf::Texture			m_setuTexture;						// Texture of SETUMonster
	sf::Sprite			m_setuSprite;						// Sprite for SETUMonster
	sf::Vector2f		m_setuPosition = m_setuWaypoints[1];// Position of SETUMonster
	int					m_setuPosIndex	= 2;				// Index to step through Setu Waypoints
	sf::Vector2f		m_setuDirection{ 0.0f, 0.0f };
	const float			M_SETU_MOVE_SPEED = 1.1f;

	bool				m_eBallTravelling = false;
	const float			M_E_BALL_RESET_DIST = 500000.0f;	// At this distance from SETU Monster, ball teleports to SETU Monster
	const float			M_E_BALL_MOVE_SPEED = 8.0f;
	const float			M_E_BALL_HIT_THRESH = 1000.0f;		// At or less than this distance to the player, the ball hits the player.
	sf::Texture			m_eBallTexture;						// Texture of energyball.
	sf::Sprite			m_eBallSprite;						// Sprite which displays the energyball
	sf::Vector2f		m_eBallPosition{ 656.0f, 500.0f };	// Position of energyBall;
	const sf::Vector2f	M_E_BALL_STARTPOS{ 656.0f, 500.0f };// Energyball start pos for reset
	sf::Vector2f		m_eBallDirection{ 0.0f, 0.0f };
	sf::SoundBuffer		m_eBallFiringSnd;					// Played as Energyball begins travel
	sf::Sound			m_eBallFiringSoundSource;
	

	sf::Texture			m_marioTexture;						// texture used for image of mario and luigi
	sf::Sprite			m_marioSprite;						// sprite used for mario and Luigi
	sf::Vector2f		m_location		{ 120.0f, 500.0f };	// Mario's position. I dig how explicit the Vector types are.
	const sf::Vector2f	M_MARIO_STARTPOS{ 120.0f, 500.0f }; // Mario start pos for reset
	sf::Vector2f		m_lastValidPos	{ 0.0f, 0.0f };		// Last valid position while not touching collider.
	sf::Vector2f		m_spriteOffset	{ 32.0f, 74.0f };
	bool				m_ImMario		{ true };			// Why is THIS in curly braces?

	Direction			m_direction = Direction::None;	// Where is mario going

	sf::SoundBuffer		m_snd_exclaimMario;					// 'Mario!' sound sample
	sf::SoundBuffer		m_snd_exclaimLuigi;					// 'Luigi!' sound sample
	sf::Sound			m_soundSource_charName;				// Sound emitter for character name

	sf::Music			m_music_angelAttack;				// 'Angel Attack' theme from Evangelion
	sf::SoundBuffer		m_resetSound;
	sf::Sound			m_soundSourceReset;

	sf::RectangleShape	m_rectangleShape;					// Central collision obstacle over SETU
	sf::RectangleShape	m_rectDebug;						// Tool to help designer identify relevant screen space numbers

	sf::Color			m_clearColor{ 255,0,0,0 };			// Transparent 'color' for m_rectangleShape

	bool				m_exitGame;							// control exiting game
};

#endif // !GAME_HPP

	