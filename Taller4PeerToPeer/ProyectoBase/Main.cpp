#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>

#define MAX_PLAYERS 3 //TODO - CANVIAR PER 4 ---------------------------------------------
#define BOOTSTRAP_PORT 50000
#define BOOTSTRAP_IP "localhost"
struct Direccion
{
	sf::String ip;
	sf::Uint16 port;
};

void coutStatus(sf::TcpSocket::Status &status);
void sendAll(sf::String msg);
void recieveFromAll();

bool test = false;
std::vector<sf::TcpSocket*> sockets;
std::vector<Direccion> direcciones;
sf::Socket::Status status;
unsigned short myPort;
std::vector<std::string> aMensajes;

int main()
{

	std::string user = "";
	std::cout << "Escribe tu nombre: \n";
	std::cin >> user;


	sf::TcpSocket socket;
	sf::String ip = BOOTSTRAP_IP;	//TODO-- POSAR LO DEL ENTER
	std::cout << "Conectando al servidor bootstrap...\n";
	status = socket.connect(sf::IpAddress(ip), BOOTSTRAP_PORT, sf::milliseconds(15));	//TODO: controlar?-----
	myPort = socket.getLocalPort();
	if (status == sf::Socket::Status::Done)
		std::cout << "Conectado al servidor (" << socket.getRemoteAddress() << ")"<< std::endl;
	coutStatus(status);
	
	//Rebre les dades del packet
	sf::Packet packet;
	status = socket.receive(packet);
	socket.disconnect();

	sf::Int8 numDirecciones = 0;
	packet >> numDirecciones;
	std::cout << "Num direcciones recibidas: " << (int)numDirecciones << std::endl;
	for (int i = 0; i < numDirecciones; i++)
	{
		Direccion dir = {};
		packet >> dir.ip;
		packet >> dir.port;
		direcciones.push_back(dir);
	}
	if (numDirecciones == 0)
	{
		std::cout << "No se han recibido direcciones\n";
		test = true;
	}
	std::cout << "--------- Iniciando conexion entre peers ---------\n";
	//Conectar-se als peer's que tenim
	for (int i = 0; i < direcciones.size(); i++)
	{
		std::cout << "Conectando con peer en la lista: " << direcciones[i].ip.toAnsiString() << ":" << direcciones[i].port << std::endl;
		sf::TcpSocket *newSocket = new sf::TcpSocket();
		status = newSocket->connect(sf::IpAddress(direcciones[i].ip), (unsigned short)direcciones[i].port, sf::milliseconds(15));
		coutStatus(status);
		sockets.push_back(newSocket);
	}

	//Esperar que sen's conectin els peer's que falten
	if (direcciones.size() < MAX_PLAYERS-1)
	{
		sf::TcpListener listener;
		listener.listen(myPort);
		for (int i = direcciones.size(); i < MAX_PLAYERS-1; i++)
		{
			sf::TcpSocket *newSocket = new sf::TcpSocket();
			status = listener.accept(*newSocket);		//-------------------------- controlar?
			std::cout << "Encontrada conexion con: " << newSocket->getRemoteAddress().toString() << ":" << newSocket->getRemotePort() << std::endl;
			coutStatus(status);
			sockets.push_back(newSocket);
		}
		listener.close();
	}

	std::cout << "Cliente terminado. Conexiones establecidas: " << sockets.size() << std::endl;

	//poner los sockets en nonBlocking
	for (int i = 0; i < sockets.size(); i++)
	{
		sockets[i]->setBlocking(false);
	}

	/*if (test)
	{
		Sleep(5000);
		sendAll("Hello peers!!");
		Sleep(1000);
		sendAll("Bye peers!!");
	} else{
		while (true)
		{
			recieveFromAll();
		}
	}*/


	std::cout << "--------- Iniciando chat ---------\n";
	aMensajes.push_back("Chat is online. Welcome!");
	/*std::cout << "Escribe la IP del bootsrap server (pulsa enter si quieres conectarte a localhost):\n";
	std::string ip = "";
	std::cin.ignore();
	std::getline(std::cin, ip);
	if (ip.empty()) {
		ip = "localhost";
	}*/
	std::string textoAEnviar="";

	//GUI--------------------------------------------------------------
	sf::Vector2i screenDimensions(800, 600);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje = " >";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);

	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);


	char buffer[100];
	size_t bytesReceived;

	while (window.isOpen())
	{
		sf::Event evento;
		while (window.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					//aMensajes.push_back(user+mensaje);
					
					//Send-------------------
					std::string messageToSend = user + ": " + mensaje;
					//size_t confirmedSend;
					//sf::Socket::Status st;
					//do
					//{
					//	st = socket.send(messageToSend.c_str(), messageToSend.length(), confirmedSend);	//vigilar lo del messageToSend
					//	if (st == sf::Socket::Status::Partial)
					//	{
					//		messageToSend = messageToSend.substr(confirmedSend+1, messageToSend.length());	//length() segur?
					//	}
					//} while (st == sf::Socket::Status::Partial);
					////End Send-------------------
					sendAll(messageToSend);

					if (aMensajes.size() > 25)
					{
						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
					}
					mensaje = ">";
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
					mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
				break;
			}
		}
		window.draw(separator);

		//Receive-------------------------------
		//sf::TcpSocket::Status result = socket.receive(buffer, 100, bytesReceived);
		//buffer[bytesReceived] = '\0';
		//if (result == sf::TcpSocket::Status::Done)
		//{
		//	aMensajes.push_back(std::string(buffer)); //segurament s'ha de passar el buffer a string
		//}
		//else if (result == sf::TcpSocket::Status::Disconnected)
		//{
		//	aMensajes.push_back("The user has disconnected");
		//	window.close();
		//}
		recieveFromAll();
		//End Receive-------------------------------


		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		window.draw(text);
		window.display();
		window.clear();
	}


	//desconectamos todos los sockets
	for (int i = 0; i < sockets.size(); i++)
	{
		sockets[i]->disconnect();
	}
	system("pause");
	
	return 0;

}

