#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <Constants.h>

class serverPlayer :PlayerInfo {
public:
	serverPlayer( sf::IpAddress newIp, unsigned short newPort, std::string newName, sf::Vector2i newPosition, short newID);
	sf::IpAddress ip;
	unsigned short port;
	short id;
};

serverPlayer::serverPlayer(sf::IpAddress newIp, unsigned short newPort, std::string newName, sf::Vector2i newPosition, short newID) {
	ip = newIp;
	port = newPort;
	name = newName;
	position = newPosition;
	id = newID;
}

//Global vars
std::vector<serverPlayer> players;
unsigned short totalPlayers = 0;
sf::UdpSocket socket;

//Fw declarations
bool isPlayerSaved(sf::IpAddress ip, unsigned short port);

int main()
{
	//serverPlayer provaPlayer = serverPlayer(sf::IpAddress("127.0.0.1"),50);
	
	//socket.setBlocking(false);
	socket.bind(PORTSERVER);

	while (true)
	{
		sf::IpAddress ip;
		unsigned short port;
		sf::Packet pack;
		sf::UdpSocket::Status status = socket.receive(pack, ip, port);

		switch (status)
		{
		case sf::Socket::Done:
		{

			std::cout << "recieved" << std::endl;
			int comandoInt;
			pack >> comandoInt;
			Cabeceras comando = (Cabeceras)comandoInt;

			switch (comando)
			{
			case HELLO:
				if (!isPlayerSaved(ip, port)) {
					//enviar la posició aleatoria en el mapa

					sf::Vector2i position = totalPlayers == 0 ? sf::Vector2i(0, 0) : sf::Vector2i(N_TILES_WIDTH*2, N_TILES_HEIGHT*2);
					serverPlayer newPlayer = serverPlayer(ip, port, (std::string)"", position, totalPlayers);
					players.push_back(newPlayer);
					//totalPlayers = 15;
					//sf::Int8 totalPlayersInt8 = (sf::Int8) totalPlayers;

					sf::Packet welcomePack;
					welcomePack << (sf::Uint8)Cabeceras::WELCOME;
					welcomePack << (sf::Uint8) totalPlayers;
					welcomePack << (sf::Uint32) position.x;
					welcomePack << (sf::Uint32) position.y;
					socket.send(welcomePack, ip, port);
					totalPlayers++;
				}
				else
				{
					std::cout << "Client was already connected" << std::endl;
					//TODO
					//Buscar jugador amb el ip:port
					//trobar la seva posicio
					//enviar, sense tocar el totalPlayers ni res
					sf::Packet welcomePack;
					welcomePack << (sf::Uint8)Cabeceras::WELCOME;


				}
				std::cout << "A client has connected" << std::endl;

				//TODO: send welcome


				break;
			case WELCOME:
				break;
			case ACKNOWLEDGE:
				break;
			case NEW_PLAYER:
				break;
			default:
				break;
			}



			break;
		}

		case sf::Socket::NotReady:
			break;
		case sf::Socket::Partial:
			break;
		case sf::Socket::Disconnected:
			break;
		case sf::Socket::Error:
			break;
		default:
			break;
		}

	}





	return 0;
}


//TODO: canviar el players per un map
bool isPlayerSaved(sf::IpAddress ip, unsigned short port) {
	for (int p = 0; p < players.size(); p++)
	{
		if (players.at(p).ip == ip && players.at(p).port == port)
		{
			return true;
		}
	}
	return false;
}
