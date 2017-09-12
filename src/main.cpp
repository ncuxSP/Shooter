#include "stdafx.h"

#include "Game/Game.h"

int main(int argc, char *argv[])
{
	Game app("Shooter", Size(1280, 800));

	return app.Run();
}