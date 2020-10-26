#include "MainScene.h"
#include "LoadTGA.h"
#include <map>
#include <sstream>

extern float elapsedTime;
const float MainScene::G = 999999.f;
const float MainScene::engineThrust = 1000.f;
std::vector<Entity*> MainScene::buddyPool;

static Vector3 RotateVec(const Vector3& vec, const float& angleInRad){
	return Vector3(vec.x * cos(angleInRad) + vec.y * -sin(angleInRad), vec.x * sin(angleInRad) + vec.y * cos(angleInRad), 0.f);
}

void MainScene::InitGame(){
	for(Entity* const& entity: Entity::entityPool){
		entity->active = false;
	}
	showMainMenu = true;
	showWinLose = false;
	blackHole = whiteHole = nullptr;
	lvl = 1;
	activeCount = asteroidCount = buddyCount = enemyCount = 0;
	score = 0;
	gameSpd = enterBT = levelSelectBT = spawnAsteroidBT = spawnBuddyBT = 0.f;
	spawnPlayerBulletBT = spawnStaticEnemyBulletBT = spawnDynamicEnemyBulletBT = spawnPlayerHomingBulletBT = spawnStaticEnemyHomingBulletBT = spawnDynamicEnemyHomingBulletBT = 0.f;
	spawnPapaAsteroidBT = spawnEnemyStaticBT = spawnEnemyDynamicBT = spawnOrbBT = 0.f;

	player = Entity::entityPool[0]; {
		player->type = Entity::TYPE::PLAYER;
		player->active = true;
		player->maxHP = 200;
		player->HP = player->maxHP;
		player->scale.Set(worldHeight / 20.f, worldHeight / 20.f, 1.f);

		player->pos.Set(worldWidth / 2.f, worldHeight / 2.f, 0.f);
		player->vel.SetZero();
		player->mass = 5.f;
		player->force.SetZero();

		player->facingDir.Set(1.f, 0.f, 0.f);
		player->angularVel = 0.f;
		player->angularMass = 2.f / 5.f * player->mass * player->scale.x * player->scale.x;
		player->torque.SetZero();
	}
	planet = Entity::entityPool[1]; {
		planet->type = Entity::TYPE::PLANET;
		planet->active = true;
		planet->maxHP = 0;
		planet->HP = planet->maxHP;
		planet->scale.Set(worldHeight / 5.f, worldHeight / 5.f, 1.f);

		if(~rand() & 1){
			planet->pos.x = Math::RandFloatMinMax(planet->scale.x, worldWidth - planet->scale.x);
			planet->pos.y = rand() & 1 ? Math::RandFloatMinMax(planet->scale.y, worldHeight / 2.f - player->scale.y - planet->scale.y) : Math::RandFloatMinMax(worldHeight / 2.f + player->scale.y + planet->scale.y, worldHeight - planet->scale.y);
		} else{
			planet->pos.x = rand() & 1 ? Math::RandFloatMinMax(planet->scale.x, worldWidth / 2.f - player->scale.x - planet->scale.x) : Math::RandFloatMinMax(worldWidth / 2.f + player->scale.x + planet->scale.x, worldWidth - planet->scale.x);
			planet->pos.y = Math::RandFloatMinMax(planet->scale.y, worldHeight - planet->scale.y);
		}
		planet->vel.SetZero();
		planet->mass = 1000.f;
		planet->force.SetZero();

		planet->facingDir.SetZero();
		planet->angularVel = 0.f;
		planet->angularMass = 2.f / 5.f * planet->mass * planet->scale.x * planet->scale.x;
		planet->torque.SetZero();
	}
	blackHole = Entity::entityPool[2]; {
		blackHole->type = Entity::TYPE::BLACK_HOLE;
		blackHole->active = false;
		blackHole->maxHP = 0;
		blackHole->HP = blackHole->maxHP;
		blackHole->scale.Set(worldHeight / 40.f, worldHeight / 40.f, 1.f);

		blackHole->pos = player->pos;
		blackHole->vel = player->facingDir * 50.f;
		blackHole->mass = 100.f;
		blackHole->force.SetZero();

		blackHole->facingDir.SetZero();
		blackHole->angularVel = 0.f;
		blackHole->angularMass = 2.f / 5.f * blackHole->mass * blackHole->scale.x * blackHole->scale.x;
		blackHole->torque.SetZero();
	}
	whiteHole = Entity::entityPool[3]; {
		whiteHole->type = Entity::TYPE::WHITE_HOLE;
		whiteHole->active = false;
		whiteHole->maxHP = 0;
		whiteHole->HP = whiteHole->maxHP;
		whiteHole->scale.Set(worldHeight / 40.f, worldHeight / 40.f, 1.f);

		whiteHole->pos = player->pos;
		whiteHole->vel = player->facingDir * 100.f;
		whiteHole->mass = 30.f;
		whiteHole->force.SetZero();

		whiteHole->facingDir.SetZero();
		whiteHole->angularVel = 0.f;
		whiteHole->angularMass = 2.f / 5.f * whiteHole->mass * whiteHole->scale.x * whiteHole->scale.x;
		whiteHole->torque.SetZero();
	}
}

void MainScene::InitMeshes(){
	meshList[(uint)MESH_TYPE::TEXT] = MeshBuilder::GenText(16, 16);
	meshList[(uint)MESH_TYPE::TEXT]->textureID = LoadTGA("Textures/FontOnScreen.tga");

	meshList[(uint)MESH_TYPE::MENU_BG] = MeshBuilder::GenQuad(Color(), 16.f, 9.f);
	meshList[(uint)MESH_TYPE::MENU_BG]->textureID = LoadTGA("Textures/MenuBG.tga");
	meshList[(uint)MESH_TYPE::GAME_BG] = MeshBuilder::GenQuad(Color(), 16.f, 9.f);
	meshList[(uint)MESH_TYPE::GAME_BG]->textureID = LoadTGA("Textures/GameBG.tga");
	meshList[(uint)MESH_TYPE::WIN_LOSE_BG] = MeshBuilder::GenQuad(Color(), 16.f, 9.f);
	meshList[(uint)MESH_TYPE::WIN_LOSE_BG]->textureID = LoadTGA("Textures/WinLoseBG.tga");

	meshList[(uint)MESH_TYPE::LOGO] = MeshBuilder::GenQuad(Color(), 2.f, 2.f);
	meshList[(uint)MESH_TYPE::LOGO]->textureID = LoadTGA("Textures/Logo.tga");

	meshList[(uint)MESH_TYPE::HP_BAR_RED] = MeshBuilder::GenQuad(Color(.5f, 0.f, 0.f), 5.f, 1.f);
	meshList[(uint)MESH_TYPE::HP_BAR_GREEN] = MeshBuilder::GenQuad(Color(0.f, .75f, 0.f), 5.f, 1.f);

	meshList[(uint)MESH_TYPE::RETICLE] = MeshBuilder::GenQuad(Color(), 1.f, 1.f);
	meshList[(uint)MESH_TYPE::RETICLE]->textureID = LoadTGA("Textures/Reticle.tga");

	meshList[(uint)MESH_TYPE::PLAYER] = MeshBuilder::GenQuad(Color(), 2.f, 2.f);
	meshList[(uint)MESH_TYPE::PLAYER]->textureID = LoadTGA("Textures/Player.tga");

	meshList[(uint)MESH_TYPE::ASTEROID] = MeshBuilder::GenQuad(Color(1.f, .5f, .5f), 2.f, 2.f);
	meshList[(uint)MESH_TYPE::ASTEROID]->textureID = LoadTGA("Textures/Asteroid.tga");

	meshList[(uint)MESH_TYPE::ENEMY_STATIC] = MeshBuilder::GenQuad(Color(1.f, .3f, .3f), 2.f, 2.f);
	meshList[(uint)MESH_TYPE::ENEMY_STATIC]->textureID = LoadTGA("Textures/EnemyStatic.tga");
	meshList[(uint)MESH_TYPE::ENEMY_DYNAMIC] = MeshBuilder::GenQuad(Color(1.f, .3f, .3f), 2.f, 2.f);
	meshList[(uint)MESH_TYPE::ENEMY_DYNAMIC]->textureID = LoadTGA("Textures/EnemyDynamic.tga");

	meshList[(uint)MESH_TYPE::BUDDY] = MeshBuilder::GenQuad(Color(0.f, 1.f, 1.f), 2.f, 2.f);
	meshList[(uint)MESH_TYPE::BUDDY]->textureID = LoadTGA("Textures/Buddy.tga");

	meshList[(uint)MESH_TYPE::RED_ORB] = MeshBuilder::GenCircle(Color(1.f, 0.f, 0.f), 30, 1.f);
	meshList[(uint)MESH_TYPE::ORANGE_ORB] = MeshBuilder::GenCircle(Color(1.f, .5f, 0.f), 30, 1.f);

	meshList[(uint)MESH_TYPE::PLAYER_BULLET] = MeshBuilder::GenCircle(Color(1.f, 0.f, 1.f), 30, 1.f);
	meshList[(uint)MESH_TYPE::ENEMY_BULLET] = MeshBuilder::GenCircle(Color(1.f, 0.f, 0.f), 30, 1.f);
	meshList[(uint)MESH_TYPE::PLAYER_BULLET]->textureID = meshList[(uint)MESH_TYPE::ENEMY_BULLET]->textureID = LoadTGA("Textures/Bullet.tga");
	meshList[(uint)MESH_TYPE::HOMING_BULLET] = MeshBuilder::GenCircle(Color(1.f, 0.f, 1.f), 30, 1.f);
	meshList[(uint)MESH_TYPE::HOMING_BULLET]->textureID = LoadTGA("Textures/HomingBullet.tga");

	meshList[(uint)MESH_TYPE::BLACK_HOLE] = MeshBuilder::GenCircle(Color(0.f, 0.f, 0.f), 30, 1.f);
	meshList[(uint)MESH_TYPE::WHITE_HOLE] = MeshBuilder::GenCircle(Color(), 30, 1.f);

	meshList[(uint)MESH_TYPE::PLANET] = MeshBuilder::GenCircle(Color(.8f, 0.f, 0.f), 30, 1.f);
	meshList[(uint)MESH_TYPE::PLANET]->textureID = LoadTGA("Textures/Planet.tga");

	meshList[(uint)MESH_TYPE::COMET_PARTICLE_SOLID] = MeshBuilder::GenCircle(Color(), 30, 1.f);
	meshList[(uint)MESH_TYPE::COMET_PARTICLE_SOLID]->textureID = LoadTGA("Textures/CometParticleSolid.tga");
	meshList[(uint)MESH_TYPE::COMET_PARTICLE_HOLLOW] = MeshBuilder::GenCircle(Color(), 30, 1.f);
	meshList[(uint)MESH_TYPE::COMET_PARTICLE_HOLLOW]->textureID = LoadTGA("Textures/CometParticleHollow.tga");
	meshList[(uint)MESH_TYPE::FIREWORK_PARTICLE] = MeshBuilder::GenCircle(Color(), 30, 1.f);
}

