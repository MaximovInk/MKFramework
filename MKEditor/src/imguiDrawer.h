#pragma once
#include "MKCore.h"
#include "MKUtils.h"
#include "imgui/src/misc/cpp/imgui_stdlib.h"
#include "imgui/imgui.h"
#include <map>
#include <string>
#include <regex>

namespace MKEditor {

	

	void drawTransform(MKEngine::transform* _transform) {
		float v[3] = { _transform->pos.x, _transform->pos.y, _transform->pos.z};

		bool changed = false;

		if (ImGui::InputFloat3("Position", v)) {
			_transform->pos = glm::vec3(v[0],v[1],v[2]);
			changed = true;
		}
		float v1[3];
		v1[0] = _transform->eulerRot.x;
		v1[1] = _transform->eulerRot.y;
		v1[2] = _transform->eulerRot.z;
		if (ImGui::InputFloat3("Rotation", v1)) {
			_transform->eulerRot = glm::vec3(v1[0], v1[1], v1[2]);
			changed = true;
		}
		float v2[3];
		v2[0] = _transform->scale.x;
		v2[1] = _transform->scale.y;
		v2[2] = _transform->scale.z;
		if (ImGui::InputFloat3("Scale", v2)) {
			_transform->scale = glm::vec3(v2[0], v2[1], v2[2]);
			changed = true;
		}

		if (changed) {
			_transform->updateLocalMatrix();
		}
	}

	void drawComponent(MKEngine::component* _component) {

		std::string name = std::string(typeid(*_component).name());
		name.erase(0, 14);
		name = std::regex_replace(name, std::regex("\\Component"), "");
		ImGui::Text(name.c_str());
		//add components drawers
	}

	void drawEntity(MKEngine::scene* scene, MKEngine::entity* entity) {
		ImGui::Text(entity->name.c_str());
		ImGui::InputText("Name", &(entity->name));

		drawTransform(entity->transform);

		ImGui::Text("Components:");
		for (size_t i = 0; i < entity->components.size(); i++)
		{
			//ImGui::Text("Component + ");
			drawComponent(entity->components[i]);
		}


		if (ImGui::Button("Add component")) {
			ImGui::OpenPopup("Add component");
		}

		int selectedComp = -1;
		if (ImGui::BeginPopup("Add component"))
		{
			ImGui::Text("Add component:");
			ImGui::Separator();
			for (int i = 0; i < 5; i++)
				if (ImGui::Selectable(("Component" + std::to_string(i)).c_str()))
					selectedComp = i;
			ImGui::EndPopup();
		}

		if (selectedComp == 0) {
			entity->components.push_back(new MKEngine::meshComponent(entity));
		}
	}
}