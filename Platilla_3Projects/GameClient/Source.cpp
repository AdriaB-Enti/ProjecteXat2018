#pragma once
#include <PlayerInfo.h>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>
#include <Constants.h>

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

	bool confirmationRecieved = true;
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

	/*int n;
	std::cin >> n;*/

	//Creación de la ventana
	sf::Vector2i screenDimensions(800, 900);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "UDPGame");

	//Texturas, Sprites y fuentes
	sf::RectangleShape mapShape(sf::Vector2f(TILESIZE*N_TILES_WIDTH, TILESIZE*N_TILES_HEIGHT));

	sf::Texture texture, characterTexture;
	if (!texture.loadFromFile("mapa2.png"))
		std::cout << "Error al cargar la textura del mapa!\n";
	if (!characterTexture.loadFromFile("personatgeTransp.png"))
		std::cout << "Error al cargar la textura del personaje!\n";
	/*if (!font.loadFromFile("courbd.ttf"))
		std::cout << "Error al cargar la fuente" << std::endl;*/

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//Cerrar la ventana
			if (event.type == sf::Event::Closed)
				window.close();

			//Detectar eventos de teclado
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape))
				window.close();

			//Detectar eventos de ratón
			if (event.type == sf::Event::MouseButtonPressed) {
				std::cout << "Mouse Pressed at position: " << event.mouseButton.x << ":"
					<< event.mouseButton.y << std::endl;
				/*if (Game::currentTurn == miTurno)
				{
					sendMove(event.mouseButton.x, event.mouseButton.y);
				}*/
				//sf::Mouse::getPosition(window)
			}

			//Detectar si estamos escribiendo algo, enviar el texto si presionamos enter, borrar la ultima letra si apretamos Backspace
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode > 31 && event.text.unicode < 128) {
					/*mensajeTeclado.push_back(static_cast<char>(event.text.unicode));*/
				}
			}
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Return)) {		//Si apretamos enter, se envia el mensaje que teniamos escrito - TODO: controlar que no s'envii si està buit
				/*if (!mensajeTeclado.empty()) {
					sendToServer(user + ": " + mensajeTeclado);
					mensajeTeclado = "";
				}*/
			}
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::BackSpace)) {
				/*if (!mensajeTeclado.empty())
					mensajeTeclado.pop_back();*/
			}
		}

		//recieveFromServer();


		window.clear();

		//Dibujar el mapa i jugadores
		//window.draw(mapShape);
		/*for (int i = 0; i < MAXPLAYERS; i++)
		{
			characterSprite.setPosition(sf::Vector2f(jugadores.at(i).position*TILESIZE));
			if (jugadores.at(i).isDead)
			{
				characterSprite.rotate(-90);
				characterSprite.move(sf::Vector2f(0, TILESIZE));
			}
			window.draw(characterSprite);
			characterSprite.setRotation(0);
			window.draw(jugadores.at(i).nameText);
			window.draw(gameResult);
		}*/

		window.display();
	}








	return 0;
}