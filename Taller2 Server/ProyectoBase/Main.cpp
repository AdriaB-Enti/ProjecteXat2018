#include <SFML\Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#define MAXPLAYERS 2

void sendToAll(std::vector<sf::TcpSocket*> &sockets, std::string mensaje);	//envia un mensaje a todos los clientes conectados

int main()
{
	//Servidor Cliente
	std::vector<sf::TcpSocket*> sockets;
	sf::TcpListener listener;
	listener.listen(50000);
	for (int playerN = 0; playerN < MAXPLAYERS; playerN++)
	{
		sf::TcpSocket* newSock = new sf::TcpSocket();
		listener.accept(*newSock);
		newSock->setBlocking(false);
		sockets.push_back(newSock);

		std::string texto = "Conexion con ... " + (newSock->getRemoteAddress()).toString() + ":" + std::to_string(newSock->getRemotePort()) + "\n";
		std::cout << texto;

		sendToAll(sockets, "Se ha conectado un nuevo usuario. Bienvenido!"); //Enviamos un mensaje a los demás clientes cuando alguien se conecta
	}
	listener.close();
	std::cout << "Todos los usuarions conectados. El chat se ha iniciado!\n";


	char buffer[100];
	size_t bytesReceived;

	while (true)																							//el server estarà sempre obert
	{
		std::vector<sf::TcpSocket*>::iterator it = sockets.begin();
		while (it != sockets.end())			//fem un recieve per cada client
		{
			//Receive-------------------------------
			sf::TcpSocket::Status result = (*it)->receive(buffer, 100, bytesReceived);
			buffer[bytesReceived] = '\0';
			if (result == sf::TcpSocket::Status::Done)
			{
				std::string lastMessage = std::string(buffer);
				sendToAll(sockets, lastMessage);
				++it;
			}
			else if (result == sf::TcpSocket::Status::Disconnected)
			{
				//enviar a cada client
				std::cout << "Un cliente se ha desconectado" << std::endl;
				(*it)->disconnect();
				delete * it;
				it = sockets.erase(it);
				sendToAll(sockets, "Un usuario se ha desconectado");
			}
			else if (result == sf::TcpSocket::Status::Error) {
				std::cout << "ERROR!!!" << std::endl;
				++it;
			}
			else if (result == sf::TcpSocket::Status::NotReady) {
				//std::cout << "NOT READY---------" << std::endl;
				++it;
			}
			else
			{
				++it;
			}
			//End Receive-------------------------------
		}
	}

	system("pause");
	
	return 0;

}

void sendToAll(std::vector<sf::TcpSocket*> &sockets, std::string mensaje) {
	for (std::vector<sf::TcpSocket*>::iterator itera = sockets.begin(); itera != sockets.end(); ++itera)
	{
		size_t confirmedSend;
		sf::Socket::Status st;
		do
		{
			st = (*itera)->send(mensaje.c_str(), mensaje.length(), confirmedSend);
			if (st == sf::Socket::Status::Partial)
			{
				mensaje = mensaje.substr(confirmedSend + 1, mensaje.length());
			}
		} while (st == sf::Socket::Status::Partial);
	}
}