void MainScene::Init(){
	Math::InitRNG();
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	shaderProgID = LoadShaders("Shaders/Physics.vs", "Shaders/Physics.fs");
	glUseProgram(shaderProgID);

	for(short i = 0; i < 204; ++i){
		Entity::entityPool.emplace_back(new Entity);
	}
	for(short i = 0; i < 200; ++i){
		Entity* const&& cParticle = new Entity;
		cParticle->spawnTime = 0.f;
		cParticle->colour = Color();
		cParticle->type = Entity::TYPE::COMET_PARTICLE;
		cParticle->active = false;
		cParticle->maxHP = 0;
		cParticle->HP = cParticle->maxHP;
		cParticle->scale.Set(worldHeight / 20.f, worldHeight / 20.f, 1.f);

		cParticle->pos.SetZero();
		cParticle->vel.SetZero();
		cParticle->mass = 5.f;
		cParticle->force.SetZero();

		cParticle->facingDir.SetZero();
		cParticle->angularVel = 0.f;
		cParticle->angularMass = 2.f / 5.f * cParticle->mass * cParticle->scale.x * cParticle->scale.x;
		cParticle->torque.SetZero();
		Entity::cometParticlePool.emplace_back(cParticle);
	}
	for(short i = 0; i < 1000; ++i){
		Entity* const&& fParticle = new Entity;
		fParticle->spawnTime = 0.f;
		fParticle->colour = Color();
		fParticle->type = Entity::TYPE::FIREWORK_PARTICLE;
		fParticle->active = false;
		fParticle->maxHP = 0;
		fParticle->HP = fParticle->maxHP;
		fParticle->scale.Set(worldHeight / 80.f, worldHeight / 80.f, 1.f);

		fParticle->pos.SetZero();
		fParticle->vel.SetZero();
		fParticle->mass = 5.f;
		fParticle->force.SetZero();

		fParticle->facingDir.SetZero();
		fParticle->angularVel = 0.f;
		fParticle->angularMass = 2.f / 5.f * fParticle->mass * fParticle->scale.x * fParticle->scale.x;
		fParticle->torque.SetZero();
		Entity::fireworkParticlePool.emplace_back(fParticle);
	}
	InitGame();
	InitMeshes();
}

void MainScene::UpdateLvl1(){
	///Asteroids
	if(spawnAsteroidBT <= elapsedTime){
		SpawnAsteroid(false);
		spawnAsteroidBT = elapsedTime + 2.f;
	}
	if(spawnPapaAsteroidBT <= elapsedTime){
		SpawnAsteroid(true);
		spawnPapaAsteroidBT = elapsedTime + 10.f;
	}

	///Static enemies
	if(spawnEnemyStaticBT <= elapsedTime){
		Entity* const& enemyS = GetInactive(); {
			enemyS->type = Entity::TYPE::ENEMY_STATIC;
			enemyS->active = true;
			enemyS->maxHP = 10;
			enemyS->HP = enemyS->maxHP;
			enemyS->scale.Set(worldHeight / 28.8f, worldHeight / 28.8f, 1.f);

			if(rand() & 1){
				enemyS->pos.x = rand() % 2 ? -enemyS->scale.x : worldWidth + enemyS->scale.x;
				enemyS->pos.y = Math::RandFloatMinMax(-enemyS->scale.y, worldHeight + enemyS->scale.y);
			} else{
				enemyS->pos.x = Math::RandFloatMinMax(-enemyS->scale.x, worldWidth + enemyS->scale.x);
				enemyS->pos.y = rand() % 2 ? -enemyS->scale.y : worldHeight + enemyS->scale.y;
			}
			enemyS->vel.SetZero();
			enemyS->mass = 5.f;
			enemyS->force = (player->pos - enemyS->pos).Normalized() * 30.f;

			enemyS->facingDir = (player->pos - enemyS->pos).Normalized();
			enemyS->angularVel = 0.f;
			enemyS->angularMass = 2.f / 5.f * enemyS->mass * enemyS->scale.x * enemyS->scale.x;
			enemyS->torque.SetZero();
		}
		spawnEnemyStaticBT = elapsedTime + 10.f;
	}

	///Dynamic enemies
	if(spawnEnemyDynamicBT <= elapsedTime){
		Entity* const& enemyD = GetInactive(); {
			enemyD->type = Entity::TYPE::ENEMY_DYNAMIC;
			enemyD->active = true;
			enemyD->maxHP = 10;
			enemyD->HP = enemyD->maxHP;
			enemyD->scale.Set(worldHeight / 20.f, worldHeight / 20.f, 1.f);

			if(rand() & 1){
				enemyD->pos.x = rand() % 2 ? -enemyD->scale.x : worldWidth + enemyD->scale.x;
				enemyD->pos.y = Math::RandFloatMinMax(-enemyD->scale.y, worldHeight + enemyD->scale.y);
			} else{
				enemyD->pos.x = Math::RandFloatMinMax(-enemyD->scale.x, worldWidth + enemyD->scale.x);
				enemyD->pos.y = rand() % 2 ? -enemyD->scale.y : worldHeight + enemyD->scale.y;
			}
			enemyD->vel.SetZero();
			enemyD->mass = 5.f;
			enemyD->force = (player->pos - enemyD->pos).Normalized() * 50.f;

			enemyD->facingDir = (player->pos - enemyD->pos).Normalized();
			enemyD->angularVel = 0.f;
			enemyD->angularMass = 2.f / 5.f * enemyD->mass * enemyD->scale.x * enemyD->scale.x;
			enemyD->torque.SetZero();
		}
		spawnEnemyDynamicBT = elapsedTime + 10.f;
	}

	///Orbs
	if(spawnOrbBT <= elapsedTime){
		Entity* const& orb = GetInactive(); {
			orb->type = ~rand() & 1 ? Entity::TYPE::RED_ORB : Entity::TYPE::ORANGE_ORB;
			orb->active = true;
			orb->maxHP = 0;
			orb->HP = orb->maxHP;
			orb->scale.Set(worldHeight / 100.f, worldHeight / 100.f, 1.f);

			if(~rand() & 1){
				orb->pos.x = Math::RandFloatMinMax(orb->scale.x, worldWidth - orb->scale.x);
				orb->pos.y = rand() & 1 ? Math::RandFloatMinMax(orb->scale.y, worldHeight / 2.f - player->scale.y - orb->scale.y) : Math::RandFloatMinMax(worldHeight / 2.f + player->scale.y + orb->scale.y, worldHeight - orb->scale.y);
			} else{
				orb->pos.x = rand() & 1 ? Math::RandFloatMinMax(orb->scale.x, worldWidth / 2.f - player->scale.x - orb->scale.x) : Math::RandFloatMinMax(worldWidth / 2.f + player->scale.x + orb->scale.x, worldWidth - orb->scale.x);
				orb->pos.y = Math::RandFloatMinMax(orb->scale.y, worldHeight - orb->scale.y);
			}
			orb->vel.SetZero();
			orb->mass = .5f;
			orb->force.SetZero();

			orb->facingDir.SetZero();
			orb->angularVel = 0.f;
			orb->angularMass = 2.f / 5.f * orb->mass * orb->scale.x * orb->scale.x;
			orb->torque.SetZero();
		}
		spawnOrbBT = elapsedTime + 5.f;
	}
}

