/// <summary>
/// @author RoBert McGregor LOGIN: c00302210
/// START:  19th Jan 2024	END: 01st Feb 2024
/// EST:	4 hours?		ACTUAL: Too long, but I had fun with it.
/// KNOWN BUGS: Program exits on player win, no time to make it better. 
///	If the player just books it for the exit, they'll win.
/// No player anticipation.
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
	m_exitGame{false}	//when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite();		// load texture
	setupSounds();		// Load sounds, prep speaker.
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
	{ // Changed a lot here, retaining Pete's orig naming where possible. Hope that's right approach!
		changeCharacter();
	}
	if (sf::Keyboard::Enter == t_event.key.code)
	{// Test code to reset the game.
		reset();
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
	eBallCollision();
	setuCollision();
	safeCollision();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	if (m_ImMario)
	{// If player switches to 'Ghost' mode, turn off background draw in order to get a trails effect
		m_window.clear(sf::Color::White);
		m_window.draw(m_playfieldSprite);
	}

	//m_window.draw(m_welcomeMessage);	// Commented out - WOULD use 'Debug', but ALL Debug atm.
	//m_window.draw(m_logoSprite);

	m_window.draw(m_characterName);		// Replaced w/stat indic for player (ie. 'Mobile' 'Ghost')

	m_window.draw(m_marioSprite);

	m_window.draw(m_setuSprite);

	m_window.draw(m_eBallSprite);

	m_window.display();
}

