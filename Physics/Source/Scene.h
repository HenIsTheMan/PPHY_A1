#pragma once
typedef unsigned int uint;
typedef std::string str;

struct Scene{
	virtual ~Scene() = default;
	virtual void Init() = 0;
	virtual void Update(const float&) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};