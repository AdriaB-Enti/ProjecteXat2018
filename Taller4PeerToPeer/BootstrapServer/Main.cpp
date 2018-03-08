//Bootstrap Server
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <vector>

#define MAX_PLAYERS 3 //TODO - CANVIAR PER 4 --------------------------------
#define LISTEN_PORT 50000
struct Direccion
{
	sf::String ip;
	sf::Uint16 port;
};
int main() {
	std::vector<Direccion> direcciones;

	sf::TcpListener listener;
	std::cout << "Servidor iniciado, escuchando...\n";
	sf::Socket::Status status = listener.listen(LISTEN_PORT);

	if (status != sf::Socket::Status::Done)
	{
		std::cout << "No se ha podido escuchar en el puerto " << LISTEN_PORT << std::endl;
	}
	else
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			sf::TcpSocket sock;
			if (listener.accept(sock) != sf::Socket::Status::Done) {
				std::cout << "No se ha podido aceptar la conexión " << std::endl;
			}
			else
			{
				
				std::cout << "Conectado con: " << sock.getRemoteAddress() << ":" << sock.getRemotePort() << std::endl;
				sf::Packet packet;
				packet << (sf::Int8) direcciones.size();
				for (int dir = 0; dir < direcciones.size(); dir++)
				{
					packet << direcciones[dir].ip;
					packet << direcciones[dir].port;
				}
				sock.send(packet);
				direcciones.push_back({ sock.getRemoteAddress().toString(), sock.getRemotePort() });
				sock.disconnect();
			}
		}

	}
	std::cout << "Direcciones guardadas: " << direcciones.size() << std::endl;
	for (int i = 0; i < direcciones.size(); i++)
	{
		//std::cout << direcciones[i].ip.toAnsiString() << " port: ";
		std::cout << direcciones[i].ip.toAnsiString() << " port: " << direcciones[i].port << std::endl;
	}
	std::cout << "Trabajo del servidor terminado, cerrando listener\n";
	listener.close();
	system("pause");
	return 0;
}