/// <summary>
/// Check player touching a wall/obj - if no, move player. If so, do not move or move back a bit
/// </summary>
void Game::move()
{
	sf::Vector2f movement{ 0.0f, 0.0f };
	float playerBot		= m_location.y + (m_marioSprite.getLocalBounds().height * m_marioBot); // Get player edge ref for wall collisions
	float playerTop		= m_location.y + (m_marioSprite.getLocalBounds().height * m_marioTop);
	float playerLeft	= m_location.x - (m_marioSprite.getLocalBounds().width * m_marioLft);
	float playerRigt	= m_location.x - (m_marioSprite.getLocalBounds().width * m_marioRgt);

	switch (m_direction)
	{
	case Direction::None:
		break;
	case Direction::Up:
		if (playerBot > 0.0f)
		{// If the bottom of the player is not touching the top of the screen...
			if (!collidingWithBounds(playerBot, playerTop, playerLeft, playerRigt))
			{
				m_lastValidPos = m_location;
				movement.y = -m_moveSpeed; // Setup target movement
			}
			else
			{// Player touching a wall? Teleport back to prev pos (before bad wall touch)
				m_location = m_lastValidPos;
			}
		}
		break;
	case Direction::Down:
		if (playerTop < m_window.getSize().y)
		{// If the top of the player is not lower than the bottom of the screen...
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

	if (m_ImMario)
	{// If player is 'Mobile' (m_ImMario == true), use movement values to set MarioSprite pos.
		m_location += movement;
		m_marioSprite.setPosition(m_location);
	}
}

/// <summary>
/// From update, moves SETU logo to waypoints, increasing waypoint index when close to current targ
/// </summary>
void Game::setuMovement()
{
	m_setuDirection	= m_setuWaypoints[m_setuPosIndex] - m_setuSprite.getPosition();
	m_setuDirection = normalizeV2f(m_setuDirection);
	m_setuPosition	= m_setuSprite.getPosition() + m_setuDirection * M_SETU_MOVE_SPEED;
	m_setuSprite.setPosition(m_setuPosition);

	if (vector2fSqrMag(m_setuWaypoints[m_setuPosIndex], m_setuSprite.getPosition()) < M_SETU_WAYPOINT_THRESHOLD)
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

/// <summary>
/// From update, moves energyball towards player, teleports back if too far from SETU monster
/// </summary>
void Game::eBallMovement()
{
	if (!m_eBallTravelling || vector2fSqrMag(m_setuSprite.getPosition(), m_eBallSprite.getPosition()) > M_E_BALL_RESET_DIST)
	{// distance is too great
		m_eBallTravelling = true;
		m_eBallSprite.setPosition(m_setuSprite.getPosition()); // Reset on top of SETU Monster
		m_eBallDirection = m_marioSprite.getPosition() - m_eBallSprite.getPosition();
		m_eBallDirection = normalizeV2f(m_eBallDirection);
		m_eBallDirection *= M_E_BALL_MOVE_SPEED;
		m_eBallFiringSoundSource.play();
	}
	else
	{
		m_eBallPosition = m_eBallSprite.getPosition() + m_eBallDirection;
		m_eBallSprite.setPosition(m_eBallPosition);
	}
}

/// <summary>
/// If energyball within threshold distance of Mario in mobile mode, call reset
/// </summary>
void Game::eBallCollision()
{
	if (vector2fSqrMag(m_marioSprite.getPosition(), m_eBallSprite.getPosition()) < M_E_BALL_HIT_THRESH)
	{
		if(m_ImMario)reset();
	}
}

/// <summary>
/// If SETU logo is within threshold distance of Mario, call reset
/// </summary>
void Game::setuCollision()
{
	if (vector2fSqrMag(m_marioSprite.getPosition(), m_setuSprite.getPosition()) < M_SETU_HIT_THRESH)
	{
		reset();
	}
}

/// <summary>
/// If Mario is close enough to safe zone, end the game. Would be nice to celebrate, but time is low
/// </summary>
void Game::safeCollision()
{
	if (vector2fSqrMag(m_marioSprite.getPosition(), sf::Vector2f(800.0f, 600.0f)) < M_SETU_HIT_THRESH)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Turns bad vector2 numbers into nice normalised numbers
/// </summary>
/// <param name="source">Scaled vector 2d</param>
/// <returns>Nice normal'd vector2f</returns>
sf::Vector2f Game::normalizeV2f(const sf::Vector2f t_source)
{// SFML 3 apparently will have a built-in utillity function for normalising vectors, so...!
	float length = sqrt((t_source.x * t_source.x) + (t_source.y * t_source.y));
	if (length != 0)
		return sf::Vector2f(t_source.x / length, t_source.y / length);
	else
		return t_source;
}

/// <summary>
/// Get the 'distance' between two points (faster than also doing sqrt)
/// </summary>
/// <param name="posA">Starting point</param>
/// <param name="posB">Ending point</param>
/// <param name="printDist">Option to read off the right distances from console</param>
/// <returns>Squared distance between two Vector2f points</returns>
float Game::vector2fSqrMag(const sf::Vector2f t_posA, const sf::Vector2f t_posB, const bool t_printDist)
{
	sf::Vector2f dist = t_posA - t_posB;

	float sqrMag = (dist.x * dist.x) + (dist.y * dist.y);

	if (t_printDist)
	{
		std::cout << "Distance is " << sqrMag << ".\n\n";
	}
	return sqrMag;
}

/// <summary>
/// Check for/get keyboard input from arrow keys and WASD
/// </summary>
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
	m_characterName.setString("Mobile"); // Hopefully this explains why you can only move as 'mario'
	m_characterName.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_characterName.setPosition(40.0f, 40.0f);
	m_characterName.setCharacterSize(80U);
	m_characterName.setOutlineColor(sf::Color::Red);
	m_characterName.setFillColor(sf::Color::Black);
	m_characterName.setOutlineThickness(3.0f);
}

/// <summary>
/// load the texture and setup the sprite for the logo, characters, background...
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
	m_setuSprite.setPosition(m_setuWaypoints[1]); // instead, should be using m_setuposition

	m_eBallSprite.setTexture(m_eBallTexture);
	m_eBallSprite.setOrigin(16.0f, 16.0f);
	m_eBallSprite.setPosition(m_eBallPosition);

	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);// Need to change stuff so text follows the char around.

	m_rectangleShape.setSize(sf::Vector2f(256.0f, 256.0f));
	m_rectangleShape.setPosition((m_window.getSize().x * 0.5f) - m_rectangleShape.getSize().x * 0.5f, (m_window.getSize().y * 0.5f) - m_rectangleShape.getSize().y * 0.5f);
	m_rectangleShape.setFillColor(m_clearColor);	// Currently using bounds of shape for collision.
}

/// <summary>
/// Load sounds and music for game
/// </summary>
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

	if (!m_resetSound.loadFromFile("ASSETS\\SOUNDS\\bicycle-horn-1.wav"))
	{
		std::cout << "Failed to load 'bicycle-horn-1.wav'\n";
	}

	if (!m_eBallFiringSnd.loadFromFile("ASSETS\\SOUNDS\\Laser 1.wav"))
	{
		std::cout << "Failed to load 'Laser 1.wav'\n";
	}

	if (!m_music_angelAttack.openFromFile("ASSETS\\SOUNDS\\03 - ANGEL ATTACK.flac"))
	{// Simple error message if load fails.
		std::cout << "Failed to load 'angel attack' bgm\n";
	}
	m_music_angelAttack.setVolume(25);
	m_music_angelAttack.setLoop(true);
	m_music_angelAttack.play();

	m_soundSource_charName.setBuffer(m_snd_exclaimMario);

	m_eBallFiringSoundSource.setBuffer(m_eBallFiringSnd);
}

