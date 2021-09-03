#include "GameObject.h"


MyShip::MyShip() :
	counter(0), life(3), hit_anime_counter(0), radius(10.0),speed(1.0)
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

void MyShip::move() {
	force = (attraction + repulsion);
	float max_vec = 4.0;
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

	pos += vec*speed;

	if (pos.x > ofGetWidth()) {
		pos.x = ofGetWidth();
		vec.x *= -1;
	}
	else if (pos.x < ofGetWidth() / 5) {
		pos.x = ofGetWidth() / 5;
		vec.x *= -1;
	}

	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vec.y *= -1;
	}
	else if (pos.y < 0) {
		pos.y = 0;
		vec.y *= -1;
	}
}
ofVec2f MyShip::relative_move()
{
	force = (attraction + repulsion);
	float max_vec = 4.0;
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

	return vec;
}
float MyShip::relative_move_x()
{
	force = (attraction + repulsion);
	float max_vec = 4.0;
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

	pos.y += vec.y * speed;

	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vec.y *= -1;
	}
	else if (pos.y < 0) {
		pos.y = 0;
		vec.y *= -1;
	}

	return vec.x;
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
	//attraction = G / (pos - attraction_pos).lengthSquared() * (pos - attraction_pos).getNormalized();
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
	float G = 100.0;
	repulsion = G / (pos - repulsion_pos).lengthSquared() * (pos - repulsion_pos).getNormalized();
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

AttractionPoint::AttractionPoint(float x, float y):
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

Wall::Wall(int x, int y, int w, int h)
{
	pos = ofVec2f(x, y);
	width = w;
	height = h;
}

void Wall::draw()
{
	ofSetColor(0, 0, 0);
	ofDrawRectangle(pos, width, height);
}

void Wall::update()
{
}

void Wall::relative_move(ofVec2f delta)
{
	pos += delta;
}
