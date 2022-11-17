#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250)/*, _cMunchieFrameTime(500)*/
{
	_frameCount = 0;
	_paused = false;
	_escKeyDown = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	/*_munchieCurrentFrameTime = 0;*/

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
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

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

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

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{

}

void Pacman::CheckViewportCollision()
{

}

void updatePacman(int elapsedTime)
{

}

void updateMunchie(int elapsedTime)
{

}

void Pacman::Update(int elapsedTime)

{   //Motion for pacman
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	if (!_paused)
	{
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

		
	}
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown)
	{
		_escKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	//if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
	//	_escKeyDown = false;

	/*_munchieCurrentFrameTime += elapsedTime;*/

	/*if (_munchieCurrentFrameTime > _cMunchieFrameTime)
	{
		_frameCount++;

		if (_frameCount >= 2)
			_frameCount = 0;

		_munchieCurrentFrameTime = 0;
	}*/

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
	
	
	
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _PacmanPosition->X << " Y: " << _PacmanPosition->Y;

	SpriteBatch::Draw(_PacmanTexture, _PacmanPosition, _PacmanSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		//_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		//_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); 
}