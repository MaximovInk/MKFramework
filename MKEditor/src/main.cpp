#include "iostream"
#include <format>

#include "imguiLayouts.h"

#include "imguizmo/imguizmoquat.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#define IMGUI_IMPLEMENTATION
#include "imgui/imgui.h"
#include "glm/gtx/string_cast.hpp"

#define SDL_MAIN_HANDLED
#include "MKCore.h"
#include "MKGraphics.h"
#include "MKUtils.h"
#include "graphicsUtils.h"
//#include "scene/scene.h"
#include "minecraft/chunk.h"

using namespace MKEngine;
using namespace MKGraphics;

mesh* _mesh;
shader* sh;
texture* tex;
camera* cam;
glm::mat4 model = glm::mat4(1.0);

MKEngine::scene* _scene;

int windowWidth;
int windowHeight;

void SetupImGuiStyle(bool bStyleDark_, float alpha_);

int mouseLockPosX;
int mouseLockPosY;
void cameraHandleInput(window* wnd, float deltaTime) {
	float speed = 3 * deltaTime;

	if (Input::getKey(SDL_SCANCODE_LSHIFT))
		speed *= 2;
	if (Input::getKey(SDL_SCANCODE_A))
		cam->moveRight(-speed);
	if (Input::getKey(SDL_SCANCODE_D))
		cam->moveRight(speed);
	if (Input::getKey(SDL_SCANCODE_W))
		cam->moveForward(speed);
	if (Input::getKey(SDL_SCANCODE_S))
		cam->moveForward(-speed);
	if (Input::getKey(SDL_SCANCODE_SPACE))
		cam->moveUp(speed);
	if (Input::getKey(SDL_SCANCODE_LCTRL))
		cam->moveUp(-speed);

	if (Input::getMouseButton(SDL_BUTTON_LMASK)) {
		if (Input::getMouseButtonDown(SDL_BUTTON_LMASK)) {
			SDL_ShowCursor(false);
			Input::getMousePos(&mouseLockPosX, &mouseLockPosY);
			SDL_WarpMouseInWindow(wnd->getNativeWindow(), mouseLockPosX, mouseLockPosY);
		}
		else {
			int mx,my;
			Input::getMousePos(&mx, &my);
			float rotX =  (float)(my - mouseLockPosY) / windowHeight;
			float rotY =  (float)(mx - mouseLockPosX) / windowWidth;
			cam->mouseMove(rotX, rotY);
		}
		SDL_WarpMouseInWindow(wnd->getNativeWindow(), mouseLockPosX, mouseLockPosY);
	}
	else if (Input::getMouseButtonUp(SDL_BUTTON_LMASK)) {
		SDL_ShowCursor(true);
	}
	cam->updateMatrix(45.0f, 0.1f, 100.0f, (float)windowWidth/windowHeight);
}

void winUpdateCallback(window* wnd, float deltaTime) 
{
	wnd->getSize(&windowWidth, &windowHeight);

	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse) {
		cameraHandleInput(wnd, deltaTime);
	}
}

