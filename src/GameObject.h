#pragma once
#include "ofMain.h"

class MyShip
{
private:
	bool is_hit;
	int counter, life, hit_anime_counter;
	float radius, speed;
	ofVec2f pos, vec, attraction, repulsion, force;

public:
	MyShip();
	~MyShip() { std::cout << "Remove: MyShip" << std::endl; }

	void update();
	void move();
	void draw();
	void addAttraction(ofVec2f attraction_pos);
	void resetAttraction();
	void addRepulsion(ofVec2f repulsion_pos);
	void resetRepulsion();
	void addStrongRepulsion(ofVec2f repulsion_pos);
	inline ofVec2f getPos() {
		return pos;
	}
};

class AttractionPoint
{
private:
	float radius;
	ofVec2f pos;


public:
	AttractionPoint(float x, float y);
	~AttractionPoint() { std::cout << "Remove: AttractionPoint" << std::endl; }

	void update() {};
	void draw();
	void setPos(ofVec2f pos);
	inline ofVec2f getPos() {
		return pos;
	}
	void setPos(float x, float y);
};

class RepulsionPoint
{
private:
	float radius;
	ofVec2f pos;


public:
	RepulsionPoint(float x, float y);
	~RepulsionPoint() { std::cout << "Remove: RepulsionPoint" << std::endl; }

	void update() {};
	void draw();
	void setPos(ofVec2f pos);
	ofVec2f getPos() {
		return pos;
	}
	void setPos(float x, float y);
};

class Target {
private:
	ofVec2f pos;
	int id;
	float radius;
	bool can_remove;

public:
	Target(int id);
	void update(ofVec2f my_pos);
	void draw();
	bool is_hit(ofVec2f my_pos);
	bool canRemove() {
		return can_remove;
	};
};