void MainScene::UpdateLvl2(){
	///Asteroids
	if(spawnAsteroidBT <= elapsedTime){
		SpawnAsteroid(false);
		spawnAsteroidBT = elapsedTime + 1.f;
	}
	if(spawnPapaAsteroidBT <= elapsedTime){
		SpawnAsteroid(true);
		spawnPapaAsteroidBT = elapsedTime + 5.f;
	}

	///Static enemies
	if(spawnEnemyStaticBT <= elapsedTime){
		Entity* const& enemyS = GetInactive(); {
			enemyS->type = Entity::TYPE::ENEMY_STATIC;
			enemyS->active = true;
			enemyS->maxHP = 10;
			enemyS->HP = enemyS->maxHP;
			enemyS->scale.Set(worldHeight / 28.8f, worldHeight / 28.8f, 1.f);

			if(rand() & 1){
				enemyS->pos.x = rand() % 2 ? -enemyS->scale.x : worldWidth + enemyS->scale.x;
				enemyS->pos.y = Math::RandFloatMinMax(-enemyS->scale.y, worldHeight + enemyS->scale.y);
			} else{
				enemyS->pos.x = Math::RandFloatMinMax(-enemyS->scale.x, worldWidth + enemyS->scale.x);
				enemyS->pos.y = rand() % 2 ? -enemyS->scale.y : worldHeight + enemyS->scale.y;
			}
			enemyS->vel.SetZero();
			enemyS->mass = 5.f;
			enemyS->force = (player->pos - enemyS->pos).Normalized() * 30.f;

			enemyS->facingDir = (player->pos - enemyS->pos).Normalized();
			enemyS->angularVel = 0.f;
			enemyS->angularMass = 2.f / 5.f * enemyS->mass * enemyS->scale.x * enemyS->scale.x;
			enemyS->torque.SetZero();
		}
		spawnEnemyStaticBT = elapsedTime + 5.f;
	}

	///Dynamic enemies
	if(spawnEnemyDynamicBT <= elapsedTime){
		Entity* const& enemyD = GetInactive(); {
			enemyD->type = Entity::TYPE::ENEMY_DYNAMIC;
			enemyD->active = true;
			enemyD->maxHP = 10;
			enemyD->HP = enemyD->maxHP;
			enemyD->scale.Set(worldHeight / 20.f, worldHeight / 20.f, 1.f);

			if(rand() & 1){
				enemyD->pos.x = rand() % 2 ? -enemyD->scale.x : worldWidth + enemyD->scale.x;
				enemyD->pos.y = Math::RandFloatMinMax(-enemyD->scale.y, worldHeight + enemyD->scale.y);
			} else{
				enemyD->pos.x = Math::RandFloatMinMax(-enemyD->scale.x, worldWidth + enemyD->scale.x);
				enemyD->pos.y = rand() % 2 ? -enemyD->scale.y : worldHeight + enemyD->scale.y;
			}
			enemyD->vel.SetZero();
			enemyD->mass = 5.f;
			enemyD->force = (player->pos - enemyD->pos).Normalized() * 50.f;

			enemyD->facingDir = (player->pos - enemyD->pos).Normalized();
			enemyD->angularVel = 0.f;
			enemyD->angularMass = 2.f / 5.f * enemyD->mass * enemyD->scale.x * enemyD->scale.x;
			enemyD->torque.SetZero();
		}
		spawnEnemyDynamicBT = elapsedTime + 5.f;
	}

	///Orbs
	if(spawnOrbBT <= elapsedTime){
		Entity* const& orb = GetInactive(); {
			orb->type = ~rand() & 1 ? Entity::TYPE::RED_ORB : Entity::TYPE::ORANGE_ORB;
			orb->active = true;
			orb->maxHP = 0;
			orb->HP = orb->maxHP;
			orb->scale.Set(worldHeight / 100.f, worldHeight / 100.f, 1.f);

			if(~rand() & 1){
				orb->pos.x = Math::RandFloatMinMax(orb->scale.x, worldWidth - orb->scale.x);
				orb->pos.y = rand() & 1 ? Math::RandFloatMinMax(orb->scale.y, worldHeight / 2.f - player->scale.y - orb->scale.y) : Math::RandFloatMinMax(worldHeight / 2.f + player->scale.y + orb->scale.y, worldHeight - orb->scale.y);
			} else{
				orb->pos.x = rand() & 1 ? Math::RandFloatMinMax(orb->scale.x, worldWidth / 2.f - player->scale.x - orb->scale.x) : Math::RandFloatMinMax(worldWidth / 2.f + player->scale.x + orb->scale.x, worldWidth - orb->scale.x);
				orb->pos.y = Math::RandFloatMinMax(orb->scale.y, worldHeight - orb->scale.y);
			}
			orb->vel.SetZero();
			orb->mass = .5f;
			orb->force.SetZero();

			orb->facingDir.SetZero();
			orb->angularVel = 0.f;
			orb->angularMass = 2.f / 5.f * orb->mass * orb->scale.x * orb->scale.x;
			orb->torque.SetZero();
		}
		spawnOrbBT = elapsedTime + 10.f;
	}
}

void MainScene::UpdateLvl3(){
	///Asteroids
	if(spawnAsteroidBT <= elapsedTime){
		SpawnAsteroid(false);
		spawnAsteroidBT = elapsedTime + .5f;
	}
	if(spawnPapaAsteroidBT <= elapsedTime){
		SpawnAsteroid(true);
		spawnPapaAsteroidBT = elapsedTime + 3.f;
	}

	///Static enemies
	if(spawnEnemyStaticBT <= elapsedTime){
		Entity* const& enemyS = GetInactive(); {
			enemyS->type = Entity::TYPE::ENEMY_STATIC;
			enemyS->active = true;
			enemyS->maxHP = 10;
			enemyS->HP = enemyS->maxHP;
			enemyS->scale.Set(worldHeight / 28.8f, worldHeight / 28.8f, 1.f);

			if(rand() & 1){
				enemyS->pos.x = rand() % 2 ? -enemyS->scale.x : worldWidth + enemyS->scale.x;
				enemyS->pos.y = Math::RandFloatMinMax(-enemyS->scale.y, worldHeight + enemyS->scale.y);
			} else{
				enemyS->pos.x = Math::RandFloatMinMax(-enemyS->scale.x, worldWidth + enemyS->scale.x);
				enemyS->pos.y = rand() % 2 ? -enemyS->scale.y : worldHeight + enemyS->scale.y;
			}
			enemyS->vel.SetZero();
			enemyS->mass = 5.f;
			enemyS->force = (player->pos - enemyS->pos).Normalized() * 30.f;

			enemyS->facingDir = (player->pos - enemyS->pos).Normalized();
			enemyS->angularVel = 0.f;
			enemyS->angularMass = 2.f / 5.f * enemyS->mass * enemyS->scale.x * enemyS->scale.x;
			enemyS->torque.SetZero();
		}
		spawnEnemyStaticBT = elapsedTime + 3.f;
	}

	///Dynamic enemies
	if(spawnEnemyDynamicBT <= elapsedTime){
		Entity* const& enemyD = GetInactive(); {
			enemyD->type = Entity::TYPE::ENEMY_DYNAMIC;
			enemyD->active = true;
			enemyD->maxHP = 10;
			enemyD->HP = enemyD->maxHP;
			enemyD->scale.Set(worldHeight / 20.f, worldHeight / 20.f, 1.f);

			if(rand() & 1){
				enemyD->pos.x = rand() % 2 ? -enemyD->scale.x : worldWidth + enemyD->scale.x;
				enemyD->pos.y = Math::RandFloatMinMax(-enemyD->scale.y, worldHeight + enemyD->scale.y);
			} else{
				enemyD->pos.x = Math::RandFloatMinMax(-enemyD->scale.x, worldWidth + enemyD->scale.x);
				enemyD->pos.y = rand() % 2 ? -enemyD->scale.y : worldHeight + enemyD->scale.y;
			}
			enemyD->vel.SetZero();
			enemyD->mass = 5.f;
			enemyD->force = (player->pos - enemyD->pos).Normalized() * 50.f;

			enemyD->facingDir = (player->pos - enemyD->pos).Normalized();
			enemyD->angularVel = 0.f;
			enemyD->angularMass = 2.f / 5.f * enemyD->mass * enemyD->scale.x * enemyD->scale.x;
			enemyD->torque.SetZero();
		}
		spawnEnemyDynamicBT = elapsedTime + 3.f;
	}
}

