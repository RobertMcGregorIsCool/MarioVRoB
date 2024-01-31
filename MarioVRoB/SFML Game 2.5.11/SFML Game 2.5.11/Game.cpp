/// <summary>
/// @author RoBert McGregor LOGIN: c00302210
/// START:  19th Jan 2024	END: ???
/// EST:	4 hours?		ACTUAL:
/// KNOWN BUGS: 
///
/// you need to change the above lines or lose marks <= Roger, wilco.
/// </summary>

#include "Game.h"

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
	{ // I've changed a lot here, but am retaining Pete's original naming where possible. Hope that's the right approach!
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
	setuMovement(); // Monster patrols waypoints around campus
	eBallMovement();// Energy ball fires at player character and teleports back to monster
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	if (m_ImMario)
	{// Basics of Ikaruga effect.
		m_window.clear(sf::Color::White);
		m_window.draw(m_playfieldSprite);
	}

	//m_window.draw(m_welcomeMessage);	// Used comments to get rid of this. I WOULD use 'Debug', but this project is all Debug atm.
	// m_window.draw(m_logoSprite);

	// m_window.draw(m_rectangleShape); // This exists for number reference, but is not drawn.

	m_window.draw(m_characterName);		// Probably replacing this with a status indicator for player char.

	m_window.draw(m_marioSprite);

	m_window.draw(m_setuSprite);

	m_window.draw(m_eBallSprite);

	//m_window.draw(m_rectDebug); // Turn this off when I've found the right values.

	m_window.display();
}

void Game::move()
{
	sf::Vector2f movement{ 0.0f, 0.0f };
	float playerBot = m_location.y + (m_marioSprite.getLocalBounds().height * 0.35f); //0.85
	float playerTop = m_location.y + (m_marioSprite.getLocalBounds().height * 0.4f);  //0.4
	float playerLeft = m_location.x - (m_marioSprite.getLocalBounds().width * 0.75f);
	float playerRigt = m_location.x - (m_marioSprite.getLocalBounds().width * 0.25f);

	switch (m_direction)
	{
	case Direction::None:
		break;
	case Direction::Up:
		if (playerBot > 0.0f)
		{
			if (!collidingWithBounds(playerBot, playerTop, playerLeft, playerRigt))
			{
				m_lastValidPos = m_location;
				movement.y = -m_moveSpeed;
			}
			else
			{
				m_location = m_lastValidPos;
			}
		}
		break;
	case Direction::Down:
		if (playerTop < m_window.getSize().y)
		{
			if (!collidingWithBounds(playerBot, playerTop, playerLeft, playerRigt))
			{
				m_lastValidPos = m_location;
				movement.y = m_moveSpeed;
			}
			else
			{
				m_location = m_lastValidPos;
			}
		}
		break;
	case Direction::Left:
		if (playerLeft > 0.0f)
		{
			if (!collidingWithBounds(playerBot, playerTop, playerLeft, playerRigt))
			{
				m_lastValidPos = m_location;
				movement.x = -m_moveSpeed;
			}
			else
			{
				m_location = m_lastValidPos;
			}
		}
		break;
	case Direction::Right:
		if (playerRigt< m_window.getSize().x)
		{
			if (!collidingWithBounds(playerBot, playerTop, playerLeft, playerRigt))
			{
				m_lastValidPos = m_location;
				movement.x = m_moveSpeed;
			}
			else
			{
				m_location = m_lastValidPos;
			}
		}
		break;
	default:
		break;
	}
	m_location += movement;
	m_marioSprite.setPosition(m_location);
}

void Game::setuMovement()
{
	m_setuDirection = { 0.0f, 0.0f }; // This shouldn't be required?
	m_setuDirection = m_setuWaypoints[m_setuPosIndex] - m_setuSprite.getPosition();
	m_setuDirection = normalizeV2f(m_setuDirection);
	m_setuPosition = m_setuSprite.getPosition() + m_setuDirection;
	m_setuSprite.setPosition(m_setuPosition);

	if (vector2fSqrMag(m_setuWaypoints[m_setuPosIndex], m_setuSprite.getPosition()) < m_setuWaypointThreshold)
	{
		if (m_setuPosIndex == m_setuWaypoints.size() -1)
		{
			m_setuPosIndex = 0;
		}
		else
		{
			m_setuPosIndex++;
		}
	}
}

void Game::eBallMovement()
{
	if (!m_eBallTravelling || vector2fSqrMag(m_setuSprite.getPosition(), m_eBallSprite.getPosition()) > m_eBallResetDist)//distance is too great
	{
		m_eBallTravelling = true;
		m_eBallSprite.setPosition(m_setuSprite.getPosition()); // Reset on top of SETU Monster
		m_eBallDirection = m_marioSprite.getPosition() - m_eBallSprite.getPosition();
		m_eBallDirection = normalizeV2f(m_eBallDirection);
		m_eBallDirection *= m_eBallMoveSpeed;
	}
	else
	{
		m_eBallPosition = m_eBallSprite.getPosition() + m_eBallDirection;
		m_eBallSprite.setPosition(m_eBallPosition);
	}
}

