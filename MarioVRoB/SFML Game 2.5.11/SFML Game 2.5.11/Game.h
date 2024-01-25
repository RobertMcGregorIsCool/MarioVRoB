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

	sf::Vector2f normalizeV2f(const sf::Vector2f source);

	void checkDirection();
	void setupFontAndText();
	void setupSprite();
	void setupSounds();
	void changeCharacter();

	void centerText();

	bool collidingWithBounds(float bot, float top, float left, float rigt);

	const float			m_moveSpeed = 1.5f; // Player movement speed - maybe other things too?

	sf::RenderWindow	m_window; // main SFML window
	sf::Font			m_mariofont; // font used by message
	sf::Text			m_characterName; // ROB: Hmm. Change from Pete's first version (ie. Github) here?
	sf::Text			m_welcomeMessage; // text used for message on screen
	sf::Texture			m_logoTexture; // texture used for sfml logo
	sf::Sprite			m_logoSprite; // sprite used for sfml logo

	sf::Texture			m_marioTexture;	// texture used for image of mario and luigi
	sf::Sprite			m_marioSprite;	// sprite used for marioLuigi

	// TopLeft 80.0f, 40.0f, TopRigt 600.0f, 40.0f, BotLef 80.0f, 440.0f, BotRit 600.0f, 440.0f
	std::array<sf::Vector2f, 4> m_setuWaypoints = { sf::Vector2f(80.0f, 40.0f), sf::Vector2f(600.0f, 40.0f), sf::Vector2f(600.0f, 440.0f), sf::Vector2f(80.0f, 440.0f) };
	sf::Texture			m_setuTexture; // Texture of SETUMonster
	sf::Sprite			m_setuSprite;	// Sprite for SETUMonster
	sf::Vector2f		m_setuPosition = m_setuWaypoints[3]; // Position of SETUMonster
	int					m_setuPosIndex	= 3;
	sf::Vector2f		m_setuDirection{ 0.0f, 0.0f };

	sf::Texture			m_eBallTexture; // Texture of energyball.
	sf::Sprite			m_eBallSprite; // Sprite which displays the energyball
	sf::Vector2f		m_eBallPosition{ 640.0f, 520.0f }; // Position of energyBall;
	

	sf::Vector2f		m_location{ 20.0f, 550.0f }; // Mario's position. I dig how explicit the Vector types are.
	sf::Vector2f		m_lastValidPos{ 0.0f, 0.0f }; // Last valid position while not touching collider.
	sf::Vector2f		m_spriteOffset{ 32.0f, 74.0f };
	bool				m_ImMario{ true }; // Why is THIS in curly braces?

	Direction			m_direction = Direction::None; // Where is mario going
	bool				m_exitGame; // control exiting game

	sf::SoundBuffer		m_snd_exclaimMario;
	sf::SoundBuffer		m_snd_exclaimLuigi;
	sf::Sound			m_soundSource_charName;

	sf::Music			m_music_angelAttack;

	sf::RectangleShape	m_rectangleShape;
	sf::RectangleShape	m_rectDebug;

	sf::Color			m_clearColor{ 255,0,0,2 };
};

#endif // !GAME_HPP

