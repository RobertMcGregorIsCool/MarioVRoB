/// <summary>
/// @author RoBert McGregor LOGIN: c00302210
/// START:  19th Jan 2024	END: ???
/// EST:	4 hours?		ACTUAL:
/// KNOWN BUGS: 
///
/// you need to change the above lines or lose marks <= Roger, wilco.
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup the image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "I SEE YOU" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupSounds(); // Load sounds, prep speaker.
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but method needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
		changeCharacter();
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	checkDirection();
	move();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	//m_window.draw(m_welcomeMessage); // Get rid of this before Pete sees it!

	

	m_window.draw(m_characterName);
	// m_window.draw(m_logoSprite);

	m_window.draw(m_rectangleShape);

	m_window.draw(m_marioSprite);

	m_window.draw(m_rectDebug); // Turn this off when I've found the right values.

	m_window.display();
}

void Game::move()
{
	sf::Vector2f movement{ 0.0f, 0.0f };
	float playerBot = m_location.y + (m_marioSprite.getLocalBounds().height * 0.85f);
	float playerTop = m_location.y + (m_marioSprite.getLocalBounds().height * 0.4f);
	float playerLeft = m_location.x + (m_marioSprite.getLocalBounds().width * 0.25f);
	float playerRigt = m_location.x + (m_marioSprite.getLocalBounds().width * 0.75f);

	float rectTop = m_rectangleShape.getGlobalBounds().top;
	float rectBot = rectTop + m_rectangleShape.getGlobalBounds().height;
	float rectLeft = m_rectangleShape.getGlobalBounds().left;
	float rectRigt = rectLeft + m_rectangleShape.getGlobalBounds().width;


	switch (m_direction)
	{
	case Direction::None:
		break;
	case Direction::Up:
		if (playerBot > 0.0f)
		{
			if (!((playerBot < rectBot && playerBot > rectTop) && (playerLeft > rectLeft && playerRigt < rectRigt)))
			{
				movement.y = -m_moveSpeed;
			}
			else
			{
				std::cout << "You sonova bitch, you did it!\n";
				m_location = sf::Vector2f(m_marioSprite.getPosition().x, rectBot - (m_marioSprite.getLocalBounds().height * 0.838f));
			}
		}
		break;
	case Direction::Down:
		if (playerTop < m_window.getSize().y)
		{
			if (!((playerBot < rectBot && playerBot > rectTop) && (playerLeft > rectLeft && playerRigt < rectRigt)))
			{
				movement.y = m_moveSpeed;
			}
			
		}
		break;
	case Direction::Left:
		if (playerLeft > 0.0f)
		{
			if (!((playerBot < rectBot && playerBot > rectTop) && (playerLeft > rectLeft && playerRigt < rectRigt)))
			{
				movement.x = -m_moveSpeed;
			}
			
		}
		break;
	case Direction::Right:
		if (playerRigt< m_window.getSize().x)
		{
			if (!((playerBot < rectBot && playerBot > rectTop) && (playerLeft > rectLeft && playerRigt < rectRigt)))
			{
				movement.x = m_moveSpeed;
			}
			
		}
		break;
	default:
		break;
	}
	m_location += movement;
	m_marioSprite.setPosition(m_location);
}

void Game::checkDirection()
{
	m_direction = Direction::None;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_direction = Direction::Up;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_direction = Direction::Down;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_direction = Direction::Left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_direction = Direction::Right;
	}
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_mariofont.loadFromFile("ASSETS\\FONTS\\SuperMario256.ttf"))
	{
		std::cout << "problem loading SuperMario256 font" << std::endl;
	}
	m_characterName.setFont(m_mariofont);
	m_characterName.setString("Mario");
	m_characterName.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_characterName.setPosition(40.0f, 40.0f);
	m_characterName.setCharacterSize(80U);
	m_characterName.setOutlineColor(sf::Color::Red);
	m_characterName.setFillColor(sf::Color::Black);
	m_characterName.setOutlineThickness(3.0f);
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}

	if (!m_marioTexture.loadFromFile("ASSETS\\IMAGES\\mario-luigi-64.png"))
	{// Simple error message if previous call fails
		std::cout << "problem loading italian plumbers" << std::endl;
	}

	m_marioSprite.setTexture(m_marioTexture);
	m_marioSprite.setPosition(m_location);
	m_marioSprite.setTextureRect(sf::IntRect(0, 0, 64, 148));

	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f); // So I need to change this stuff so the text follows the character around.

	m_rectangleShape.setSize(sf::Vector2f(256.0f, 256.0f));
	m_rectangleShape.setPosition((m_window.getSize().x * 0.5f) - m_rectangleShape.getSize().x * 0.5f, (m_window.getSize().y * 0.5f) - m_rectangleShape.getSize().y * 0.5f);
	m_rectangleShape.setFillColor(sf::Color::Cyan);

	m_rectDebug.setSize(sf::Vector2f(256.0f, 4.0f));
	m_rectDebug.setPosition((m_window.getSize().x * 0.5f) - m_rectDebug.getSize().x * 0.5f, m_rectangleShape.getGlobalBounds().top + m_rectangleShape.getGlobalBounds().height);
	m_rectDebug.setFillColor(sf::Color::Red);
}

void Game::setupSounds()
{
	if (!m_snd_exclaimMario.loadFromFile("ASSETS\\SOUNDS\\mario.wav"))
	{// Simple error message if load fails.
		std::cout << "Failed to load 'mario.wav'\n";
	}

	if (!m_snd_exclaimLuigi.loadFromFile("ASSETS\\SOUNDS\\luigi.wav"))
	{// Simple error message if load fails.
		std::cout << "Failed to load 'luigi.wav'\n";
	}

	m_soundSource_charName.setBuffer(m_snd_exclaimMario);
}

void Game::changeCharacter()
{
	m_ImMario = !m_ImMario; // Toggle whether you are or are not Mario.
	if (m_ImMario)
	{
		m_characterName.setString("Mario");
		m_characterName.setFillColor(sf::Color::Red);
		m_characterName.setOutlineColor(sf::Color::Green);
		m_marioSprite.setTextureRect(sf::IntRect{ 0,0,64,148 }); // Mario is to left of 128 width texture, so rect starts at x0.
		centerText();

		m_soundSource_charName.setBuffer(m_snd_exclaimMario);
		m_soundSource_charName.play();
	}
	else
	{
		m_characterName.setString("Luigi");
		m_characterName.setFillColor(sf::Color::Green);
		m_characterName.setOutlineColor(sf::Color::Red);
		m_marioSprite.setTextureRect(sf::IntRect{ 64,0,64,148 });// Luigi is to right of 128 width texture, so rect starts at x64 (hor middle).
		centerText();

		m_soundSource_charName.setBuffer(m_snd_exclaimLuigi);
		m_soundSource_charName.play();
	}
}

void Game::centerText()
{
	sf::Vector2f location{ 0.0f, 0.0f };
	location.y = 50.0f;
	location.x = 400 - (m_characterName.getGlobalBounds().width / 2);
	m_characterName.setPosition(location);
}
