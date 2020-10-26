#pragma once
#include "Utility.h"
#include "Vector3.h"
#include <vector>

struct Entity final{
	static std::vector<Entity*> cometParticlePool;
	static std::vector<Entity*> entityPool;
	static std::vector<Entity*> fireworkParticlePool;
	enum class TYPE{
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
		COMET_PARTICLE,
		FIREWORK_PARTICLE,
		AMT
	};
	Entity();
	static void ClearPools();
	const Entity* target;
	float spawnTime;
	Color colour;

	TYPE type;
	bool active;
	int maxHP;
	int HP;
	Vector3 scale;

	Vector3 pos;
	Vector3 vel;
	float mass;
	Vector3 force;

	Vector3 facingDir;
	float angularVel;
	float angularMass;
	Vector3 torque;
};