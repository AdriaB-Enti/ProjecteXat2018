#pragma once
#include <SFML\Graphics.hpp>



class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();
	std::string name;
	sf::Vector2i position;
	int lives;
};