void MainScene::Update(const float& dt){
	FPS = 1.f / dt;
	if(App::IsKeyPressed(VK_RETURN) && enterBT <= elapsedTime){
		if(showMainMenu){
			showMainMenu = false;
			switch(lvl){
				case 1: gameSpd = worldHeight / 1080.f; break;
				case 2: gameSpd = worldHeight / 1080.f * 1.2f; break;
				default: gameSpd = worldHeight / 1080.f * 1.5f;
			}
		}
		if(showWinLose){
			showWinLose = false;
			showMainMenu = true;
			InitGame();
		}
		enterBT = elapsedTime + .5f;
	}
	if(showMainMenu){
		UpdateMainMenu(dt);
	}
	if(showWinLose){
		UpdateWinLose(dt);
	}

	if(!showMainMenu && !showWinLose){
		if(score >= (lvl == 1 ? 500 : (lvl == 2 ? 700 : 1000)) || player->HP <= 0){
			enterBT = elapsedTime + .5f;
			showWinLose = true;
		}

		///Player movement
		if(App::IsKeyPressed('W') || App::IsKeyPressed(VK_UP)){
			player->force = engineThrust * player->facingDir;
		}
		if(App::IsKeyPressed('S') || App::IsKeyPressed(VK_DOWN)){
			player->force = engineThrust * -player->facingDir;
		}
		if(App::IsKeyPressed('A') || App::IsKeyPressed(VK_LEFT)){
			player->force = engineThrust / 20.f * player->facingDir;
			player->torque.Set(0.f, 0.f, 4000.f);
		}
		if(App::IsKeyPressed('D') || App::IsKeyPressed(VK_RIGHT)){
			player->force = engineThrust / 20.f * player->facingDir;
			player->torque.Set(0.f, 0.f, -4000.f);
		}
		if(!App::IsKeyPressed('W') && !App::IsKeyPressed(VK_UP) && !App::IsKeyPressed('S') && !App::IsKeyPressed(VK_DOWN)){
			player->force.SetZero();
		}
		if(!App::IsKeyPressed('A') && !App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed('D') && !App::IsKeyPressed(VK_RIGHT)){ //Reverse thrusters to stabalise and calibrate player
			if(player->angularVel > 0.f){
				player->torque.Set(0.f, 0.f, -2000.f);
			}
			if(player->angularVel < 0.f){
				player->torque.Set(0.f, 0.f, 2000.f);
			}
		}

		if(spawnBuddyBT <= elapsedTime && buddyCount < 20){ //Buddies
			Entity* const& buddy = GetInactive(); {
				buddy->type = Entity::TYPE::BUDDY;
				buddy->active = true;
				buddy->maxHP = 3;
				buddy->HP = buddy->maxHP;
				buddy->scale.Set(worldHeight / 40.f, worldHeight / 40.f, 1.f);

				buddy->pos = player->pos - player->facingDir * 200.f;
				buddy->vel.SetZero();
				buddy->mass = 2.f;
				buddy->force.SetZero();

				buddy->facingDir.Set(1.f, 0.f, 0.f);
				buddy->angularVel = 0.f;
				buddy->angularMass = 2.f / 5.f * buddy->mass * buddy->scale.x * buddy->scale.x;
				buddy->torque.SetZero();
			}
			buddyPool.emplace_back(buddy);
			spawnBuddyBT = elapsedTime + .5f;
		}
		if(App::IsKeyPressed(VK_SPACE) && spawnPlayerBulletBT <= elapsedTime){ //Player Bullets
			Entity* const& pBullet = GetInactive(); {
				pBullet->type = Entity::TYPE::PLAYER_BULLET;
				pBullet->active = true;
				pBullet->maxHP = 1;
				pBullet->HP = pBullet->maxHP;
				pBullet->scale.Set(worldHeight / 50.f, worldHeight / 50.f, 1.f);

				pBullet->pos = player->pos;
				pBullet->vel = player->facingDir * 1000.f;
				pBullet->mass = .5f;
				pBullet->force.SetZero();

				pBullet->facingDir.SetZero();
				pBullet->angularVel = 0.f;
				pBullet->angularMass = 2.f / 5.f * pBullet->mass * pBullet->scale.x * pBullet->scale.x;
				pBullet->torque.SetZero();
			}
			player->force = 200.f * -player->facingDir; //Recoil
			spawnPlayerBulletBT = elapsedTime + .1f;
		}
		if(App::IsKeyPressed('H') && spawnPlayerHomingBulletBT <= elapsedTime){ //Homing Bullets
			Entity* const& homingBullet = GetInactive(); {
				homingBullet->target = nullptr;
				homingBullet->type = Entity::TYPE::HOMING_BULLET;
				homingBullet->active = true;
				homingBullet->maxHP = 1;
				homingBullet->HP = homingBullet->maxHP;
				homingBullet->scale.Set(worldHeight / 60.f, worldHeight / 60.f, 1.f);

				homingBullet->pos = player->pos;
				homingBullet->vel = player->facingDir * 300.f;
				homingBullet->mass = .5f;
				homingBullet->force.SetZero();

				homingBullet->facingDir = player->facingDir;
				homingBullet->angularVel = 0.f;
				homingBullet->angularMass = 2.f / 5.f * homingBullet->mass * homingBullet->scale.x * homingBullet->scale.x;
				homingBullet->torque.SetZero();
			}
			player->force = 500.f * -player->facingDir; //Recoil
			spawnPlayerHomingBulletBT = elapsedTime + .3f;
		}
		if(App::IsKeyPressed('O') && !blackHole->active){
			blackHole->type = Entity::TYPE::BLACK_HOLE;
			blackHole->active = true;
			blackHole->maxHP = 0;
			blackHole->HP = blackHole->maxHP;
			blackHole->scale.Set(worldHeight / 40.f, worldHeight / 40.f, 1.f);

			blackHole->pos = player->pos;
			blackHole->vel = player->facingDir * 50.f;
			blackHole->mass = 100.f;
			blackHole->force.SetZero();

			blackHole->facingDir.SetZero();
			blackHole->angularVel = 0.f;
			blackHole->angularMass = 2.f / 5.f * blackHole->mass * blackHole->scale.x * blackHole->scale.x;
			blackHole->torque.SetZero();
		}
		if(App::IsKeyPressed('P') && !whiteHole->active){
			whiteHole->type = Entity::TYPE::WHITE_HOLE;
			whiteHole->active = true;
			whiteHole->maxHP = 0;
			whiteHole->HP = whiteHole->maxHP;
			whiteHole->scale.Set(worldHeight / 40.f, worldHeight / 40.f, 1.f);

			whiteHole->pos = player->pos;
			whiteHole->vel = player->facingDir * 100.f;
			whiteHole->mass = 30.f;
			whiteHole->force.SetZero();

			whiteHole->facingDir.SetZero();
			whiteHole->angularVel = 0.f;
			whiteHole->angularMass = 2.f / 5.f * whiteHole->mass * whiteHole->scale.x * whiteHole->scale.x;
			whiteHole->torque.SetZero();
		}
		
		switch(lvl){
			case 1: UpdateLvl1(); break;
			case 2: UpdateLvl2(); break;
			default: UpdateLvl3();
		}
		activeCount = 0;
		asteroidCount = 0;
		buddyCount = 0;
		enemyCount = 0;

		for(Entity* const& entity: Entity::entityPool){
			if(entity->active){
				bool&& collided = false;
				if(entity->type == Entity::TYPE::PLAYER_BULLET){ //Detection and resolution of dist-based collisions with player bullets
					for(auto iter = Entity::entityPool.begin() + 4; iter != Entity::entityPool.end(); ++iter){
						Entity* const& instance = *iter;
						if(instance->active && instance->maxHP != 0 && instance->type != Entity::TYPE::PLAYER_BULLET &&
							instance->type != Entity::TYPE::BUDDY && (instance->pos - entity->pos).Length() <= entity->scale.x + instance->scale.x){
							--instance->HP;
							entity->active = false;
							collided = true;
							if(instance->HP <= 0 && instance->maxHP){
								if(instance->type == Entity::TYPE::ASTEROID){
									score += instance->maxHP > 10 ? 40 : 5;
								}
								if(instance->type == Entity::TYPE::ENEMY_STATIC){
									score += 20;
								}
								if(instance->type == Entity::TYPE::ENEMY_STATIC){
									score += 25;
								}
							}
						}
					}
				}
				if(entity->type == Entity::TYPE::ENEMY_BULLET){ //Detection and resolution of dist-based collisions with enemy bullets
					for(auto iter = Entity::entityPool.begin(); iter != Entity::entityPool.end(); ++iter){
						Entity* const& instance = *iter;
						if(instance->active && instance->maxHP != 0 && instance->type != Entity::TYPE::ENEMY_BULLET &&
							instance->type != Entity::TYPE::ENEMY_STATIC && instance->type != Entity::TYPE::ENEMY_DYNAMIC && (instance->pos - entity->pos).Length() <= entity->scale.x + instance->scale.x){
							--instance->HP;
							entity->active = false;
							collided = true;
						}
					}
				}
				if(entity->type == Entity::TYPE::HOMING_BULLET){
					if(entity->target && entity->target->active && (entity->target->type == Entity::TYPE::ASTEROID || entity->target->type == Entity::TYPE::ENEMY_STATIC || entity->target->type == Entity::TYPE::ENEMY_DYNAMIC)){
						const Vector3&& dir = entity->target->pos - entity->pos;
						Vector3 normal = (dir).Cross(entity->facingDir);
						const float&& dist = (dir).Length();
						if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON) && entity->facingDir != dir.Normalized()){
							entity->facingDir = RotateVec(entity->facingDir, Math::DegreeToRadian(normal.z < 0.f ? 1.f : -1.f)); //Turn CW or CCW to face target
						}
						entity->vel = entity->facingDir * dist; //Force based on dist to player
					} else{
						std::map<float, const Entity*> sorted;
						for(auto iter = Entity::entityPool.begin() + 4; iter != Entity::entityPool.end(); ++iter){
							const Entity* const& instance = *iter;
							if(instance->active && (instance->type == Entity::TYPE::ASTEROID || instance->type == Entity::TYPE::ENEMY_STATIC || instance->type == Entity::TYPE::ENEMY_DYNAMIC)){
								sorted[(instance->pos - entity->pos).Length()] = instance;
							}
						}
						entity->target = sorted.empty() ? nullptr : sorted.begin()->second;
					}

					///Detection and resolution of dist-based collisions with homing bullets
					for(auto iter = Entity::entityPool.begin() + 4; iter != Entity::entityPool.end(); ++iter){
						Entity* const& instance = *iter;
						if(instance->active && instance->maxHP != 0 && instance->type != Entity::TYPE::HOMING_BULLET &&
							instance->type != Entity::TYPE::BUDDY && (instance->pos - entity->pos).Length() <= entity->scale.x + instance->scale.x){
							--instance->HP;
							entity->active = false;
							collided = true;
						}
					}
				}
				if(!collided && entity->type == Entity::TYPE::ASTEROID || entity->type == Entity::TYPE::ENEMY_STATIC || entity->type == Entity::TYPE::ENEMY_DYNAMIC){ //More generic collisions
					for(Entity* const& instance : Entity::entityPool){
						if(instance->active && instance->type != entity->type && instance->maxHP && instance->type != Entity::TYPE::ENEMY_BULLET && (entity->pos - instance->pos).Length() <= entity->scale.x + instance->scale.x){
							const int entityOldHP = entity->HP;
							const int instanceOldHP = instance->HP;
							entity->HP -= Math::Max(0, instanceOldHP);
							instance->HP -= Math::Max(0, entityOldHP);
						}
					}
				}

				if(entity != player && (entity->pos - player->pos).Length() <= entity->scale.x + player->scale.x){ //Detection and resolution of dist-based collisions with player
					if(entity->type == Entity::TYPE::RED_ORB){
						entity->active = false;
						player->HP += 5;
						player->HP = Math::Min(player->HP, player->maxHP);
						continue;
					}
					if(entity->type == Entity::TYPE::ORANGE_ORB){
						entity->active = false;
						score += 50;
						continue;
					}
				}
				if(planet->active && entity != planet && (entity->pos - planet->pos).Length() <= entity->scale.x + planet->scale.x){ //Detection and resolution of dist-based collisions with planet
					entity->HP = 0;
				}
				if(entity->HP <= 0 && entity->maxHP){
					entity->active = false;
					if(entity->type == Entity::TYPE::ASTEROID && entity->maxHP > 10){ //If papa asteroid...
						SpawnMultipleAsteroids(entity->pos, Math::RandIntMinMax(20, 30));
					}
					if(entity->type == Entity::TYPE::ENEMY_STATIC || entity->type == Entity::TYPE::ENEMY_DYNAMIC){
						Entity* const& orb = GetInactive(); {
							orb->type = ~rand() & 1 ? Entity::TYPE::RED_ORB : Entity::TYPE::ORANGE_ORB;
							orb->active = true;
							orb->maxHP = 0;
							orb->HP = orb->maxHP;
							orb->scale.Set(worldHeight / 100.f, worldHeight / 100.f, 1.f);

							orb->pos = entity->pos;
							orb->vel.SetZero();
							orb->mass = .5f;
							orb->force.SetZero();

							orb->facingDir.SetZero();
							orb->angularVel = 0.f;
							orb->angularMass = 2.f / 5.f * orb->mass * orb->scale.x * orb->scale.x;
							orb->torque.SetZero();
						}
					}
					if(entity->type == Entity::TYPE::BUDDY){
						buddyPool.erase(std::find(buddyPool.begin(), buddyPool.end(), entity));
					}
					continue;
				}
				if(blackHole->active && entity != blackHole && entity != player){
					const float&& dist = (blackHole->pos - entity->pos).Length();
					if(dist <= blackHole->scale.x * 2.f){
						entity->active = false;
						blackHole->mass += entity->mass; //Accumulation of mass
						continue;
					}
					if(dist <= blackHole->scale.x * 5.f && entity->scale.x > blackHole->scale.x){
						entity->scale.Set(entity->scale.x / 1.05f, entity->scale.y / 1.05f, 1.f); //Compression
					}
					if(dist <= blackHole->scale.x * 25.f){
						entity->force = G * (entity->mass * blackHole->mass / (dist * dist)) * (blackHole->pos - entity->pos).Normalized();
					}
				}
				if(whiteHole->active && entity != whiteHole && entity != player){
					const float&& dist = (entity->pos - whiteHole->pos).Length();
					if(dist <= whiteHole->scale.x * 2.f){ //Yeeted away, too fast to see, "disappeared"
						entity->active = false;
						continue;
					}
					if(dist <= whiteHole->scale.x * 20.f){
						entity->force = G * (entity->mass * whiteHole->mass / (dist * dist)) * (entity->pos - whiteHole->pos).Normalized();
					}
				}

				if(entity->type == Entity::TYPE::BUDDY){
					const Vector3&& dir = player->pos - entity->pos;
					Vector3 normal = (dir).Cross(entity->facingDir);
					const float&& dist = (dir).Length();
					if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON) && entity->facingDir != dir.Normalized()){
						entity->facingDir = RotateVec(entity->facingDir, Math::DegreeToRadian(normal.z < 0.f ? 1.f : -1.f)); //Turn CW or CCW to face player
					}
					entity->force = entity->facingDir * dist; //Force based on dist to player

					for(auto iter = Entity::entityPool.begin(); iter != Entity::entityPool.end(); ++iter){
						Entity* const& instance = *iter;
						if(instance->active && instance != entity){
							const float&& dist = (entity->pos - instance->pos).Length();
							if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON) && dist <= entity->scale.x + instance->scale.x){
								entity->force += G * (entity->mass * instance->mass / (dist * dist)) * (entity->pos - instance->pos).Normalized();
							}
						}
					}

					Vector3 translation;
					float count = 0.f;
					for(const Entity* const& buddy: buddyPool){
						const float&& dist = (entity->pos - buddy->pos).Length();
						if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON) && dist <= (entity->scale.x + buddy->scale.x) * 3){
							translation += buddy->pos;
							++count;
						}
					}
					if(!count){
						entity->pos += Vector3();
					} else{
						translation = (1.f / count) * translation;
						entity->pos += (translation - entity->pos).Normalized();
					}
				}
				if(entity->type == Entity::TYPE::ENEMY_STATIC){
					const Vector3&& dir = player->pos - entity->pos;
					Vector3 normal = (dir).Cross(entity->facingDir);
					const float&& dist = (dir).Length();
					if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON) && entity->facingDir != dir.Normalized()){
						entity->facingDir = RotateVec(entity->facingDir, Math::DegreeToRadian(normal.z < 0.f ? 1.f : -1.f)); //Turn CW or CCW to face player
					}
					if((player->pos - entity->pos).Length() <= (entity->scale.x + player->scale.x) * 7.f){
						entity->force = entity->vel = Vector3(0.f);
					}

					if(spawnStaticEnemyBulletBT <= elapsedTime && entity->pos.x > -entity->scale.x && entity->pos.x < worldWidth + entity->scale.x && entity->pos.y > -entity->scale.y && entity->pos.y < worldHeight + entity->scale.y){
						Entity* const& eBullet = GetInactive(); {
							eBullet->type = Entity::TYPE::ENEMY_BULLET;
							eBullet->active = true;
							eBullet->maxHP = 1;
							eBullet->HP = eBullet->maxHP;
							eBullet->scale.Set(worldHeight / 50.f, worldHeight / 50.f, 1.f);

							eBullet->pos = entity->pos;
							eBullet->vel = entity->facingDir * 200.f;
							eBullet->mass = .5f;
							eBullet->force.SetZero();

							eBullet->facingDir.SetZero();
							eBullet->angularVel = 0.f;
							eBullet->angularMass = 2.f / 5.f * eBullet->mass * eBullet->scale.x * eBullet->scale.x;
							eBullet->torque.SetZero();
						}
						spawnStaticEnemyBulletBT = elapsedTime + .3f;
					}
				}
				if(entity->type == Entity::TYPE::ENEMY_DYNAMIC){
					const Vector3&& dir = player->pos - entity->pos;
					Vector3 normal = (dir).Cross(entity->facingDir);
					const float&& dist = (dir).Length();
					if(!(dist <= Math::EPSILON && -dist <= Math::EPSILON) && entity->facingDir != dir.Normalized()){
						entity->facingDir = RotateVec(entity->facingDir, Math::DegreeToRadian(normal.z < 0.f ? 1.f : -1.f)); //Turn CW or CCW to face player
					}
					if((player->pos - entity->pos).Length() > (entity->scale.x + player->scale.x) * 3.f){
						entity->force = entity->facingDir * 200.f;
					} else if((entity->pos - player->pos).Length() <= (entity->scale.x + player->scale.x) * 7.f){
						entity->force = entity->vel = Vector3(0.f);
					}

					if(spawnDynamicEnemyBulletBT <= elapsedTime && entity->pos.x > -entity->scale.x && entity->pos.x < worldWidth + entity->scale.x && entity->pos.y > -entity->scale.y && entity->pos.y < worldHeight + entity->scale.y){
						Entity* const& eBullet = GetInactive(); {
							eBullet->type = Entity::TYPE::ENEMY_BULLET;
							eBullet->active = true;
							eBullet->maxHP = 1;
							eBullet->HP = eBullet->maxHP;
							eBullet->scale.Set(worldHeight / 50.f, worldHeight / 50.f, 1.f);

							eBullet->pos = entity->pos;
							eBullet->vel = entity->facingDir * 800.f;
							eBullet->mass = .5f;
							eBullet->force.SetZero();

							eBullet->facingDir.SetZero();
							eBullet->angularVel = 0.f;
							eBullet->angularMass = 2.f / 5.f * eBullet->mass * eBullet->scale.x * eBullet->scale.x;
							eBullet->torque.SetZero();
						}
						spawnDynamicEnemyBulletBT = elapsedTime + .3f;
					}
				}

				if(entity->type == Entity::TYPE::BUDDY || entity->type == Entity::TYPE::PLAYER){
					///Wrap around
					if(entity->pos.x <= 0.f){
						entity->pos.x += worldWidth;
					}
					if(entity->pos.x >= worldWidth){
						entity->pos.x -= worldWidth;
					}
					if(entity->pos.y <= 0.f){
						entity->pos.y += worldHeight;
					}
					if(entity->pos.y >= worldHeight){
						entity->pos.y -= worldHeight;
					}
				}
				if(entity->type == Entity::TYPE::PLAYER_BULLET || entity->type == Entity::TYPE::ENEMY_BULLET || entity->type == Entity::TYPE::HOMING_BULLET || entity == blackHole || entity == whiteHole){
					if(entity->pos.x <= -entity->scale.x || entity->pos.x >= worldWidth + entity->scale.x || entity->pos.y <= -entity->scale.y || entity->pos.y >= worldHeight + entity->scale.y){ //If exit screen view...
						entity->active = false; //Unspawn
						continue;
					}
				}
				if(entity->type == Entity::TYPE::ASTEROID || entity->type == Entity::TYPE::ENEMY_STATIC || entity->type == Entity::TYPE::ENEMY_DYNAMIC){
					if(entity->pos.x < -entity->scale.x * 2 || entity->pos.x > worldWidth + entity->scale.x * 2 || entity->pos.y < -entity->scale.y * 2 || entity->pos.y > worldHeight + entity->scale.y * 2){ //If exit screen view and outside-screen spawn area...
						entity->active = false; //Unspawn
						continue;
					}
				}

				entity->vel += entity->force * (1.f / entity->mass) * dt * gameSpd;
				if(entity->type != Entity::TYPE::PLAYER_BULLET){
					entity->vel.x = Math::Clamp(entity->vel.x, -180.f, 180.f);
					entity->vel.y = Math::Clamp(entity->vel.y, -180.f, 180.f);
				}
				entity->pos.x += entity->vel.x * dt * gameSpd;
				entity->pos.y += entity->vel.y * dt * gameSpd;

				entity->angularVel += entity->torque.z * (1.f / entity->angularMass) * dt * gameSpd;
				entity->angularVel = Math::Clamp(entity->angularVel, -8.f, 8.f);
				entity->facingDir = RotateVec(entity->facingDir, entity->angularVel * dt * gameSpd);

				++activeCount;
				if(entity->type == Entity::TYPE::ASTEROID){
					++asteroidCount;
				}
				if(entity->type == Entity::TYPE::BUDDY){
					++buddyCount;
				}
				if(entity->type == Entity::TYPE::ENEMY_STATIC || entity->type == Entity::TYPE::ENEMY_DYNAMIC){
					++enemyCount;
				}
			}
		}
	}
}

