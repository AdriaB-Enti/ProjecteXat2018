//Bootstrap Server
#include <SFML/Network.hpp>

#define MAX_PLAYERS 2 //TODO - CANVIAR PER 4 --------------------------------

struct Direccion
{
	std::string ip;
	unsigned short port;
};
int main() {
	std::vector<Direccion> direcciones;

	sf::TcpListener listener;
	listener.listen(50000);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		sf::TcpSocket sock;
		listener.accept(sock); //.................
		//usar packet
		//omplir amb les direccions
		//enviar el packet
		direcciones.push_back({ sock.getRemoteAddress().toString(), sock.getRemotePort() });
		sock.disconnect();
	}
	listener.close();
	system("pause");
	return 0;
}