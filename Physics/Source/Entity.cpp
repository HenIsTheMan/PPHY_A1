#include "Entity.h"

std::vector<Entity*> Entity::cometParticlePool;
std::vector<Entity*> Entity::entityPool;
std::vector<Entity*> Entity::fireworkParticlePool;

Entity::Entity(): target(nullptr), spawnTime(0.f), colour(Color()),
	type(TYPE::AMT), active(true), maxHP(0), HP(0), scale(1.f),
	pos(0.f), vel(0.f), mass(1.f), force(0.f),
	facingDir(0.f), angularVel(0.f), angularMass(1.f), torque(0.f){}

void Entity::ClearPools(){
	while(cometParticlePool.size() > 0){
		delete cometParticlePool.back();
		cometParticlePool.pop_back();
	}
	while(entityPool.size() > 0){
		delete entityPool.back();
		entityPool.pop_back();
	}
	while(fireworkParticlePool.size() > 0){
		delete fireworkParticlePool.back();
		fireworkParticlePool.pop_back();
	}
}