void coutStatus(sf::TcpSocket::Status &status) {
	std::string statusStr = "";
	switch (status)
	{
	case sf::Socket::Done:
		statusStr = "Done";
		break;
	case sf::Socket::NotReady:
		statusStr = "NotReady";
		break;
	case sf::Socket::Partial:
		statusStr = "Partial";
		break;
	case sf::Socket::Disconnected:
		statusStr = "Disconnected";
		break;
	case sf::Socket::Error:
		statusStr = "Error";
		break;
	default:
		break;
	}
	std::cout << "Status: " << statusStr << std::endl;
}


void sendAll(sf::String msg) {
	for (int i = 0; i < sockets.size(); i++)
	{
		size_t confirmedSend;
		do
		{
			status = sockets[i]->send(msg.toAnsiString().c_str(), msg.toAnsiString().length(), confirmedSend);	//TODO.... comprovar que s'hagi enviat tot...
			if (status == sf::Socket::Status::Partial)
			{
				msg = msg.substring(confirmedSend + 1, msg.getSize());	//getSize() - length() segur?
			}
		} while (status == sf::Socket::Status::Partial);

	}
}

void recieveFromAll() {
	for (int i = 0; i < sockets.size(); i++)
	{
		char buffer[100];
		size_t bytesReceived;
		status = sockets[i]->receive(buffer, 100, bytesReceived);
		buffer[bytesReceived] = '\0';
		
		if (status == sf::Socket::Status::Done)
		{
			aMensajes.push_back(std::string(buffer));
		}
		else if (status == sf::Socket::Status::Disconnected)
		{
			aMensajes.push_back("A user has disconnected");
			//window.close();
			//esborrar.....------------------------------------------------
		}

	}

}