void MainScene::UpdateMainMenu(const float& dt){
	if(levelSelectBT <= elapsedTime){
		lvl += short(App::IsKeyPressed(VK_UP) - App::IsKeyPressed(VK_DOWN));
		if(lvl < 1){
			lvl = 3;
		}
		if(lvl > 3){
			lvl = 1;
		}
		levelSelectBT = elapsedTime + .2f;
	}
	for(Entity* const& cParticle: Entity::cometParticlePool){
		if(!cParticle->active){
			cParticle->spawnTime = elapsedTime;
			cParticle->colour = Color(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
			cParticle->type = Entity::TYPE::COMET_PARTICLE;
			cParticle->active = true;
			cParticle->maxHP = 0;
			cParticle->HP = cParticle->maxHP;
			cParticle->scale.Set(worldHeight / 20.f, worldHeight / 20.f, 1.f);

			cParticle->pos.Set(Math::RandFloatMinMax(worldWidth * 3.f / 4.f - 25.f, worldWidth * 3.f / 4.f + 25.f), Math::RandFloatMinMax(worldHeight / 4.f - 25.f, worldHeight / 4.f + 25.f), 0.f);
			cParticle->vel.Set(Math::RandFloatMinMax(-200.f, -500.f), Math::RandFloatMinMax(200.f, 500.f), 0.f);
			cParticle->mass = 5.f;
			cParticle->force.SetZero();

			cParticle->facingDir.SetZero();
			cParticle->angularVel = 0.f;
			cParticle->angularMass = 2.f / 5.f * cParticle->mass * cParticle->scale.x * cParticle->scale.x;
			cParticle->torque.SetZero();
			break;
		}
	}
	for(Entity* const& cParticle: Entity::cometParticlePool){
		if(cParticle->active){
			cParticle->pos.x += cParticle->vel.x * dt * (worldWidth / 1920.f);
			cParticle->pos.y += cParticle->vel.y * dt * (worldHeight / 1080.f);
			if(cParticle->spawnTime + 3.f <= elapsedTime){
				cParticle->active = false;
			}
		}
	}
}

void MainScene::UpdateWinLose(const float& dt){
	for(short cycle = 0; cycle < 10; ++cycle){
		Vector3 offset = Vector3(Math::RandFloatMinMax(50.f, worldWidth - 50.f), Math::RandFloatMinMax(50.f, worldHeight - 50.f), 0.f);
		for(size_t i = (Entity::fireworkParticlePool.size() / 10) * cycle; i < (Entity::fireworkParticlePool.size() / 10) * (cycle + 1); ++i){
			Entity* const& fParticle = Entity::fireworkParticlePool[i];
			if(!fParticle->active){
				fParticle->spawnTime = elapsedTime;
				fParticle->colour = Color(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
				fParticle->type = Entity::TYPE::FIREWORK_PARTICLE;
				fParticle->active = true;
				fParticle->maxHP = 0;
				fParticle->HP = fParticle->maxHP;
				fParticle->scale.Set(worldHeight / 80.f, worldHeight / 80.f, 1.f);

				fParticle->pos.Set(offset.x + Math::RandFloatMinMax(0.f, 25.f) * Math::RandFloatMinMax(-1.f, 1.f), offset.y + Math::RandFloatMinMax(0.f, 25.f) * Math::RandFloatMinMax(-1.f, 1.f), 0.f);
				fParticle->vel = Vector3(Math::RandFloatMinMax(-1.f, 1.f), Math::RandFloatMinMax(-1.f, 1.f), 0.f) * 200.f;
				fParticle->mass = 5.f;
				fParticle->force.SetZero();

				fParticle->facingDir.SetZero();
				fParticle->angularVel = 0.f;
				fParticle->angularMass = 2.f / 5.f * fParticle->mass * fParticle->scale.x * fParticle->scale.x;
				fParticle->torque.SetZero();
			}
		}
		for(size_t i = (Entity::fireworkParticlePool.size() / 10) * cycle; i < (Entity::fireworkParticlePool.size() / 10) * (cycle + 1); ++i){
			Entity* const& fParticle = Entity::fireworkParticlePool[i];
			if(fParticle->active){
				fParticle->pos.x += fParticle->vel.x * dt * (worldWidth / 1920.f);
				fParticle->pos.y += fParticle->vel.y * dt * (worldHeight / 1080.f);
				if(fParticle->spawnTime + 1.f <= elapsedTime){
					fParticle->active = false;
				}
			}
		}
	}
}

Entity* const& MainScene::GetInactive(){
	for(auto iter = Entity::entityPool.begin() + 4; iter != Entity::entityPool.end(); ++iter){
		Entity* const& entity = *iter;
		if(!entity->active){
			return entity;
		}
	}
	Entity::entityPool.emplace_back(new Entity);
	printf("\n1 entity was added to the entity pool.\n");
	return Entity::entityPool.back();
}

void MainScene::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 projection;
	projection.SetToOrtho(0.0, double(worldWidth), 0.0, double(worldHeight), -10.0, 10.0);
	projectionStack.LoadMatrix(projection);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	viewStack.LookAt(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	modelStack.LoadIdentity();

	if(showMainMenu){
		RenderMainMenu();
		return;
	}
	if(showWinLose){
		RenderWinLose(!(player->HP <= 0));
		return;
	}
	modelStack.PushMatrix(); {
		modelStack.Translate(worldWidth / 2.f, worldHeight / 2.f, 0.f);
		modelStack.Scale(worldWidth / 16.f, worldHeight / 9.f, 1.f);
		RenderMesh(meshList[(uint)MESH_TYPE::GAME_BG]);
	} modelStack.PopMatrix();

	for(auto iter = Entity::entityPool.rbegin(); iter != Entity::entityPool.rend(); ++iter){
		const Entity* const& entity = *iter;
		if(entity->active){
			modelStack.PushMatrix(); {
				modelStack.Translate(entity->pos.x, entity->pos.y, 0.f);
				modelStack.Rotate(Math::RadianToDegree(atan2(entity->facingDir.y, entity->facingDir.x)), 0.f, 0.f, 1.f);

				modelStack.PushMatrix(); {
					modelStack.Scale(entity->scale.x, entity->scale.y, 1.f);
					MESH_TYPE meshType;
					switch(entity->type){
						case Entity::TYPE::PLAYER: meshType = MESH_TYPE::PLAYER; break;
						case Entity::TYPE::ASTEROID: meshType = MESH_TYPE::ASTEROID; break;
						case Entity::TYPE::ENEMY_STATIC: meshType = MESH_TYPE::ENEMY_STATIC; break;
						case Entity::TYPE::ENEMY_DYNAMIC: meshType = MESH_TYPE::ENEMY_DYNAMIC; break;
						case Entity::TYPE::BUDDY: meshType = MESH_TYPE::BUDDY; break;
						case Entity::TYPE::RED_ORB: meshType = MESH_TYPE::RED_ORB; break;
						case Entity::TYPE::ORANGE_ORB: meshType = MESH_TYPE::ORANGE_ORB; break;
						case Entity::TYPE::PLAYER_BULLET: meshType = MESH_TYPE::PLAYER_BULLET; break;
						case Entity::TYPE::ENEMY_BULLET: meshType = MESH_TYPE::ENEMY_BULLET; break;
						case Entity::TYPE::HOMING_BULLET: meshType = MESH_TYPE::HOMING_BULLET; break;
						case Entity::TYPE::BLACK_HOLE: meshType = MESH_TYPE::BLACK_HOLE; break;
						case Entity::TYPE::WHITE_HOLE: meshType = MESH_TYPE::WHITE_HOLE; break;
						case Entity::TYPE::PLANET: meshType = MESH_TYPE::PLANET; break;
						default: meshType = MESH_TYPE::AMT;
					}
					if(meshType != MESH_TYPE::AMT){
						RenderMesh(meshList[(uint)meshType]);
					}
				} modelStack.PopMatrix();

				if(entity->maxHP && entity->type != Entity::TYPE::PLAYER_BULLET && entity->type != Entity::TYPE::ENEMY_BULLET && entity->type != Entity::TYPE::HOMING_BULLET){
					modelStack.PushMatrix(); {
						modelStack.Translate(worldWidth / 96.f, 0.f, 0.f);
						modelStack.Rotate(-90.f, 0.f, 0.f, 1.f);
						modelStack.Scale(worldHeight / 150.f, worldHeight / 150.f, 1.f);
						RenderMesh(meshList[(uint)MESH_TYPE::HP_BAR_RED]);
						modelStack.PushMatrix(); {
							modelStack.Translate(float(entity->HP) / float(entity->maxHP) == 1.f ? 0.f : -(1.f - float(entity->HP) / float(entity->maxHP)) * 5.f / 2.f, 0.f, 0.f);
							modelStack.Scale(float(entity->HP) / float(entity->maxHP), 1.f, 1.f);
							RenderMesh(meshList[(uint)MESH_TYPE::HP_BAR_GREEN]);
						} modelStack.PopMatrix();
					} modelStack.PopMatrix();
				}
			} modelStack.PopMatrix();

			if(entity->type == Entity::TYPE::HOMING_BULLET && entity->target && entity->target->active){
				modelStack.PushMatrix(); {
					modelStack.Translate(entity->target->pos.x, entity->target->pos.y, 0.f);
					modelStack.Scale(entity->target->scale.x * 3.5f, entity->target->scale.y * 3.5f, 1.f);
					RenderMesh(meshList[(uint)MESH_TYPE::RETICLE]);
				} modelStack.PopMatrix();
			}
		}
	}

	std::ostringstream ss;
	ss.precision(3);
	ss << "FPS: " << FPS;
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 1.f, 1.f), worldHeight / 400.f, 0.f, worldHeight / 60.f);
	ss << "Player HP: " << player->HP;
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 0.f, 0.f), worldHeight / 400.f, 0.f, worldHeight / 72.2f);
	ss << "Score: " << score << '/' << (lvl == 1 ? 500 : (lvl == 2 ? 700 : 1000));
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, .5f, 0.f), worldHeight / 400.f, 0.f, worldHeight / 90.f);
	ss << "Active Entities: " << activeCount;
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 1.f, 0.f), worldHeight / 400.f, 0.f, worldHeight / 120.f);
	ss << "Asteroids: " << asteroidCount;
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(0.f, 1.f, 0.f), worldHeight / 400.f, 0.f, worldHeight / 180.f);
	ss << "Buddies: " << buddyCount;
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(0.f, .5f, 1.f), worldHeight / 400.f, 0.f, worldHeight / 360.f);
	ss << "Enemies: " << enemyCount;
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 0.f, 1.f), worldHeight / 400.f, 0.f, 0.f);
}

