#pragma once
#include <SFML\Graphics.hpp>

#define IPSERVER "127.0.0.1"
#define PORTSERVER 50000
#define MAXPLAYERS 2

enum Cabeceras
{
	HELLO,
	WELCOME,
	ACKNOWLEDGE,
	NEW_PLAYER
};

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();
	std::string name;
	sf::Vector2i position;
	int lives;
};