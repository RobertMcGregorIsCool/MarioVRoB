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
#include <SFML/Audio.hpp>

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

	void checkDirection();
	void setupFontAndText();
	void setupSprite();
	void setupSounds();
	void changeCharacter();

	void centerText();

	const float			m_moveSpeed = 1.5f; // Player movement speed - maybe other things too?

	sf::RenderWindow	m_window; // main SFML window
	sf::Font			m_mariofont; // font used by message
	sf::Text			m_characterName; // ROB: Hmm. Change from Pete's first version here?
	sf::Text			m_welcomeMessage; // text used for message on screen
	sf::Texture			m_logoTexture; // texture used for sfml logo
	sf::Sprite			m_logoSprite; // sprite used for sfml logo

	sf::Texture			m_marioTexture;	// texture used for image of mario and luigi
	sf::Sprite			m_marioSprite;	// sprite used for marioLuigi

	sf::Vector2f		m_location{ 400.0f, 200.0f }; // Mario's position. I dig how explicit the Vector types are.
	sf::Vector2f		m_spriteOffset{ 32.0f, 74.0f };
	bool				m_ImMario{ true }; // Why is THIS in curly braces?

	Direction			m_direction = Direction::None; // Where is mario going
	bool				m_exitGame; // control exiting game

	sf::SoundBuffer		m_snd_exclaimMario;
	sf::SoundBuffer		m_snd_exclaimLuigi;
	sf::Sound			m_soundSource_charName;
};

#endif // !GAME_HPP

