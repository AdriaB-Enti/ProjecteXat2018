#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <iostream>

//Global vars
unsigned short myID;


int main()
{
	PlayerInfo playerInfo;

	sf::UdpSocket socket;
	socket.setBlocking(false);


	sf::Packet helloPack;
	helloPack << Cabeceras::HELLO;
	socket.send(helloPack, IPSERVER, PORTSERVER);

	sf::Packet serverPack;
	sf::IpAddress ipServer;
	unsigned short portServer;
	//recibir el welcome del servidor

	bool confirmationRecieved = false;
	while (!confirmationRecieved)
	{
		sf::UdpSocket::Status status = socket.receive(serverPack,ipServer, portServer);

		switch (status)
		{
		case sf::Socket::Done:
			std::cout << "S'ha rebut el packet del servidor\n";
			confirmationRecieved = true;

			Cabeceras cab;
			unsigned short cab_short;
			/*serverPack >> cab_short;
			cab = (Cabeceras)cab_short;
			if (cab == Cabeceras::WELCOME)
				std::cout << "La cabecera era correcta" << cab << std::endl;
			else
				std::cout << "La cabecera NO era correcta" << cab << std::endl;
			sf::Int8 idRecibida;
			serverPack >> idRecibida;
			myID = (unsigned short)idRecibida;*/

			sf::Uint8 cab8, id8;
			serverPack >> cab8;
			serverPack >> id8;

			if ((Cabeceras)cab8 == Cabeceras::WELCOME)
			{
				myID = (unsigned short)id8;
			}

			std::cout << "Benvingut, jugador amb ID=" << myID<< std::endl;

			break;
		case sf::Socket::NotReady:
			break;
		case sf::Socket::Partial:
			std::cout << "Partial package\n";
			break;
		case sf::Socket::Disconnected:
			break;
		case sf::Socket::Error:
			break;
		default:
			break;
		}
		//fer un sleep per si de cas?
		//reenviar el packet
	}

	int n;
	std::cin >> n;


	return 0;
}