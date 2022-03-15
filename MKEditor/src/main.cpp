#include "iostream"
#include <format>

#include "imguiLayouts.h"

#include "imguizmo/imguizmoquat.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#define IMGUI_IMPLEMENTATION
#include "imgui/imgui.h"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/matrix_decompose.hpp>

#define SDL_MAIN_HANDLED
#include "MKCore.h"
#include "MKGraphics.h"
#include "MKUtils.h"
#include "graphicsUtils.h"
//#include "scene/scene.h"
#include "minecraft/blockMapComponent.h"
#include "minecraft/chunk.h"
#include "minecraft/blocks.h"
#include "perlinNoise/PerlinNoise.hpp"
#include "physics/physics.h"

using namespace MKEngine;
using namespace MKGraphics;

mesh* _mesh;
shader* sh;
texture* tex;
camera* cam;
glm::mat4 model = glm::mat4(1.0);
//MKGame::blocks* blocks;

MKEngine::scene* _scene;
MKGame::blockMapComponent* blockMap;

int windowWidth;
int windowHeight;

void SetupImGuiStyle(bool bStyleDark_, float alpha_);

//std::string solutionDir = "C:\\Users\\Mink\\source\\repos\\MKFramework\\";
std::string solutionDir = "C:\\Users\\Danila\\Documents\\Github\\MKFramework\\";

int mouseLockPosX;
int mouseLockPosY;

float camFov = 75.0f;


#define RUN_SPEED_MUL 1.5f
#define DEFAULT_PLAYER_SPEED 4.0f
#define DEFAULT_AIR_DAMPING 0.1f
#define PLAYER_NOT_ONGROUND_DAMPING 10.0f

MKGame::Hitbox* player;

cubeMap* sky;
shader* skyboxShader;

void cameraHandleInput(window* wnd, float deltaTime) {
	float speed = DEFAULT_PLAYER_SPEED;

	bool sprint = Input::getKey(SDL_SCANCODE_LSHIFT);

	if (sprint) {
		camFov = std::lerp(camFov, 90.0f, deltaTime * 10.0f);
		speed *= RUN_SPEED_MUL;
	}
	else {
		camFov = std::lerp(camFov, 75.0f, deltaTime * 20.0f);
	}

	auto dirForward = cam->Orientation;
	dirForward.y = 0;
	dirForward = glm::normalize(dirForward);
	auto dirRight = glm::normalize(glm::cross(cam->Orientation, cam->Up));

	int substeps = (int)(deltaTime * 1000);
	substeps = (substeps <= 0 ? 1 : (substeps > 100 ? 100 : substeps));

	MKGame::physStep(deltaTime, substeps, blockMap, player);

	auto playerPos = player->pos;

	cam->Position = playerPos + glm::vec3(0,player->halfSize.y,0);
	blockMap->drawCenter = glm::vec3(playerPos.x/CHUNK_W, 0, playerPos.z / CHUNK_D);
	

	glm::vec3 dir(0, 0, 0);
	if (Input::getKey(SDL_SCANCODE_A)) {
		dir.x -= dirRight.x;
		dir.z -= dirRight.z;
	}
	if (Input::getKey(SDL_SCANCODE_D)) {
		dir.x += dirRight.x;
		dir.z += dirRight.z;
	}
	if (Input::getKey(SDL_SCANCODE_W)) {
		dir.x += dirForward.x;
		dir.z += dirForward.z;
 	}
	if (Input::getKey(SDL_SCANCODE_S)) {
		dir.x -= dirForward.x;
		dir.z -= dirForward.z;
	}
	if (Input::getKey(SDL_SCANCODE_SPACE) && player->isGrounded)
	{
		player->velocity.y += 7;
	}
	player->linear_damp = DEFAULT_AIR_DAMPING;


	if (length(dir) > 0.0f) {
		dir = normalize(dir);

		if (!player->isGrounded)
			player->linear_damp = PLAYER_NOT_ONGROUND_DAMPING;

		player->velocity.x += dir.x * speed * deltaTime * 9;
		player->velocity.z += dir.z * speed * deltaTime * 9;
	}

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
	cam->updateMatrix(camFov, 0.1f, 100.0f, (float)windowWidth/windowHeight);
}