void MainScene::RenderTextOnScreen(const Mesh* const& mesh, const std::ostringstream& ss, const Color& colour, const float& size, const float& x, const float& y){
	if(!mesh || mesh->textureID <= 0){
		return;
	}
	Mtx44 projection;
	projection.SetToOrtho(0.0, double(worldWidth) / 10.0, 0.0, double(worldHeight) / 10.0, -10.0, 10.0);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(projection);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, 1.f);
	glUniform1i(glGetUniformLocation(shaderProgID, "useTex"), 1);
	glUniform1i(glGetUniformLocation(shaderProgID, "text"), 1);
	glUniform3fv(glGetUniformLocation(shaderProgID, "textColour"), 1, &colour.r);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(glGetUniformLocation(shaderProgID, "tex"), 0);
	str&& text = ss.str();
	for(unsigned i = 0; i < text.length(); ++i){
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * .7f + .3f, .1f, 0.f); //.7f is spacing, .3f and .1f are offsets
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgID, "MVP"), 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	const_cast<std::ostringstream&>(ss).str("");
	glBindTexture(GL_TEXTURE_2D, 0);

	glUniform1i(glGetUniformLocation(shaderProgID, "text"), 0);
	glUniform1i(glGetUniformLocation(shaderProgID, "useTex"), 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void MainScene::RenderMesh(const Mesh* const& mesh, const bool& useCustom, const Color& colour, const float& alpha){
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgID, "MVP"), 1, GL_FALSE, &MVP.a[0]);
	glUniform1i(glGetUniformLocation(shaderProgID, "useCustom"), useCustom);
	glUniform3fv(glGetUniformLocation(shaderProgID, "myColour"), 1, &colour.r);
	glUniform1f(glGetUniformLocation(shaderProgID, "myAlpha"), alpha);
	if(mesh->textureID){
		glUniform1i(glGetUniformLocation(shaderProgID, "useTex"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(glGetUniformLocation(shaderProgID, "tex"), 0);
	}
	mesh->Render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(glGetUniformLocation(shaderProgID, "useTex"), 0);
	glUniform1i(glGetUniformLocation(shaderProgID, "useCustom"), 0);
}

void MainScene::RenderMainMenu(){
	modelStack.PushMatrix(); {
		modelStack.Translate(worldWidth / 2.f, worldHeight / 2.f, 0.f);
		modelStack.Scale(worldWidth / 16.f, worldHeight / 9.f, 1.f);
		RenderMesh(meshList[(uint)MESH_TYPE::MENU_BG]);
	} modelStack.PopMatrix();

	modelStack.PushMatrix(); {
		modelStack.Translate(worldWidth - 250.f, worldHeight - 250.f, 0.f);
		modelStack.Scale(worldHeight / 6.f, worldHeight / 6.f, 1.f);
		RenderMesh(meshList[(uint)MESH_TYPE::LOGO]);
	} modelStack.PopMatrix();

	for(const Entity* const& cParticle: Entity::cometParticlePool){
		if(cParticle->active){
			modelStack.PushMatrix(); {
				modelStack.Translate(cParticle->pos.x, cParticle->pos.y, 0.f);
				modelStack.Scale(cParticle->scale.x, cParticle->scale.y, 1.f);
				RenderMesh(meshList[(uint)MESH_TYPE::COMET_PARTICLE_HOLLOW], true, cParticle->colour, (3.f - (elapsedTime - cParticle->spawnTime)) / 3.f);
			} modelStack.PopMatrix();
		}
	}

	for(const Entity* const& cParticle: Entity::cometParticlePool){
		if(cParticle->active){
			modelStack.PushMatrix(); {
				modelStack.Translate(cParticle->pos.x - 500.f, cParticle->pos.y, 0.f);
				modelStack.Scale(cParticle->scale.x, cParticle->scale.y, 1.f);
				RenderMesh(meshList[(uint)MESH_TYPE::COMET_PARTICLE_SOLID], true, cParticle->colour, (3.f - (elapsedTime - cParticle->spawnTime)) / 3.f);
			} modelStack.PopMatrix();
		}
	}

	std::ostringstream ss;
	ss << "Level 1";
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 1.f, (float)!(lvl == 1)), worldHeight / (250.f - 30.f * (float)(lvl == 1)), worldWidth / 192.f, worldHeight / 108.f);
	ss << "Level 2";
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 1.f, (float)!(lvl == 2)), worldHeight / (250.f - 30.f * (float)(lvl == 2)), worldWidth / 96.f, worldHeight / 54.f);
	ss << "Level 3";
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(1.f, 1.f, (float)!(lvl == 3)), worldHeight / (250.f - 30.f * (float)(lvl == 3)), worldWidth / 64.f, worldHeight / 36.f);
	ss << "Asteroid Shooter";
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(0.f, .25f, 1.f), worldHeight / 200.f, 0.f, 0.f);
}

