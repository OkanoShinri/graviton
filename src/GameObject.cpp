#include "GameObject.h"

MyShip::MyShip() :
	counter(0), life(3), hit_anime_counter(0), radius(3.0), speed(1.0)
{
	is_hit = false;
	pos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	vec = ofVec2f(0, 0);
	resetAttraction();
	resetRepulsion();
	force = ofVec2f(0, 0);
}

void MyShip::update()
{
	move();
}

ofVec2f MyShip::move() {
	force = (attraction + repulsion);
	float max_vec = 6.0;
	float min_vec = -max_vec;
	vec += force;
	if (vec.x < min_vec) {
		vec.x = min_vec;
	}
	if (vec.x > max_vec) {
		vec.x = max_vec;
	}
	if (vec.y < min_vec) {
		vec.y = min_vec;
	}
	if (vec.y > max_vec) {
		vec.y = max_vec;
	}

	pos += vec * speed;

	return pos;
}

void MyShip::draw()
{
	ofSetColor(0, 0, 0);
	ofDrawCircle(pos, radius);
}

void MyShip::addAttraction(ofVec2f attraction_pos)
{
	if ((pos - attraction_pos).length() < 10) {
		attraction.x = 0;
		attraction.y = 0;
		return;
	}
	float G = -0.1;
	attraction = G / (pos - attraction_pos).lengthSquared() * (pos - attraction_pos).getNormalized();
	attraction = G * (pos - attraction_pos).getNormalized();
}

void MyShip::resetAttraction()
{
	attraction = ofVec2f(0.0, 0.0);
}

void MyShip::addRepulsion(ofVec2f repulsion_pos)
{
	if ((pos - repulsion_pos).length() < 10) {
		repulsion.x = 0;
		repulsion.y = 0;
		return;
	}
	float G = 0.1;
	repulsion = G / (pos - repulsion_pos).lengthSquared() * (pos - repulsion_pos).getNormalized();
	repulsion = G * (pos - repulsion_pos).getNormalized();
}

void MyShip::resetRepulsion()
{
	repulsion = ofVec2f(0.0, 0.0);
}

void MyShip::addStrongRepulsion(ofVec2f repulsion_pos)
{
	if ((pos - repulsion_pos).length() < 10) {
		return;
	}
	float G = 100.0;
	repulsion = G / ((pos.x - repulsion_pos.x)*(pos.x - repulsion_pos.x) + (pos.y - repulsion_pos.y)*(pos.y - repulsion_pos.y)) * (pos - repulsion_pos).getNormalized();
}

bool MyShip::isHitLine(float x1, float y1, float x2, float y2)
{
	bool is_hit = false;
	if (y1 == y2 && (y1 > pos.y != y1 > pos.y + vec.y)) {
		if (x1 < pos.x && pos.x < x2)
		{
			is_hit = true;
			pos.y = y1;
			vec.y *= -1.1;
		}
	}
	else if (x1 == x2 && (x1 > pos.x != x1 > pos.x + vec.x)) {
		if (y1 < pos.y && pos.y < y2)
		{
			is_hit = true;
			pos.x = x1;
			vec.x *= -1.1;
		}
	}
	return is_hit;
}

bool MyShip::isHitBox(float x, float y, int w, int h)
{
	bool is_hit = false;
	if (pos.x < x && y < pos.y&&pos.y < y + h && x < pos.x + vec.x)
	{
		is_hit = true;
		vec.x *= -1.1;
	}
	else if (x + w < pos.x && y < pos.y&&pos.y < y + h && pos.x + vec.x < x + w)
	{
		is_hit = true;
		vec.x *= -1.1;
	}
	else if (pos.y < y && x < pos.x&& pos.x < x + w && y < pos.y + vec.y)
	{
		is_hit = true;
		vec.y *= -1.1;
	}
	else if (y + h < pos.y && x < pos.x&&pos.x < x + w && pos.y + vec.y < y + h)
	{
		is_hit = true;
		vec.y *= -1.1;
	}

	//’†‚É‹‚½‚ç‰Ÿ‚µo‚µ‚Ä‚â‚é
	if (x < pos.x && pos.x<x+w&& y < pos.y && pos.y < y + h)
	{
		int mindist = pos.x - x;
		ofVec2f delta = ofVec2f(-mindist, 0);
		if (x + w - pos.x < mindist) 
		{
			mindist = x + w - pos.x;
			delta.set(mindist, 0);
		}
		if (pos.y-y < mindist)
		{
			mindist = pos.y - y;
			delta.set(0, -mindist);
		}
		if (y + h - pos.y < mindist)
		{
			mindist = y + h - pos.y;
			delta.set(mindist, 0);
		}
		pos += delta;
	}
	return is_hit;
}

