#include "ScriptManager.h"

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }

int main() {
	//Server server("192.168.0.4", 333, 24);
	//server.CallbackString(OnString);
	srand((unsigned int)time(NULL));//Basically enables random number generations
	Display* display = Display::MakeGlobal(1600, 900, "VIDYA GAEM");
	if (Mix_OpenAudio(32000, MIX_DEFAULT_FORMAT, 2, 1024) < 0) { std::cout << "ERROR: MixOpenAudio: " << Mix_GetError() << std::endl; }
	Shader::Global()->Init("./res/shader2");

	ScriptManager::Start(0);

	Console::Log("World loop started");
	while (display->IsOpen()) {
		World::Update();
	}

	ScriptManager::UnloadAll();
	Sound::UnloadAll();
	Texture::UnloadAll();
	Mesh::UnloadAll();
	Mix_CloseAudio();
	system("pause");
	return 0;
}