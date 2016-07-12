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

Display* display = Display::MakeGlobal(1600, 900, "LANDON");
double times = 1;
Camera* camera;
Shader* shade;
Texture tex, bg, chair;
Image img;
Prism prism(10,10,10);
Prop prop;
Ambient amb;
Spotlight spot;
Sprite sprite;
Player player;

int main() {
	//Server server("192.168.0.4", 333, 24);
	//server.CallbackString(OnString);
	camera = Camera::Global();
	shade = Shader::Global();
	shade->Init("./res/shader2");
	tex.Load("./res/tex/cuboid.jpg");
	bg.Load("./res/tex/blueprint.jpg");
	chair.Load("./res/model/chair.jpg");
	img.Init2D(0, 0, 1600, 900, bg);
	display->Update();

	prop.SetMesh("./res/model/chair.obj");
	prop.SetTex(chair);
	prop.Scale(0.4);

	amb.SetRange(7);
	spot.SetColor(0, 0, 1);
	prism.SetTex(tex);
	prism.Teleport(-5, -1, -5);
	sprite.Init(bg);
	sprite.Teleport(3, 0.5, 2);

	player.Move(0, 0.3, 0);

	while (display->IsOpen()) {
		times += 0.01;
		display->Clear(0.5, 0.5, 0.5, 1);
		//prop.Update();
		//img.Draw();
		spot.Teleport(player.GetEyePos());
		spot.SetDirection(camera->GetForward());
		sprite.Draw();
		prism.Draw();
		prop.Draw();
		prop.Teleport(0, cos(times), sin(times) - 4);
		prop.SetRotation(0, sin(times), cos(times));
		player.ApplyVelocity();
		World::CalcCollisions();
		player.Update();
		display->Update();
	}
	return 0;
}