AttractionPoint::AttractionPoint(float x, float y) :
	radius(10.0)
{
	pos = ofVec2f(x, y);
}

void AttractionPoint::draw()
{
	ofSetColor(255, 0, 0);
	ofDrawCircle(pos, 5.0);
}

void AttractionPoint::setPos(ofVec2f mouse_pos)
{
	pos = mouse_pos;
}

void AttractionPoint::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

RepulsionPoint::RepulsionPoint(float x, float y) :
	radius(10.0)
{
	pos = ofVec2f(x, y);
}

void RepulsionPoint::draw()
{
	ofSetColor(0, 0, 255);
	ofDrawCircle(pos, 5.0);
}

void RepulsionPoint::setPos(ofVec2f mouse_pos)
{
	pos = mouse_pos;
}

void RepulsionPoint::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

Target::Target(int id) :
	id(id), radius(20.0), can_remove(false)
{
	pos = ofVec2f(ofRandom(ofGetWidth() / 4, ofGetWidth()), ofRandom(ofGetHeight()));
}

void Target::update(ofVec2f my_pos)
{
	can_remove = is_hit(my_pos);
}

void Target::draw()
{
	ofSetColor(200, 200, 200);
	ofDrawCircle(pos, radius);
}

bool Target::is_hit(ofVec2f my_pos)
{
	return pos.distance(my_pos) < radius;
}

Wall::Wall(int x_1, int y_1, int x_2, int y_2)
{
	x1 = x_1;
	y1 = y_1;
	x2 = x_2;
	y2 = y_2;
}

void Wall::draw()
{
	ofSetColor(0, 0, 0);
	ofDrawLine(x1, y1, x2, y2);
}

void Wall::update()
{
}

void Wall::relative_move(ofVec2f delta)
{
	x1 += delta.x;
	y1 += delta.y;
	x2 += delta.x;
	y2 += delta.y;
}

Obstacle::Obstacle(int x, int y, int w, int h)
{
	pos = ofVec2f(x, y);
	width = w;
	height = h;
}

void Obstacle::draw(ofVec2f center_pos)
{
	if (pos.x + width < center_pos.x - 512 || center_pos.x + 512 < pos.x || pos.y + height < center_pos.y - 384 || center_pos.y + 384 < pos.y) {
		return;
	}
	ofSetColor(0, 0, 0);
	ofDrawRectangle(pos, width, height);
}

void Obstacle::update(ofVec2f center_pos)
{
}

MovingObstacle::MovingObstacle(int x, int y, int w, int h, int movex1, int movey1):
	counter(0), init_pos(ofVec2f(x, y))
{
	pos = ofVec2f(x, y);
	width = w;
	height = h;
	amplitude_x = abs(movex1 - x);
	amplitude_y = abs(movey1 - y);

	if (x == movex1 && y == movey1) {
		move = false;
	}
	else {
		move = true;
	}
}

void MovingObstacle::draw(ofVec2f center_pos)
{
	Obstacle::draw(center_pos);
}


void MovingObstacle::update(ofVec2f center_pos)
{
	if (pos.x + width < center_pos.x - 512 || center_pos.x + 512 < pos.x || pos.y + height < center_pos.y - 384 || center_pos.y + 384 < pos.y) {
		return;
	}
	if (move) {
		pos.x = init_pos.x + (1 + amplitude_x * cos(counter / 100.0))*0.5;
		pos.y = init_pos.y + (1 + amplitude_y * cos(counter / 100.0))*0.5;
	}
	counter++;
}
