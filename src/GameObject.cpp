#include "GameObject.h"


MyShip::MyShip() :
	counter(0), life(3), hit_anime_counter(0), radius(10.0)
{
	is_hit = false;
	pos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	vec = ofVec2f(0, 0);
	force = ofVec2f(0, 0);
}

void MyShip::update()
{
	move();
}

void MyShip::move() {
	vec += force;
	pos += vec;

	if (pos.x > ofGetWidth()) {
		pos.x = ofGetWidth();
		vec.x *= -1;
	}
	else if (pos.x < 0) {
		pos.x = 0;
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

void AttractionPoint::draw()
{
	ofSetColor(255, 0, 0);
	ofDrawCircle(pos, 5.0);
}

void RepulsionPoint::draw()
{
	ofSetColor(0, 0, 255);
	ofDrawCircle(pos, 5.0);
}
