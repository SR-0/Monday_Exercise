#include "Engine.h"





const unsigned int	WIDTH			= 1024;
const unsigned int	HEIGHT			= 768;
const char*	const	TITLE			= "Test Title";
const unsigned int	ANTI_ALIASING	= 4;





int main()
{
	Engine::Create(WIDTH, HEIGHT, TITLE, nullptr, nullptr, ANTI_ALIASING);
	Engine::Setup();

	while (Engine::IsRunning())
	{
		Engine::Update();
		Engine::Render();
	}

	Engine::Destroy();
}
