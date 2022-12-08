#include "Pacman.h"

#include <sstream>
#include <ctime>
#include <cstdlib>
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	_frameCount = 0;
	_paused = false;
	_escKeyDown = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update 6and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _PacmanTexture;
	delete _PacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_PacmanTexture = new Texture2D();
	_PacmanTexture->Load("Textures/Pacman.tga", false);
	_PacmanPosition = new Vector2(350.0f, 350.0f);
	_PacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Clyde
	_clydeTexture = new Texture2D();
	_clydeTexture->Load("Textures/Clyde.png", false);
	_clydePosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 32, Graphics::GetViewportHeight() / 2.0f - 32);
	_clydeSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	/*_munchieRect = new Rect(100.0f, 450.0f, 12, 12);*/

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	if (!_paused)
	{
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_PacmanPosition->X += _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 0;
			_pacmanCurrentFrameTime += elapsedTime;
			_PacmanSourceRect->X = _PacmanSourceRect->Width * _pacmanFrame;
			_PacmanSourceRect->Y = _PacmanSourceRect->Height * _pacmanDirection;
		}
		if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_PacmanPosition->X -= _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 2;
			_pacmanCurrentFrameTime += elapsedTime;
			_PacmanSourceRect->X = _PacmanSourceRect->Width * _pacmanFrame;
			_PacmanSourceRect->Y = _PacmanSourceRect->Height * _pacmanDirection;
		}
		if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_PacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 3;
			_pacmanCurrentFrameTime += elapsedTime;
			_PacmanSourceRect->X = _PacmanSourceRect->Width * _pacmanFrame;
			_PacmanSourceRect->Y = _PacmanSourceRect->Height * _pacmanDirection;
		}
		if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_PacmanPosition->Y += _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 1;
			_pacmanCurrentFrameTime += elapsedTime;
			_PacmanSourceRect->X = _PacmanSourceRect->Width * _pacmanFrame;
			_PacmanSourceRect->Y = _PacmanSourceRect->Height * _pacmanDirection;
		}
		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;

			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;

			_pacmanCurrentFrameTime = 0;
		}
	}
}


void Pacman::Update(int elapsedTime)

{   //Motion for pacman
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	if (!_paused)
	{
		int randNum = rand();
		int randRemainder = randNum % 2;
		Input(elapsedTime, keyboardState);

		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;

			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;

			_pacmanCurrentFrameTime = 0;
		}
		// Allow pacman to go over the edge of the screen and zap to the other side
		if (_PacmanPosition->X < -32)
			_PacmanPosition->X = Graphics::GetViewportWidth();
		if (_PacmanPosition->X > Graphics::GetViewportWidth())
			_PacmanPosition->X = -32;
		if (_PacmanPosition->Y > Graphics::GetViewportHeight())
			_PacmanPosition->Y = -32;
		if (_PacmanPosition->Y < -32)
			_PacmanPosition->Y = Graphics::GetViewportHeight();

		// Set Menu Parameters
		_menuBackground = new Texture2D();
		_menuBackground->Load("Textures/Transparency.png", false);
		_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
		_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

		if (_frameCount == 60)
		{
			if (randRemainder == 1)
			{


				SpawnProjectile(Projectile::burst);
				tempObject->angle++;
				SpawnProjectile(Projectile::burst);
				tempObject->angle--;
				SpawnProjectile(Projectile::burst);
			}
			else
				SpawnProjectile(Projectile::straight);

		}
	}


	/*if (_frameCount == 60)
	{
		for (int i = 0; i < 5; ++i)
		{

			SpawnProjectile(Projectile::burst);
			tempObject->angle++;
			SpawnProjectile(Projectile::burst);
			tempObject->angle--;
			SpawnProjectile(Projectile::burst);

		}
	}*/
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown)
	{
		_escKeyDown = true;
		_paused = !_paused;

		/*for (int i = 0; i < 5; ++i)
		{

				SpawnProjectile(Projectile::burst);
				tempObject->angle++;
				SpawnProjectile(Projectile::burst);
				tempObject->angle--;
				SpawnProjectile(Projectile::burst);		
				
		}*/
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	_munchieCurrentFrameTime += elapsedTime;



	for (int i = 0; i < Projectiles.size(); i++)
	{
		UpdateProjectile(Projectiles[i]);
	}
	_frameCount++;
	if (_frameCount == 61)
		_frameCount = 0;

}

void Pacman::SpawnProjectile(Projectile::projectileType type)
{
	tempObject = new Projectile();
	Projectiles.push_back(tempObject);

	tempObject->thisProjectileType = type;
	tempObject->speed = 4.0f; 
	tempObject->_projectilePosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 32, Graphics::GetViewportHeight() / 2.0f - 32);
	tempObject->_targetPosition = new Vector2(_PacmanPosition->X+16, _PacmanPosition->Y+16);
	tempObject->_projectileSourceRect = _munchieRect;
	tempObject->_projectileTexture = _munchieInvertedTexture; 
	int diffX = tempObject->_targetPosition->X - tempObject->_projectilePosition->X;
	int diffY = tempObject->_targetPosition->Y - tempObject->_projectilePosition->Y;

	tempObject->angle = (float)atan2(diffY, diffX);

}
void Pacman::UpdateProjectile(Projectile* projectileUpdating)
{

	if (projectileUpdating->thisProjectileType == projectileUpdating->straight)
	{
		projectileUpdating->_projectilePosition->X += projectileUpdating->speed * cos(projectileUpdating->angle);
		projectileUpdating->_projectilePosition->Y += projectileUpdating->speed * sin(projectileUpdating->angle);
	}
	else if (projectileUpdating->thisProjectileType == projectileUpdating->predictive)
	{
		
	}
	else if (projectileUpdating->thisProjectileType == projectileUpdating->burst)
	{
		
		projectileUpdating->_projectilePosition->X += projectileUpdating->speed * cos(projectileUpdating->angle);
		projectileUpdating->_projectilePosition->Y += projectileUpdating->speed * sin(projectileUpdating->angle);
	}
}

void Pacman::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	
	for (int i = 0; i < Projectiles.size(); i++)
	{
		SpriteBatch::Draw(Projectiles[i]->_projectileTexture, Projectiles[i]->_projectilePosition, Projectiles[i]->_projectileSourceRect);
	}
	
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _PacmanPosition->X << " Y: " << _PacmanPosition->Y;

	SpriteBatch::Draw(_PacmanTexture, _PacmanPosition, _PacmanSourceRect); // Draws Pacman

	//if (_frameCount < 10)
	//{
	//	// Draws Red Munchie
	//	SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

	//	_frameCount++;
	//}
	//else
	//{
	//	// Draw Blue Munchie
	//	SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	//	
	//	_frameCount++;

	//	if (_frameCount >= 60)
	//		_frameCount = 0;
	//}
	
	SpriteBatch::Draw(_clydeTexture, _clydePosition, _clydeSourceRect);

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); 
}