#include "Script_GameStart.h"

GameStart::GameStart(){
	Events::Listen(Evt_MouseClick, this);

	cube.Load("./res/tex/cuboid.jpg");
	blue.Load("./res/tex/blueprint.jpg");
	wood.Load("./res/tex/woodoid.jpg");

	prism.SetDimensions(10, 10, 10);
	prism.Teleport(-5, -5, -5);
	prism.SetTex(wood);
}

void GameStart::Close() {
	
}

void GameStart::MouseClick(double x, double y, int type) {
	if (type == 1) {
		prism.SetDimensions(prism.GetDimensions() - glm::vec3(1, 1, 1));
	}
}