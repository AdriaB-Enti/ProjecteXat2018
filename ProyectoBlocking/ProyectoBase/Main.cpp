#include <SFML\Network.hpp>

#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex myMutex;
bool running = true;

void readData(sf::TcpSocket *socket, std::vector<std::string> *aMensajes) {
	char buffer[100];
	size_t bytesReceived;

	while (running)
	{
		//Receive-------------------------------
		sf::TcpSocket::Status result = socket->receive(buffer, 100, bytesReceived);
		buffer[bytesReceived] = '\0';
		if (result == sf::TcpSocket::Status::Done)
		{
			std::lock_guard<std::mutex> guard(myMutex);
			aMensajes->push_back(std::string(buffer)); //segurament s'ha de passar el buffer a string
		}
		else if (result == sf::TcpSocket::Status::Disconnected)
		{
			running = false;
			std::lock_guard<std::mutex> guard(myMutex);
			aMensajes->push_back("The user has disconnected");
		}
		else if (result == sf::TcpSocket::Status::Error) {
			running = false;
		}
		//End Receive-------------------------------
	}

}

void prova(int numm, int num1, int num2, int num3, int num4, int num5) {
	while (true)
	{
		printf("esto es una prueba\n");

	}
	numm = num1 + num2 + num3 + num4 + num5;
}

int main()
{
	//Servidor Cliente
	std::cout << "¿Seras servidor (s) o cliente (c)? ... ";
	char c;
	std::cin >> c;
	sf::TcpSocket *socket = new sf::TcpSocket();
	std::string textoAEnviar="";
	std::vector<std::string> *aMensajes = new std::vector<std::string>();		//s'ha de inicialitzar socket i aMensajes?
	sf::String user = "";	//client o server
	if (c == 's')
	{
		sf::TcpListener listener;
		listener.listen(50000);
		listener.accept(*socket);
		textoAEnviar = "Mensaje desde servidor\n";
		listener.close();
		aMensajes->push_back("Chat is online: You are the server!");
		user = "Server: ";
	}
	else if (c == 'c')
	{
		socket->connect("localhost", 50000, sf::milliseconds(15.f));
		textoAEnviar = "Mensaje desde cliente\n";
		aMensajes->push_back("Chat is online: You are the client!");
		user = "Client: ";
	}
	else
	{
		exit(0);
	}
	
	std::string texto = "Conexion con ... " + (socket->getRemoteAddress()).toString() + ":" + std::to_string(socket->getRemotePort()) + "\n";
	std::cout << texto;


	//Chat
	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow *window = new sf::RenderWindow();		//iniciar?
	window->create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

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


	std::thread t(&readData, socket, aMensajes);
	
	while (running && window->isOpen())
	{
		sf::Event evento;
		while (window->pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window->close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					std::lock_guard<std::mutex> guard(myMutex);
					aMensajes->push_back(user+mensaje);
					
					
					//Send-------------------
					std::string lastMessage = aMensajes->back();
					size_t confirmedSend;
					sf::Socket::Status st;
					do
					{
						st = socket->send(lastMessage.c_str(), lastMessage.length(), confirmedSend);	//vigilar lo del lastMessage
						if (st == sf::Socket::Status::Partial)
						{
							lastMessage = lastMessage.substr(confirmedSend+1, lastMessage.length());	//length() segur?
						}
					} while (st == sf::Socket::Status::Partial);
					//End Send-------------------


					if (aMensajes->size() > 25)
					{
						aMensajes->erase(aMensajes->begin(), aMensajes->begin() + 1);
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
		window->draw(separator);

		

		for (size_t i = 0; i < aMensajes->size(); i++)
		{
			std::string chatting = (*aMensajes)[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window->draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		window->draw(text);
		window->display();
		window->clear();
	}
	running = false;
	t.join();
	socket->disconnect();
	window->close();
	delete window;
	delete aMensajes;
	delete socket;
	system("pause");
	
	return 0;

}