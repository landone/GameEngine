#include "Console.h"
#include "Prism.h"
#include "Image.h"
#include "Prop.h"
#include "Ambient.h"
#include "Spotlight.h"
#include "Sprite.h"
#include "display.h"
#include "Player.h"
#include "EZServer.h"
#include "EZClient.h"
#include "World.h"
#include "ComFunc.h"

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }

class Alternate : public Listener {
public:
	Alternate() {
		Events::Listen(Evt_MouseClick, this);
	}
	void MouseClick(double x, double y, int type) {
		if (type == 1) {
			//prism.Rotate(0, 0, PI / 180);
			//glm::vec3 point = World::TraceRay(player.GetEyePos(), player.GetEyeDirection());
			//prop.Teleport(point);
		}
		else if (type == 3) {
			World::Test();
		}
	}
};
Alternate Alt;

int main() {
	//Server server("192.168.0.4", 333, 24);
	//server.CallbackString(OnString);
	srand((unsigned int)time(NULL));//Basically enables random number generations
	Display* display = Display::MakeGlobal(1600, 900, "VIDYA GAEM");
	Shader::Global()->Init("./res/shader2");
	Texture tex, bg, chair;
	Prism prism(10, 10, 10);
	Prop prop;
	Ambient amb;
	Spotlight spot;
	Sprite sprite;
	Player player;

	tex.Load("./res/tex/cuboid.jpg");
	bg.Load("./res/tex/blueprint.jpg");
	chair.Load("./res/model/chair.jpg");

	prop.SetMesh("./res/model/chair.obj");
	prop.SetTex(chair);
	prop.Scale(0.2);
	prop.Teleport(3, -1, 3);

	amb.SetRange(7);
	spot.SetColor(0, 0, 1);
	prism.SetTex(tex);
	prism.Teleport(-5, -1, -5);
	sprite.Init(bg);
	sprite.Teleport(3, 0.5, 2);

	player.Move(0, 4, 0);

	while (display->IsOpen()) {
		World::Update();
	}
	return 0;
}