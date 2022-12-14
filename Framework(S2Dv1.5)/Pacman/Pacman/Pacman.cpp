#include "Pacman.h"

#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	tempObject = new Projectile();
	_frameCount = 0;
	_paused = false;
	_escKeyDown = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;
	pew = new SoundEffect();
	legallyDistinctWakaWaka = new SoundEffect();
	triplePew = new SoundEffect();
	debug = new SoundEffect();
	damageOne = new SoundEffect();
	damageTwo = new SoundEffect();
	damageThree = new SoundEffect();

	//Initialise important Game aspects
	Audio::Initialise();
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
	delete pew, legallyDistinctWakaWaka, triplePew, debug;
	delete _clydeTexture, _clydeSourceRect;
	delete heartRect, heartTexture;
	delete damageOne, damageTwo, damageThree;
}

bool Pacman::CheckCollisions(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;
	if (bottom1 < top2)
		return false;
	else if (top1 > bottom2)
		return false;
	else if (right1 < left2)
		return false;
	else if (left1 > right2)
		return false;

	return true;
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

	//Load heart
	heartTexture = new Texture2D();
	heartTexture->Load("Textures/hearts.png", false);
	heartRect = new Rect(0.0f, 0.0f, 96, 32);
	heartPosition = new Vector2(932, 0);
	

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Sound
	pew->Load("Sounds/pew.wav");
	legallyDistinctWakaWaka->Load("Sounds/legallyDistinctWakaWaka.wakawaka.wav");
	triplePew->Load("Sounds/triplePew.wav");
	debug->Load("Sounds/debug.wav");
	//Damage sound effects
	damageOne->Load("Sounds/damageOne.wav");
	damageTwo->Load("Sounds/damageTwo.wav");
	damageThree->Load("Sounds/damageThree.wav");

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
	else if (projectileUpdating->thisProjectileType == projectileUpdating->burst)
	{
		projectileUpdating->_projectilePosition->X += projectileUpdating->speed * cos(projectileUpdating->angle);
		projectileUpdating->_projectilePosition->Y += projectileUpdating->speed * sin(projectileUpdating->angle);
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
			
			Audio::Play(legallyDistinctWakaWaka);
			if (randRemainder == 1)
			{
				Audio::Play(triplePew);
				SpawnProjectile(Projectile::burst);
				tempObject->angle++;
				SpawnProjectile(Projectile::burst);
				tempObject->angle--;
				SpawnProjectile(Projectile::burst);
			}
			else
				Audio::Play(pew);
				SpawnProjectile(Projectile::straight);

		}
	}
	for (int j = 0; j < size(Projectiles); ++j)
	{
		if (CheckCollisions(_PacmanPosition->X, _PacmanPosition->Y, _PacmanSourceRect->Width, _PacmanSourceRect->Height, Projectiles[j]->_projectilePosition->X, Projectiles[j]->_projectilePosition->Y, 12, 12))
		{
			//Basic damage taking collision
			Projectiles[j]->_projectilePosition->X = 100000;
			pacmanHP -= 1;
			heartPosition->X += 32;
			Audio::Play(damageOne);

			if(pacmanHP == 2)
				Audio::Play(damageOne);
			else if(pacmanHP == 1)
				Audio::Play(damageTwo);
			else if(pacmanHP == 0)
				Audio::Play(damageThree);
	/*		switch (pacmanHP) {
			case 2:
				Audio::Play(damageOne);

				break;
			case 1:
				Audio::Play(damageTwo);

				break;
			case 0:
				Audio::Play(damageThree);

				break;
			}*/
		}

			
	}
	

	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown)
	{
		_escKeyDown = true;
		_paused = !_paused;

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

	
	SpriteBatch::Draw(_clydeTexture, _clydePosition, _clydeSourceRect);


	///For whatever reason, these two methods of drawing three hearts via loops doesn't work. I'm sure there it is fixable, but time is too limited and instead I opt for just extending the sprite and moving it.

	/*for (k = 0; k < 64; k =+ 32);
	{
		SpriteBatch::Draw(heartTexture, heartPosition, heartRect);
	}*/

	/*while (k > 932)
	{
		SpriteBatch::Draw(heartTexture, heartPosition, heartRect);
		k = k - 32;
	}*/

	SpriteBatch::Draw(heartTexture, heartPosition, heartRect);


	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); 
}