void MainScene::RenderWinLose(const bool& win){
	modelStack.PushMatrix(); {
		modelStack.Translate(worldWidth / 2.f, worldHeight / 2.f, 0.f);
		modelStack.PushMatrix(); {
			modelStack.Scale(worldWidth / 16.f, worldHeight / 9.f, 1.f);
			RenderMesh(meshList[(uint)MESH_TYPE::WIN_LOSE_BG]);
		} modelStack.PopMatrix();
		modelStack.PushMatrix(); {
			modelStack.Scale(worldHeight / 3.f, worldHeight / 3.f, 1.f);
			RenderMesh(meshList[(uint)MESH_TYPE::LOGO]);
		} modelStack.PopMatrix();
	} modelStack.PopMatrix();

	for(const Entity* const& fParticle: Entity::fireworkParticlePool){
		if(fParticle->active){
			modelStack.PushMatrix(); {
				modelStack.Translate(fParticle->pos.x, fParticle->pos.y, 0.f);
				modelStack.Scale(fParticle->scale.x, fParticle->scale.y, 1.f);
				RenderMesh(meshList[(uint)MESH_TYPE::FIREWORK_PARTICLE], true, fParticle->colour, (1.f - (elapsedTime - fParticle->spawnTime)) / 1.f);
			} modelStack.PopMatrix();
		}
	}

	std::ostringstream ss;
	if(win){
		ss << "You Win!";
	} else{
		ss << "You Lose... Or Did You?";
	}
	RenderTextOnScreen(meshList[(uint)MESH_TYPE::TEXT], ss, Color(!win, win, 0.f), worldHeight / 100.f, 0.f, 0.f);
}