bool polygonMode = false;
void togglePolygonMode() {
	if(polygonMode)
		setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool cullface = false;
void toggleCullface() {
	if(cullface)
		MKGraphics::enable(GL_CULL_FACE);
	else
		MKGraphics::disable(GL_CULL_FACE);
}

bool drawDemoGUI = false;

void drawControls() {
	ImGui::Begin("Controls");

	if (ImGui::Checkbox("Cullfacing", &cullface)) toggleCullface();
	if (ImGui::Checkbox("PolygonMode", &polygonMode)) togglePolygonMode();
	ImGui::Checkbox("Demo GUI", &drawDemoGUI);

	ImGui::Text(glm::to_string(cam->Position).c_str());

	ImGui::End();
}
void winSDLCallback(window* wnd, SDL_Event event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

MKGame::chunk* chunk;
void winRenderCallback(window* wnd, float deltaTime) 
{
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	clearColor(229.0/255,204.0/255,214.0/255,1);
	setViewport(0, 0, wnd->getWidth(), wnd->getHeight());
	
	tex->bind();
	sh->use();
	sh->setMat4("camMatrix", cam->Matrix);
	sh->setMat4("model", model);

	//_mesh->draw();
	chunk->draw();
	_scene->render();
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(wnd->getNativeWindow());
	ImGui::NewFrame();

	glm::mat4 camView = glm::lookAt(cam->Position, cam->Position + cam->Orientation, -cam->Up);

	glm::quat quatTest = glm::quat_cast(camView);

	quat qRot = quat(quatTest.x,quatTest.y, quatTest.z, quatTest.w);
	ImGui::gizmo3D("##gizmo1", qRot,100, imguiGizmo::mode3Axes | imguiGizmo::cubeAtOrigin);

	if(drawDemoGUI)
		ImGui::ShowDemoWindow();

	drawControls();

	MKEditor::drawFrameRate(deltaTime);
	MKEditor::drawHierarchy(_scene);
	MKEditor::drawInspector(_scene);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


int main(int argc, char* args[]) {
	application::init();
	window win("Hello window", 500, 500, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext glContext = SDL_GL_CreateContext(win.getNativeWindow());
	application::setGLContext(glContext);
	MKGraphics::loadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
	
	LOG::info("fsgd {}, {}", 0, 25);
	MKEngine::utilsExample1();
	MKGraphics::enable(GL_DEPTH_TEST);

	sh = new shader(
		"C:\\Users\\MInk\\source\\repos\\MKFramework\\resources\\shaders\\simpleShader.vert",
		"C:\\Users\\MInk\\source\\repos\\MKFramework\\resources\\shaders\\simpleShader.frag");

	tex = new texture("C:\\Users\\MInk\\source\\repos\\MKFramework\\resources\\textures\\common\\dirt.png","diffuse",0, GL_UNSIGNED_BYTE);
	
	cam = new camera(glm::vec3(0,0,2));
	
	chunk = new MKGame::chunk();

	for (size_t i = 0; i < 16; i++)
	{
		for (size_t j = 0; j < 16; j++)
		{
			chunk->setTile(glm::ivec3(i, 0, j), 15);
			chunk->setTile(glm::ivec3(i, 1, j), 15);
			chunk->setTile(glm::ivec3(i, 2, j), 15);
		}
	}

	chunk->generateMesh();
	

	/*
	

	chunk->checkTile(0);
	chunk->checkTile(25823);
	chunk->checkTile(1280);
	chunk->checkTile(3);
	chunk->checkTile(99);
	*/
	

	_scene = new scene();

	_scene->cam = cam;

	_scene->createEntity();
	_scene->createEntity();
	_scene->createEntity();
	auto ent1 = _scene->createEntity();
	ent1->name = "Example name";
	_scene->createEntity();
	_scene->createEntity();

	
	/*
	
	std::vector<vertex> verts{
	vertex{glm::vec3(0,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,0)},
	vertex{glm::vec3(0,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(0,1)},
	vertex{glm::vec3(1,1,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,1)},
	vertex{glm::vec3(1,0,0), glm::vec3(0),glm::vec3(1), glm::vec2(1,0)}
	};
	std::vector<GLuint> ind{ 0,1,2,0,2,3 };


	_mesh = new mesh();
	_mesh->vertices = verts;
	_mesh->indices = ind;
	_mesh->applyData();
	
	*/

	//_mesh = MKEngine::Utils::makeQuad(glm::vec2(-0.9f),glm::vec2(0.9f),MKGraphics::AXIS_Z);
	//_mesh = MKEngine::Utils::makeCube();
	

	auto glsl_version = "#version 460";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::StyleColorsDark();

	SetupImGuiStyle(true, 1.0f);

	ImGui_ImplSDL2_InitForOpenGL(win.getNativeWindow(), glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	win.setRenderCallback(winRenderCallback);
	win.setUpdateCallback(winUpdateCallback);
	win.setSDLEventCallback(winSDLCallback);

	application::startLoop();

	application::finalize();

	system("pause");
	return 0;
}

void SetupImGuiStyle(bool bStyleDark_, float alpha_)
    {
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		//style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		//style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		style.WindowRounding = 10;
		style.Alpha = 0.97f;
		style.ScrollbarSize = 17;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;

		style.GrabRounding = style.FrameRounding = 2.3f;
    }