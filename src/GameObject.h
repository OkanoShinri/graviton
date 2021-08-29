#pragma once
#include "ofMain.h"

class MyShip
{
private:
	bool is_hit;
	int counter, life, hit_anime_counter;
	float radius;
	ofVec2f pos, vec, force;

public:
	MyShip();
	~MyShip() { std::cout << "Remove: MyShip" << std::endl; }

	void update();
	void move();
	void draw();
};

class AttractionPoint
{
private:
	float radius;
	ofVec2f pos;


public:
	AttractionPoint() {};
	~AttractionPoint() { std::cout << "Remove: AttractionPoint" << std::endl; }

	void update() {};
	void draw();
};

class RepulsionPoint
{
private:
	float radius;
	ofVec2f pos;


public:
	RepulsionPoint() {};
	~RepulsionPoint() { std::cout << "Remove: RepulsionPoint" << std::endl; }

	void update() {};
	void draw();
};