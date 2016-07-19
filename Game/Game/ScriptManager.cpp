#include "ScriptManager.h"
#include <vector>
#include"Script_GameStart.h"

static std::vector<Script*> scripts;

Script* ScriptManager::Start(int id) {
	Script* result = nullptr;
	switch (id) {//ADD ALL NEW SCRIPT TYPES TO THIS SWITCH
	case 0:
		result = new GameStart;
		break;
	case 1:
		result = new Script;
		break;
	}
	if (result != nullptr) {
		result->SetID(id);
		scripts.push_back(result);
	}
	return result;
}

void ScriptManager::Stop(Script* obj) {
	Script* temp;
	for (unsigned int i = 0; i < scripts.size();i++) {
		temp = scripts[i];
		if (obj == temp) {
			scripts.erase(scripts.begin() + i);
			temp->Close();
			delete temp;
			break;
		}
	}
}

void ScriptManager::UnloadAll() {
	for (Script* temp : scripts) {
		temp->Close();
		delete temp;
	}
	scripts.clear();
}