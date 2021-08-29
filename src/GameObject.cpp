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

	if (pos.x > ofGetWidth()*3/4) {
		pos.x = ofGetWidth() * 3 / 4;
		vec.x *= -1;
	}
	else if (pos.x < ofGetWidth() / 4) {
		pos.x = ofGetWidth() / 4;
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

void MyShip::draw()
{
	ofSetColor(30, 30, 30);
	ofDrawCircle(pos, radius);
}

void MyShip::addAttraction(ofVec2f attraction_pos)
{
	if ((pos - attraction_pos).length() < 10) {
		return;
	}
	float G = -10.0;
	attraction = G / ((pos.x - attraction_pos.x)*(pos.x - attraction_pos.x) + (pos.y - attraction_pos.y)*(pos.y - attraction_pos.y)) * (pos - attraction_pos);
}

void MyShip::resetAttraction()
{
	attraction = ofVec2f(0.0, 0.0);
}

void MyShip::addRepulsion(ofVec2f repulsion_pos)
{
	if ((pos - repulsion_pos).length() < 10) {
		return;
	}
	float G = 10.0;
	repulsion = G / ((pos.x - repulsion_pos.x)*(pos.x - repulsion_pos.x) + (pos.y - repulsion_pos.y)*(pos.y - repulsion_pos.y)) * (pos - repulsion_pos);
}

void MyShip::resetRepulsion()
{
	repulsion = ofVec2f(0.0, 0.0);
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