/// <summary>
/// Originally this changed Mario to Luigi and vice versa. Now it powers an Ikaruga-style mechanic.
/// </summary>
void Game::changeCharacter()
{
	m_ImMario = !m_ImMario; // Toggle whether you are or are not Mario.
	if (m_ImMario)
	{
		m_characterName.setString("Mobile");
		m_characterName.setFillColor(sf::Color::Red);
		m_characterName.setOutlineColor(sf::Color::Green);
		m_marioSprite.setTextureRect(sf::IntRect{ 0,0,64,148 }); // Mario left of 128w text, rect starts@ x0.
		centerText();

		m_soundSource_charName.setBuffer(m_snd_exclaimMario);
		m_soundSource_charName.play();
	}
	else
	{
		m_characterName.setString("Ghost");
		m_characterName.setFillColor(sf::Color::Green);
		m_characterName.setOutlineColor(sf::Color::Red);
		m_marioSprite.setTextureRect(sf::IntRect{ 64,0,64,148 });// Luigi right of 128w text, rect starts@ x64 (hor mid).
		centerText();

		m_soundSource_charName.setBuffer(m_snd_exclaimLuigi);
		m_soundSource_charName.play();
	}
}

/// <summary>
/// Centers the status text. Ideally we'd call this at the start, but we're doing this later to demo to Pete that text is movable
/// </summary>
void Game::centerText()
{// In ideal world, I'd have time to make this more generic
	sf::Vector2f location{ 0.0f, 0.0f };
	location.y = 50.0f;
	location.x = 400 - (m_characterName.getGlobalBounds().width / 2);
	m_characterName.setPosition(location);
}

/// <summary>
/// My ugly home-grown collision detection code
/// </summary>
/// <param name="bot">Position of player bottom</param>
/// <param name="top">Position of player top</param>
/// <param name="left">Position of player left</param>
/// <param name="rigt">Position of player right</param>
/// <returns>True of colliding, false if not colliding</returns>
bool Game::collidingWithBounds(float t_bot, float t_top, float t_left, float t_rigt)
{
	float rectTop	= m_rectangleShape.getGlobalBounds().top;
	float rectBot	= rectTop + m_rectangleShape.getGlobalBounds().height;
	float rectLeft	= m_rectangleShape.getGlobalBounds().left;
	float rectRigt	= rectLeft + m_rectangleShape.getGlobalBounds().width;

	if ((t_bot < rectBot && t_bot > rectTop) && (t_left < rectRigt && t_rigt > rectLeft))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Resets the game to default vars - used if player 'dies'.
/// </summary>
void Game::reset()
{
	m_music_angelAttack.play();

	m_soundSourceReset.setBuffer(m_resetSound);
	m_soundSourceReset.play();

	m_setuPosIndex = 2;
	m_setuSprite.setPosition(m_setuWaypoints[1]);

	m_eBallTravelling = false;

	m_location		= M_MARIO_STARTPOS;		// Mario start pos for reset
	m_marioSprite.setPosition(m_location);
}