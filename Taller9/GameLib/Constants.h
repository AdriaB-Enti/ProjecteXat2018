#pragma once

//Server
#define IPSERVER "127.0.0.1"
#define PORTSERVER 50000
#define MAXPLAYERS 2

//Game World
#define TILESIZE 100
#define N_TILES_WIDTH 8
#define N_TILES_HEIGHT 8
//Game properties
#define CHARACTER_SPEED 0.02f

//Enums
enum Cabeceras
{
	HELLO,
	WELCOME,
	ACKNOWLEDGE,
	NEW_PLAYER,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	OK_POSITION
};

