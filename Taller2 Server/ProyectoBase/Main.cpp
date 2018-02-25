#include <SFML\Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#define MAXPLAYERS 3


int main()
{
	//TODO: REFRACTOR del codi (deixar d'usar el aMensajes, que s'usava en el client)
	//Servidor Cliente
	std::vector<sf::TcpSocket*> sockets;
	
	std::string textoAEnviar="";	//em sembla que no el faig servir
	std::vector<std::string> aMensajes;
	sf::TcpListener listener;

	listener.listen(50000);
	for (int playerN = 0; playerN < MAXPLAYERS; playerN++)
	{
		sf::TcpSocket* newSock = new sf::TcpSocket();
		listener.accept(*newSock);
		newSock->setBlocking(false);
		sockets.push_back(newSock);
		textoAEnviar = "Mensaje desde servidor\n";

		std::string texto = "Conexion con ... " + (newSock->getRemoteAddress()).toString() + ":" + std::to_string(newSock->getRemotePort()) + "\n";
		std::cout << texto;

		for (std::vector<sf::TcpSocket*>::iterator it = sockets.begin(); it != sockets.end(); ++it)			//Enviamos un mensaje a los demás clientes cuando alguien se conecta
		{
			std::string newClient = "Se ha conectado un nuevo usuario. Bienvenido!";
			size_t confirmedSend;
			sf::Socket::Status st;
			do
			{
				st = (*it)->send(newClient.c_str(), newClient.length(), confirmedSend);
				if (st == sf::Socket::Status::Partial)
				{
					newClient = newClient.substr(confirmedSend + 1, newClient.length());
				}
			} while (st == sf::Socket::Status::Partial);
		}
	}
	listener.close();

	std::cout << "All users connected. Chat is now online!\n";


	char buffer[100];
	size_t bytesReceived;

	while (true)																							//el server estarà sempre obert
	{
		for (int playerN = 0; playerN < MAXPLAYERS; playerN++)												//fem un recieve per cada client -- potser dona problemes si un es desconecta (estarem mirant tots els sockets)
		{
			//Receive-------------------------------
			
			sf::TcpSocket::Status result = sockets.at(playerN)->receive(buffer, 100, bytesReceived);
			buffer[bytesReceived] = '\0';
			if (result == sf::TcpSocket::Status::Done)
			{
				aMensajes.push_back(std::string(buffer)); //segurament s'ha de passar el buffer a string

				for (int playerSend = 0; playerSend < MAXPLAYERS; playerSend++)								//per cada recieve, fem un send a cada client
				{
					//Send-------------------
					std::string lastMessage = aMensajes.back();
					size_t confirmedSend;
					sf::Socket::Status st;
					do
					{
						st = sockets.at(playerSend)->send(lastMessage.c_str(), lastMessage.length(), confirmedSend);	//vigilar lo del lastMessage
						if (st == sf::Socket::Status::Partial)
						{
							lastMessage = lastMessage.substr(confirmedSend + 1, lastMessage.length());	//length() segur?
						}
					} while (st == sf::Socket::Status::Partial);
					//End Send-------------------
				}
			}
			else if (result == sf::TcpSocket::Status::Disconnected)
			{
				//enviar a cada
				//aMensajes.push_back("The user has disconnected");
			}

			//End Receive-------------------------------
		}
	}

	for (int playerN = 0; playerN < MAXPLAYERS; playerN++)	//aquí en teoria no arribarà mai.... 
	{
		sockets.at(playerN)->disconnect();
	}
	system("pause");
	
	return 0;

}