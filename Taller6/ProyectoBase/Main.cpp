#include <SFML\Network.hpp>

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

//TODO: posar en el game lib
#define PORTSERVER 50000

enum Cabeceras
{
	HELLO,
	WELCOME,
	ACKNOWLEDGE,
	NEW_PLAYER
};

struct Player
{
	std::string name;
	sf::IpAddress ip;
	unsigned short port;
	unsigned short id;
};


std::vector<Player> players;
int totalPlayers = 0;

//Fordward declaration
bool isPlayerSaved(sf::IpAddress ip, unsigned short port);


int main()
{
	//Servidor
	sf::UdpSocket socket;
	//socket.setBlocking(false);
	socket.bind(PORTSERVER);
	std::vector<std::string> aMensajes;
	

	while (true)	//Won't stop
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
			Cabeceras comando = (Cabeceras) comandoInt;

			switch (comando)
			{
			case HELLO:
				if (!isPlayerSaved(ip, port)) {
					Player newPlayer;
					newPlayer.ip = ip;
					newPlayer.port = port;
					newPlayer.name = "";
					newPlayer.id = totalPlayers;
					players.push_back(newPlayer);
					totalPlayers++;
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
	
	system("pause");
	
	return 0;

}

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
