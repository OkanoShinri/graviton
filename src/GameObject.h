#pragma once
#include "ofMain.h"


class MyShip
{
private:
	bool delta_x_in_box, delta_y_in_box, delta_pos_in_box;
	int counter, life, hit_anime_counter, clear_area_w = 0, clear_area_h = 0;
	float radius, speed_rate,max_speed, clear_area_x = 0, clear_area_y = 0;
	ofVec2f pos, attraction, repulsion, force;
	ofVec2f velocity;

public:
	MyShip();
	~MyShip() {}

	void update();
	ofVec2f move();
	void simulate_move();
	void draw() const;
	bool onDashPanel(float x, float y, int w, int h, int direction);
	void addAttraction(ofVec2f attraction_pos);
	void resetAttraction();
	void addRepulsion(ofVec2f repulsion_pos);
	void resetRepulsion();
	void addStrongRepulsion(ofVec2f repulsion_pos);
	inline ofVec2f getPos() {
		return pos;
	}
	void setClearArea(float x, float y, int w, int h) {
		clear_area_x = x;
		clear_area_y = y;
		clear_area_w = w;
		clear_area_h = h;
	}
	void setSpeedRate(float a_speed_rate){
		speed_rate = a_speed_rate;
	}
	void setMaxSpeed(float a_max_speed){
		max_speed = a_max_speed;
	}
	bool getBoxIntersection(float x, float y, int w, int h);
	
	inline bool isInBox(const float x, const float y, const int w, const int h, const float mx, const float my)
	{
		return (x < mx && mx < x + w && y < my && my < y + h);
	};
	inline bool isInCircle(const float x, const float y, const float r, const float mx, const float my)
	{
		return ((x - mx)*(x - mx) + (y - my)*(y - my) < r*r);
	};
	bool isInCircle(const float x, const float y, const float r)
	{
		return ((x - pos.x)*(x - pos.x) + (y - pos.y)*(y - pos.y) < r*r);
	};
	bool isClear() {
		return  (clear_area_x < pos.x 
			&& pos.x < clear_area_x + clear_area_w 
			&& clear_area_y < pos.y 
			&& pos.y < clear_area_y + clear_area_h);
	}
};

class AttractionPoint
{
private:
	float radius;
	ofVec2f pos;


public:
	AttractionPoint(float x, float y);
	~AttractionPoint() {}

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
	~RepulsionPoint() {}

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


class Obstacle {
public:
	//Obstacle():width(0), height(0) {};
	Obstacle(int x, int y, int w, int h);
	virtual void draw(const ofVec2f center_pos);
	virtual void update(const ofVec2f center_pos);
	virtual inline float getX() { return this->pos.x; };
	virtual inline float getY() { return this->pos.y; };
	virtual inline int getW() { return this->width; };
	virtual inline int getH() { return this->height; };

private:
	ofVec2f pos;
	const int width, height;
};


class MovingObstacle : public Obstacle {
public:
	//MovingObstacle():counter(0),width(0),height(0) {};
	MovingObstacle(int x, int y, int w, int h, int movex1, int movey1);
	void draw(const ofVec2f center_pos);
	void update(const ofVec2f center_pos);
	inline float getX() { return this->pos.x; };
	inline float getY() { return this->pos.y; };
	inline int getW() { return this->width; };
	inline int getH() { return this->height; };
private:
	bool move;
	const ofVec2f init_pos;
	ofVec2f pos;
	const int width, height;
	int counter;
	double amplitude_x, amplitude_y;
};

class DashPanel {
public:
	DashPanel(int x, int y, int w, int h, int direction);
	void draw(const ofVec2f center_pos);
	void update();
	inline float getX() { return this->m_pos.x; };
	inline float getY() { return this->m_pos.y; };
	inline int getW() { return this->m_width; };
	inline int getH() { return this->m_height; };
	inline int getDirection() { return m_direction; }
private:
	const ofVec2f m_pos;
	const int m_direction, m_width, m_height;
};

class Target {
public:
	Target(int x, int y);
	void draw(const ofVec2f center_pos);
	void update();
	inline float getX() { return this->m_pos.x; };
	inline float getY() { return this->m_pos.y; };
	inline int getR() { return this->m_radius; };
private:
	const ofVec2f m_pos;
	const float m_radius;
};