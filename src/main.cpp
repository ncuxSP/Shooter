#include "stdafx.h"

#include "Game/Game.h"

int main(int argc, char *argv[])
{
	Game game("Shooter", Size(1280, 800));

	return game.Run();
}