sf::Vector2f Game::normalizeV2f(const sf::Vector2f source) // NOT MY CODE. https://en.sfml-dev.org/forums/index.php?topic=1488.0 (bottom of page)
{// However, SFML 3 apparently will have a built-in utillity function for normalising vectors, so...!
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float Game::vector2fSqrMag(const sf::Vector2f posA, const sf::Vector2f posB, const bool printDist)
{
	sf::Vector2f dist = posA - posB;

	float sqrMag = (dist.x * dist.x) + (dist.y * dist.y);

	if (printDist)
	{
		std::cout << "Distance is " << sqrMag << ".\n\n";
	}

	return sqrMag;
}

void Game::checkDirection()
{
	m_direction = Direction::None;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_direction = Direction::Up;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_direction = Direction::Down;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_direction = Direction::Left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
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
	m_characterName.setString("Active");
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

	if (!m_marioTexture.loadFromFile("ASSETS\\IMAGES\\ROB\\sculpture01001RS.png")) // mario-luigi-64
	{// Simple error message if previous call fails
		std::cout << "problem loading college sculpture" << std::endl;
	}

	if (!m_setuTexture.loadFromFile("ASSETS\\IMAGES\\ROB\\SETU_logo_monster01001RS.png"))
	{// Simple error message if loading SETU Monster fails.
		std::cout << "Problem loading the SETU Monster." << std::endl;
	}

	if (!m_eBallTexture.loadFromFile("ASSETS\\IMAGES\\ROB\\SETU_EnergyBall01001RS.png"))
	{// Simple error message if loading Energy Ball fails.
		std::cout << "Problem loading the EnergyBall texture";
	}

	if (!m_playfieldTexture.loadFromFile("ASSETS\\IMAGES\\ROB\\playfield01001RS.png"))
	{// Simple error message if loading playfield fails
		std::cout << "Problem loading the playfield texture\n";
	}

	m_playfieldSprite.setTexture(m_playfieldTexture);

	m_marioSprite.setTexture(m_marioTexture);
	m_marioSprite.setPosition(m_location);
	m_marioSprite.setTextureRect(sf::IntRect(0, 0, 64, 148));
	m_marioSprite.setOrigin(64.0f, 74.0f);// (64.0f, 74.0f);

	m_setuSprite.setTexture(m_setuTexture);
	m_setuSprite.setOrigin(64.0f, 64.0f);
	m_setuSprite.setPosition(m_setuWaypoints[2]); // instead, should be using m_setuposition

	m_eBallSprite.setTexture(m_eBallTexture);
	m_eBallSprite.setOrigin(16.0f, 16.0f);
	m_eBallSprite.setPosition(m_eBallPosition);

	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);		// So I need to change this stuff so the text follows the character around.

	m_rectangleShape.setSize(sf::Vector2f(256.0f, 256.0f));
	m_rectangleShape.setPosition((m_window.getSize().x * 0.5f) - m_rectangleShape.getSize().x * 0.5f, (m_window.getSize().y * 0.5f) - m_rectangleShape.getSize().y * 0.5f);
	m_rectangleShape.setFillColor(m_clearColor);	// Previously used sf::Color::Cyan . Currently using bounds of shape for collision.

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

	if (!m_music_angelAttack.openFromFile("ASSETS\\SOUNDS\\03 - ANGEL ATTACK.flac"))
	{// Simple error message if load fails.
		std::cout << "Failed to load 'angel attack' bgm\n";
	}
	m_music_angelAttack.setLoop(true);
	m_music_angelAttack.play();

	m_soundSource_charName.setBuffer(m_snd_exclaimMario);
}

void Game::changeCharacter()
{// Originally this changed Mario to Luigi and vice versa. Now it powers an Ikaruga-style mechanic.
	m_ImMario = !m_ImMario; // Toggle whether you are or are not Mario.
	if (m_ImMario)
	{
		m_characterName.setString("Active");
		m_characterName.setFillColor(sf::Color::Red);
		m_characterName.setOutlineColor(sf::Color::Green);
		m_marioSprite.setTextureRect(sf::IntRect{ 0,0,64,148 }); // Mario is to left of 128 width texture, so rect starts at x0.
		centerText();

		m_soundSource_charName.setBuffer(m_snd_exclaimMario);
		m_soundSource_charName.play();
	}
	else
	{
		m_characterName.setString("Ghost");
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

bool Game::collidingWithBounds(float bot, float top, float left, float rigt)
{
	float rectTop	= m_rectangleShape.getGlobalBounds().top;
	float rectBot	= rectTop + m_rectangleShape.getGlobalBounds().height;
	float rectLeft	= m_rectangleShape.getGlobalBounds().left;
	float rectRigt	= rectLeft + m_rectangleShape.getGlobalBounds().width;

	if ((bot < rectBot && bot > rectTop) && (left < rectRigt && rigt > rectLeft))
	{
		return true;
	}
	else
	{
		return false;
	}
}
