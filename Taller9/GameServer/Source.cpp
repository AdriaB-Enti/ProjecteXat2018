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
bool checkMove(int x, int y);

int main()
{
	//serverPlayer provaPlayer = serverPlayer(sf::IpAddress("127.0.0.1"),50);
	
	//socket.setBlocking(false);
	socket.bind(PORTSERVER);

	while (true)
	{
		sf::IpAddress clientIp;
		unsigned short clientPort;
		sf::Packet pack;
		sf::UdpSocket::Status status = socket.receive(pack, clientIp, clientPort);

		switch (status)
		{
		case sf::Socket::Done:
		{

			std::cout << "recieved" << std::endl;
			sf::Uint8 comandoInt;
			pack >> comandoInt;
			Cabeceras comando = (Cabeceras)comandoInt;

			switch (comando)
			{
			case HELLO:
			{
				if (!isPlayerSaved(clientIp, clientPort)) {
					//enviar la posició aleatoria en el mapa

					sf::Vector2i position = totalPlayers == 0 ? sf::Vector2i(0, 0) : sf::Vector2i(N_TILES_WIDTH*2, N_TILES_HEIGHT*2);
					serverPlayer newPlayer = serverPlayer(clientIp, clientPort, (std::string)"", position, totalPlayers);
					players.push_back(newPlayer);
					//totalPlayers = 15;
					//sf::Int8 totalPlayersInt8 = (sf::Int8) totalPlayers;

					sf::Packet welcomePack;
					welcomePack << (sf::Uint8)Cabeceras::WELCOME;
					welcomePack << (sf::Uint8) totalPlayers;
					welcomePack << (sf::Uint32) position.x;
					welcomePack << (sf::Uint32) position.y;
					socket.send(welcomePack, clientIp, clientPort);
					totalPlayers++;
				}
				else
				{
					std::cout << "Client was already connected" << std::endl;
					//TODO
					//Buscar jugador amb el clientIp:clientPort
					//trobar la seva posicio
					//enviar, sense tocar el totalPlayers ni res
					sf::Packet welcomePack;
					welcomePack << (sf::Uint8)Cabeceras::WELCOME;


				}
				std::cout << "A client has connected" << std::endl;
				//TODO: send welcome
			}

				break;
			case WELCOME:
				break;
			case ACKNOWLEDGE:
				break;
			case NEW_PLAYER:
				break;
			case MOVE_LEFT:
			{
				std::cout << "player " << clientIp.toString() << " moving left\n";

				//hard coded al primer jugador. buscar el jugador pertinent
				//players.front().position
				//int newPosX = CHARACTER_SPEED
				//checkMove(newPosX,
				sf::Packet movementPacket;
				movementPacket << (sf::Uint8)Cabeceras::OK_POSITION;

			}

				break;
			case MOVE_RIGHT:
				std::cout << "player " << clientIp.toString() << " moving right\n";
				break;
			case MOVE_UP:
				std::cout << "player " << clientIp.toString() << " moving up\n";
				break;
			case MOVE_DOWN:
				std::cout << "player " << clientIp.toString() << " moving down\n";
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

//Checks if a position is inside the map
bool checkMove(int x, int y)
{
	if (x > 0 && x < TILESIZE*N_TILES_WIDTH && y > 0 && y < TILESIZE*N_TILES_HEIGHT) {
		return true;
	}

	return false;
}
