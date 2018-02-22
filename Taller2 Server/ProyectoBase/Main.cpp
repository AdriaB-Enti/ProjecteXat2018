#include <SFML\Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#define MAXPLAYERS 4


int main()
{
	//Servidor Cliente
	
	std::vector<sf::TcpSocket> sockets;
	int playersConnected = 0;
	sf::TcpSocket socket;
	socket.setBlocking(false);

	std::string textoAEnviar="";
	std::vector<std::string> aMensajes;
	sf::TcpListener listener;

	for (int playerN = 0; playerN < MAXPLAYERS; playerN++)
	{
		listener.listen(50000);
		listener.accept(socket);
		textoAEnviar = "Mensaje desde servidor\n";
		listener.close();
		std::cout << "User connected:\n";

		std::string texto = "Conexion con ... " + (socket.getRemoteAddress()).toString() + ":" + std::to_string(socket.getRemotePort()) + "\n";
		std::cout << texto;

	}
	std::cout << "All users connected. Chat is online!\n";


	char buffer[100];
	size_t bytesReceived;

	while (true)
	{
		
		//Receive-------------------------------
		sf::TcpSocket::Status result = socket.receive(buffer, 100, bytesReceived);
		buffer[bytesReceived] = '\0';
		if (result == sf::TcpSocket::Status::Done)
		{
			aMensajes.push_back(std::string(buffer)); //segurament s'ha de passar el buffer a string


			//Send-------------------
			std::string lastMessage = aMensajes.back();
			size_t confirmedSend;
			sf::Socket::Status st;
			do
			{
				st = socket.send(lastMessage.c_str(), lastMessage.length(), confirmedSend);	//vigilar lo del lastMessage
				if (st == sf::Socket::Status::Partial)
				{
					lastMessage = lastMessage.substr(confirmedSend + 1, lastMessage.length());	//length() segur?
				}
			} while (st == sf::Socket::Status::Partial);
			//End Send-------------------

		}
		else if (result == sf::TcpSocket::Status::Disconnected)
		{
			aMensajes.push_back("The user has disconnected");
		}

		//End Receive-------------------------------

	}

	
	socket.disconnect();

	
	system("pause");
	
	return 0;

}