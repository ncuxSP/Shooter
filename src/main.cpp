#include "stdafx.h"

#include "Framework/Application.h"

int main(int argc, char *argv[])
{
	Engine::Application app("Shooter", Engine::Size(1280, 800));

	return app.Run();
}