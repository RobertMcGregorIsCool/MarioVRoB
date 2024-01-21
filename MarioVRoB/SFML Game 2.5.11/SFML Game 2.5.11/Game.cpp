/// <summary>
/// @author RoBert McGregor
/// @date Jan 2024
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
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
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
	m_window.draw(m_welcomeMessage);
	m_window.draw(m_logoSprite);

	m_window.draw(m_marioSprite);

	m_window.display();
}

void Game::move()
{
	sf::Vector2f movement{ 0.0f, 0.0f };
	switch (m_direction)
	{
	case Direction::None:
		break;
	case Direction::Up:
		movement.y = -m_moveSpeed;
		break;
	case Direction::Down:
		movement.y = m_moveSpeed;
		break;
	case Direction::Left:
		movement.x = -m_moveSpeed;
		break;
	case Direction::Right:
		movement.x = m_moveSpeed;
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
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

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
		std::cout << "problem loading logo" << std::endl;
	}

	m_marioSprite.setTexture(m_marioTexture);
	m_marioSprite.setPosition(m_location);
	m_marioSprite.setTextureRect(sf::IntRect(0, 0, 64, 148));

	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}
