#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include <vector>
// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Projectile {
	enum projectileType{straight, burst, predictive};
	projectileType thisProjectileType; 
	Vector2* _projectilePosition;
	Rect* _projectileSourceRect;
	Texture2D* _projectileTexture;
	Vector2* _targetPosition;
	float angle;

	float speed; 
	/*int _pacmanDirection;*/
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	//Cpoordinate system 2

	//const int middleX = 1024/2;
	//const int middleY = 768 / 2;

	// Data to represent Pacman
	Vector2* _PacmanPosition;
	Rect* _PacmanSourceRect;
	Texture2D* _PacmanTexture;
	int _pacmanDirection;

	vector<Projectile*>Projectiles;

	// Data to represent Clyde
	Vector2* _clydePosition;
	Rect* _clydeSourceRect;
	Texture2D* _clydeTexture;

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;
	//const int _cMunchieFrameTime;
	//int _munchieFrame;
	//int _munchieCurrentFrameTime;

	// Position for String
	Vector2* _stringPosition;

	float _cPacmanSpeed;

	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _escKeyDown;
	int _pacmanFrame, _pacmanCurrentFrameTime;
	const int _cPacmanFrameTime;

	int _munchieFrame, _munchieCurrentFrameTime;
	const int _cMunchieFrameTime;

	void Input(int elapsedTime, Input::KeyboardState* state);

	void SpawnProjectile(Projectile::projectileType type);
	void UpdateProjectile(Projectile* projectileUpdating);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};