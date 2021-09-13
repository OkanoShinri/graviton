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
	ofVec2f move();
	void draw();
	void addAttraction(ofVec2f attraction_pos);
	void resetAttraction();
	void addRepulsion(ofVec2f repulsion_pos);
	void resetRepulsion();
	void addStrongRepulsion(ofVec2f repulsion_pos);
	inline ofVec2f getPos() {
		return pos;
	}
	bool isHitLine(float x1, float y1, float x2, float y2);
	bool isHitBox(float x, float y, int w, int h);
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
	void move(ofVec2f delta) {
		pos += delta;
	};
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
	void move(ofVec2f delta) {
		pos += delta;
	};
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

class Wall {
public:
	Wall(int x_1, int y_1, int x_2, int y_2);
	void draw();
	void update();
	void relative_move(ofVec2f delta);
	bool canRemove() {
		return max(x1, x2) < -100;
	};
	inline float getX1() { return x1; };
	inline float getY1() { return y1; };
	inline float getX2() { return x2; };
	inline float getY2() { return y2; };
private:
	//ofVec2f pos1, pos2;
	float x1, y1, x2, y2;
};

class Obstacle {
public:
	Obstacle() {};
	Obstacle(int x, int y, int w, int h);
	virtual void draw(ofVec2f center_pos);
	virtual void update(ofVec2f center_pos);
	bool canRemove() {
		return false;
	};

	inline float getX() { return this->pos.x; };
	inline float getY() { return this->pos.y; };
	inline int getW() { return this->width; };
	inline int getH() { return this->height; };

protected:
	ofVec2f pos;
	int width, height;
};


class MovingObstacle : public Obstacle {
public:
	MovingObstacle():counter(0) {};
	MovingObstacle(int x, int y, int w, int h, int movex1, int movey1);
	void draw(ofVec2f center_pos);
	void update(ofVec2f center_pos);
private:
	bool move;
	const ofVec2f init_pos;
	int counter;
	double amplitude_x, amplitude_y;
};