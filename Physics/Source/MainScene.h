#pragma once
#include "Entity.h"
#include "Scene.h"
#include "App.h"
#include <sstream>
#include "Shader.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Utility.h"
#include <chrono>

class MainScene final: public Scene{
	static const float G;
	static const float engineThrust;
	static std::vector<Entity*> buddyPool;
	enum class MESH_TYPE{
		TEXT,
		MENU_BG,
		GAME_BG,
		WIN_LOSE_BG,
		LOGO,
		HP_BAR_RED,
		HP_BAR_GREEN,
		RETICLE,
		PLAYER,
		ASTEROID,
		ENEMY_STATIC,
		ENEMY_DYNAMIC,
		BUDDY,
		RED_ORB,
		ORANGE_ORB,
		PLAYER_BULLET,
		ENEMY_BULLET,
		HOMING_BULLET,
		BLACK_HOLE,
		WHITE_HOLE,
		PLANET,
		COMET_PARTICLE_SOLID,
		COMET_PARTICLE_HOLLOW,
		FIREWORK_PARTICLE,
		AMT
	};
public:
	bool showMainMenu;
	bool showWinLose;
	short lvl;
	short activeCount;
	short asteroidCount;
	short buddyCount;
	short enemyCount;
	float FPS;
	float worldWidth;
	float worldHeight;
	float gameSpd;
	float enterBT;
	float levelSelectBT;
	float spawnAsteroidBT;
	float spawnBuddyBT;
	float spawnPlayerBulletBT;
	float spawnStaticEnemyBulletBT;
	float spawnDynamicEnemyBulletBT;
	float spawnPlayerHomingBulletBT;
	float spawnStaticEnemyHomingBulletBT;
	float spawnDynamicEnemyHomingBulletBT;
	float spawnPapaAsteroidBT;
	float spawnEnemyStaticBT;
	float spawnEnemyDynamicBT;
	float spawnOrbBT;
	int score;
	MainScene() = default;
	void Init();
	void Update(const float& dt);
	void Render();
	void Exit();
	Entity* const& GetInactive();
	Entity* player;
	Entity* planet;
	Entity* blackHole;
	Entity* whiteHole;
	void InitGame();
	void InitMeshes();
	void UpdateLvl1();
	void UpdateLvl2();
	void UpdateLvl3();
	void UpdateMainMenu(const float& dt);
	void UpdateWinLose(const float& dt);
	void RenderTextOnScreen(const Mesh* const& mesh, const std::ostringstream& ss, const Color& colour, const float& size, const float& x, const float& y);
	void RenderMesh(const Mesh* const& mesh, const bool& useCustom = false, const Color& colour = Color(), const float& alpha = 1.f);
	void RenderMainMenu();
	void RenderWinLose(const bool& win);
	void SpawnAsteroid(bool&& papa);
	void SpawnMultipleAsteroids(const Vector3& papaPos, short&& qty);
	unsigned VAO;
	unsigned shaderProgID;
	Mesh* meshList[(uint)MESH_TYPE::AMT];
	MS modelStack;
	MS viewStack;
	MS projectionStack;
};