void MainScene::SpawnAsteroid(bool&& papa){
	Entity* const& asteroid = GetInactive(); {
		asteroid->type = Entity::TYPE::ASTEROID;
		asteroid->active = true;
		asteroid->maxHP = papa ? 17 : 2;
		asteroid->HP = asteroid->maxHP;
		const float&& scaleFactor = papa ? worldHeight / 10.f : Math::RandFloatMinMax(worldHeight / 100.f, worldHeight / 40.f);
		asteroid->scale.Set(scaleFactor, scaleFactor, 1.f);

		if(rand() & 1){
			asteroid->pos.x = rand() % 2 ? -asteroid->scale.x : worldWidth + asteroid->scale.x;
			asteroid->pos.y = Math::RandFloatMinMax(-asteroid->scale.y, worldHeight + asteroid->scale.y);
		} else{
			asteroid->pos.x = Math::RandFloatMinMax(-asteroid->scale.x, worldWidth + asteroid->scale.x);
			asteroid->pos.y = rand() % 2 ? -asteroid->scale.y : worldHeight + asteroid->scale.y;
		}
		asteroid->vel.SetZero();
		asteroid->mass = papa ? 85.f : 20.f;
		asteroid->force = (player->pos - asteroid->pos).Normalized() * (papa ? 50.f : Math::RandFloatMinMax(100.f, 400.f));

		asteroid->facingDir.Set(1.f, 0.f, 0.f);
		asteroid->angularVel = 0.f;
		asteroid->angularMass = 2.f / 5.f * asteroid->mass * asteroid->scale.x * asteroid->scale.x;
		asteroid->torque.Set(0.f, 0.f, papa ? Math::RandFloatMinMax(-2000.f, 2000.f) : Math::RandFloatMinMax(-5000.f, 5000.f));
	}
}

void MainScene::SpawnMultipleAsteroids(const Vector3& papaPos, short&& qty){
	const float&& increment = 360.f / float(qty);
	for(float angle = 0.f; angle <= 360.f; angle += increment){
		Entity* const& asteroid = GetInactive(); {
			asteroid->type = Entity::TYPE::ASTEROID;
			asteroid->active = true;
			asteroid->maxHP = 2;
			asteroid->HP = asteroid->maxHP;
			const float&& scaleFactor = Math::RandFloatMinMax(worldHeight / 100.f, worldHeight / 40.f);
			asteroid->scale.Set(scaleFactor, scaleFactor, 1.f);

			asteroid->pos = papaPos;
			const Vector3&& dir = RotateVec(Vector3(1.f, 0.f, 0.f), Math::DegreeToRadian(angle));
			asteroid->vel = dir * 200.f;
			asteroid->mass = 20.f;
			asteroid->force = -dir * 35.f;

			asteroid->facingDir.SetZero();
			asteroid->angularVel = 0.f;
			asteroid->angularMass = 2.f / 5.f * asteroid->mass * asteroid->scale.x * asteroid->scale.x;
			asteroid->torque.Set(0.f, 0.f, Math::RandFloatMinMax(-5000.f, 5000.f));
		}
	}
}

void MainScene::Exit(){
	while(buddyPool.size() > 0){
		buddyPool.pop_back();
	}
	Entity::ClearPools();
	for(int i = 0; i < (int)MESH_TYPE::AMT; ++i){
		if(meshList[i]){
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	glDeleteProgram(shaderProgID);
	glDeleteVertexArrays(1, &VAO);
}