const float timeStep = 1.0f / 60.0f;

void winUpdateCallback(window* wnd, float deltaTime) 
{
	wnd->getSize(&windowWidth, &windowHeight);

	_scene->update();

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


void winRenderCallback(window* wnd, float deltaTime) 
{
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	clearColor(75/255.0,159/255.0,220/255.0,1);
	setViewport(0, 0, wnd->getWidth(), wnd->getHeight());
	
	sh->use();
	sh->setMat4("camMatrix", cam->Matrix);
	sh->setMat4("model", model);
	sh->setVec4("lightColor", glm::vec4(1.0,0.5,0.4,1.0));
	sh->setVec3("camPos", cam->Position);

	_scene->render();

	MKEngine::Utils::drawQuad(glm::vec3(3,9,2), glm::vec3(0,0,0), glm::vec2(1, 1), glm::vec4(1, 0, 0, 1), cam);
	MKEngine::Utils::drawLine(player->pos, glm::vec3(3, 9, 2), glm::vec4(0, 1, 0, 1), cam);

	sky->draw(*skyboxShader, *cam);
	
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

	MKEngine::utilsExample1();
	MKGraphics::enable(GL_DEPTH_TEST);
	MKGraphics::enable(GL_BLEND);
	MKGraphics::blendEnable();

	std::string resDir = solutionDir + "resources\\";

	sh = new shader(
		(resDir + "shaders\\diffuse.vert").c_str(),
		(resDir + "shaders\\diffuse.frag").c_str());

	tex = new texture((resDir + "textures\\common\\dirt.png").c_str(), "diffuse", 0, GL_UNSIGNED_BYTE);

	cam = new camera(glm::vec3(3, 11, 3));
	

	MKGame::blocks::setTex(new texture((resDir + "textures\\blocks.png").c_str(), "diffuse", 0, GL_UNSIGNED_BYTE));
	MKGame::blocks::registerData(new MKGame::dirt());
	MKGame::blocks::registerData(new MKGame::grass());

	_scene = new scene();

	_scene->cam = cam;

	auto blockMapEnt = _scene->createEntity("Block map");
	blockMap = new MKGame::blockMapComponent(blockMapEnt);
	blockMapEnt->components.push_back(blockMap);

	player = new MKGame::Hitbox(glm::vec3(3, 11, 3), glm::vec3(0.2f, 0.9f, 0.2f));

	float counter = 0;
	float rot = 45.0f;

	const siv::PerlinNoise::seed_type seed = 12345u;
	const siv::PerlinNoise perlin{ seed };

	for (int i = -128; i < 128; i++)
	{
		for (int j = -128; j < 128; j++)
		{
			const double noise = perlin.octave2D_01((i * 0.01), (j * 0.01), 16, 0.6) * 10;

			int maxH = static_cast<int>(noise);

			for (int h = 0; h <= maxH; h++)
			{
				if (h == maxH) {
					blockMap->setTile(glm::ivec3(i, h, j), 2);
				}
				else {
					blockMap->setTile(glm::ivec3(i, h, j), 1);
				}
			}
		}
	}

	std::vector<std::string> faces
	{
			resDir+"textures\\skybox\\Cold Sunset\\left+x.png",
			resDir+"textures\\skybox\\Cold Sunset\\right-x.png",
			resDir+"textures\\skybox\\Cold Sunset\\up+y.png",
			resDir+"textures\\skybox\\Cold Sunset\\down-y.png",
			resDir+"textures\\skybox\\Cold Sunset\\front+z.png",
			resDir+"textures\\skybox\\Cold Sunset\\back-z.png"
	};

	sky = new cubeMap(faces);
	skyboxShader = new shader((resDir+ "shaders\\skybox.vert").c_str(), (resDir+ "shaders\\skybox.frag").c_str());
	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);
	skyboxShader->useDefault();

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