#pragma once
#include "imgui/imgui.h"
#include "imguiDrawer.h"
#include "MKCore.h"
#include "MKUtils.h"
#include "graphicsUtils.h"

namespace MKEditor {
#define FRAMERATE_RES 64

	MKEngine::entity* selected = nullptr;

	void drawHierarchy(MKEngine::scene* _scene) {
		ImGui::Begin("Hierarchy");

		if (ImGui::IsWindowHovered())
		{
			if (ImGui::IsMouseClicked(1))
			{
				ImGui::OpenPopup("HierarchyOptions");
			}
		}

		if (ImGui::BeginPopup("HierarchyOptions"))
		{
			if (ImGui::MenuItem("Create empty"))
			{
				_scene->createEntity();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Create 3d object"))
			{
				if (ImGui::MenuItem("Cube")) {
					auto _ent = _scene->createEntity();
					auto _mc = new MKEngine::meshComponent(_ent);
					_mc->mesh = MKEngine::Utils::makeCube();
					_mc->shader = new shader(
						"C:\\Users\\Danila\\Documents\\Github\\MKFramework\\resources\\shaders\\simpleShader.vert",
						"C:\\Users\\Danila\\Documents\\Github\\MKFramework\\resources\\shaders\\simpleShader.frag");

					_ent->components.push_back(_mc);
				}
				ImGui::MenuItem("Sphere");
				ImGui::MenuItem("Plane");
				ImGui::MenuItem("Cylinder");
				if(ImGui::MenuItem("Flower") ){
					auto _ent = _scene->createEntity();
					auto _mc = new MKEngine::meshComponent(_ent);
					auto _tc = new MKEngine::textureComponent(_ent);

					_mc->mesh = MKEngine::Utils::makeQuad(glm::vec2(0, 0), glm::vec2(1, 1), MKGraphics::AXIS_Z);

					_mc->shader = new shader(
						"C:\\Users\\Danila\\Documents\\Github\\MKFramework\\resources\\shaders\\simpleShader.vert",
						"C:\\Users\\Danila\\Documents\\Github\\MKFramework\\resources\\shaders\\simpleShader.frag");

					_tc->_texture = new texture("C:\\scg.png", "diffuse", 0, GL_UNSIGNED_BYTE);

					_ent->components.push_back(_tc);
					_ent->components.push_back(_mc);

					
				}
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		auto entites = _scene->entities;
		int index = 0;
		bool isSelected = false;
		for (size_t i = 0; i < entites.size(); i++)
		{
			ImGui::PushID(index);
			bool treeNodeOpen = ImGui::TreeNodeEx(
				entites[i]->name.c_str(),
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_SpanAvailWidth,
				entites[i]->name.c_str());

			if (treeNodeOpen) {
				ImGui::TreePop();
			}

			if (ImGui::IsMouseClicked(0) && !isSelected && ImGui::IsItemHovered()) {
					selected = entites[i]; isSelected = true;
			}
			if (ImGui::IsMouseClicked(0) && !isSelected && ImGui::IsWindowHovered()) {
					selected = nullptr;
			}

			ImGui::PopID();
			index++;
		}

		if (ImGui::Button("Add entity")) {
			_scene->createEntity();
		}

		ImGui::End();
	}

	void drawInspector(MKEngine::scene* _scene) {
		ImGui::Begin("Inspector");

		if (selected != nullptr) {
			drawEntity(_scene,selected);
		}

		ImGui::End();
	}

	float framerate[FRAMERATE_RES];
	void drawFrameRate(float deltaTime) {

		for (size_t i = 1; i < FRAMERATE_RES; i++)
		{
			framerate[i - 1] = framerate[i];
		}
		framerate[FRAMERATE_RES - 1] = deltaTime * 100;

		ImGui::Begin("Framerate");

		ImGui::Text(("FPS:" + std::to_string(int(1.0f / deltaTime)) + " " + std::format("{:.2f}", deltaTime * 1000) + "ms").c_str());

		ImGui::PlotLines("Frame Times", framerate, FRAMERATE_RES, 0, 0, 0, 100);
		ImGui::End();
	}

}