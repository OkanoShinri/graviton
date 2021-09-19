#include "GameObject.h"

MyShip::MyShip() :
	counter(0), life(3), hit_anime_counter(0), radius(3.f), speed_rate(1.f), max_speed(3.f),
	delta_x_in_box(false),delta_y_in_box(false),delta_pos_in_box(false)
{

	pos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	velocity = ofVec2f(0, 0);
	resetAttraction();
	resetRepulsion();
	force = ofVec2f(0, 0);
}

void MyShip::update()
{
	move();
	delta_x_in_box = false;
	delta_y_in_box = false;
	delta_pos_in_box = false;
}

ofVec2f MyShip::move() {
	
	if (delta_x_in_box && delta_y_in_box) {
		velocity = -velocity;
	}
	else if (delta_x_in_box) {
		velocity.x = -velocity.x;
	}
	else if (delta_y_in_box) {
		velocity.y = -velocity.y;
	}
	else if (delta_pos_in_box) {
		velocity = -velocity;
	}
	
	pos += velocity;

	return pos;
}

void MyShip::simulate_move()
{
	force = (attraction + repulsion);
	velocity += force;
	if (velocity.x < -max_speed) {
		velocity.x += 0.1;
	}
	if (velocity.x > max_speed) {
		velocity.x -= 0.1;
	}
	if (velocity.y < -max_speed) {
		velocity.y += 0.1;
	}
	if (velocity.y > max_speed) {
		velocity.y -= 0.1;
	}
}

void MyShip::draw() const
{ 
	ofSetColor(20, 255, 20);
	ofDrawRectangle(clear_area_x, clear_area_y, clear_area_w, clear_area_h);
	ofSetColor(0, 0, 0);
	ofDrawCircle(pos, radius);
}

void MyShip::onDashPanel(float x, float y, int w, int h, int direction)
{
	if (isInBox(x, y, w, h, pos.x, pos.y)) {
		switch (direction)
		{
		case 1://up
			velocity.y = -7.f;
			break;
		case 2://right
			velocity.x = 7.f;
			break;
		case 3://down
			velocity.y = 7.f;
			break;
		case 4://left
			velocity.x = -7.f;
			break;
		default:
			break;
		}
	}
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

bool MyShip::getBoxIntersection(float x, float y, int w, int h)
{
	x -= 1;
	y -= 1;
	w += 2;
	h += 2;

	bool hit = false;
	if (isInBox(x, y, w, h, pos.x + velocity.x, pos.y) || isInBox(x, y, w, h, pos.x - velocity.x, pos.y))
	{
		hit = true;
		delta_x_in_box = true;
	}
	else if (isInBox(x, y, w, h, pos.x, pos.y + velocity.y) || isInBox(x, y, w, h, pos.x, pos.y - velocity.y))
	{
		hit = true;
		delta_y_in_box = true;
	}
	else if (isInBox(x, y, w, h, pos.x + velocity.x, pos.y + velocity.y))
	{
		hit = true;
		delta_pos_in_box = true;
	}


	//���ɋ����牟���o���Ă��
	if (isInBox(x, y, w, h, pos.x, pos.y))
	{
		float mindist = pos.x - x;
		ofVec2f delta = ofVec2f(-mindist - 2, 0);
		if (x + w - pos.x < mindist)
		{
			mindist = x + w - pos.x;
			delta.set(mindist + 2, 0);
		}
		if (pos.y - y < mindist)
		{
			mindist = pos.y - y;
			delta.set(0, -mindist - 2);
		}
		if (y + h - pos.y < mindist)
		{
			mindist = y + h - pos.y;
			delta.set(0, mindist + 2);
		}
		pos += delta;

		//�f�o�b�O�p
		assert(!isInBox(x, y, w, h, pos.x, pos.y));
	}

	return hit;
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

Obstacle::Obstacle(int x, int y, int w, int h) :
	pos(ofVec2f(x, y)), width(w), height(h)
{
}

void Obstacle::draw(const ofVec2f center_pos)
{
	if (pos.x + width < center_pos.x - 512 || center_pos.x + 512 < pos.x || pos.y + height < center_pos.y - 384 || center_pos.y + 384 < pos.y) {
		return;
	}
	ofSetColor(0, 0, 0);
	ofDrawRectangle(pos, width, height);
}

void Obstacle::update(const ofVec2f center_pos)
{
}

MovingObstacle::MovingObstacle(int x, int y, int w, int h, int movex1, int movey1):
	counter(0), init_pos(ofVec2f(x, y)),  width(w), height(h)
{
	pos = ofVec2f(movex1, movey1);
	amplitude_x = abs(movex1 - x);
	amplitude_y = abs(movey1 - y);

	if (x == movex1 && y == movey1) {
		move = false;
	}
	else {
		move = true;
	}
}

void MovingObstacle::draw(const ofVec2f center_pos)
{
	Obstacle::draw(center_pos);
}


void MovingObstacle::update(const ofVec2f center_pos)
{
	if (pos.x + width < center_pos.x - 512 || center_pos.x + 512 < pos.x || pos.y + height < center_pos.y - 384 || center_pos.y + 384 < pos.y) {
		return;
	}
	if (move) {
		pos.x = init_pos.x + amplitude_x * (1 + cos(counter / 100.0))*0.5;
		pos.y = init_pos.y + amplitude_y * (1 + cos(counter / 100.0))*0.5;
	}
	counter++;
}

DashPanel::DashPanel(int x, int y, int w, int h, int direction):
	m_pos(ofVec2f(x, y)), m_width(w), m_height(h),m_direction(direction)
{
}

void DashPanel::draw(const ofVec2f center_pos)
{
	if (m_pos.x + m_width < center_pos.x - 512 || center_pos.x + 512 < m_pos.x ||m_pos.y + m_height < center_pos.y - 384 || center_pos.y + 384 < m_pos.y) {
		return;
	}
	ofSetColor(255, 165, 0);
	switch (m_direction)
	{
	case 1://up
		ofDrawTriangle(m_pos.x + m_width / 2, m_pos.y, m_pos.x + m_width, m_pos.y + m_height, m_pos.x, m_pos.y + m_height);
		break;
	case 2://right
		ofDrawTriangle(m_pos.x, m_pos.y, m_pos.x + m_width, m_pos.y + m_height / 2, m_pos.x, m_pos.y + m_height);
		break;
	case 3://down
		ofDrawTriangle(m_pos.x, m_pos.y, m_pos.x + m_width, m_pos.y, m_pos.x + m_width / 2, m_pos.y + m_height);
		break;
	case 4://left
		ofDrawTriangle(m_pos.x, m_pos.y + m_height / 2, m_pos.x + m_width, m_pos.y, m_pos.x + m_width, m_pos.y + m_height);
		break;
	default:
		break;
	}
}

void DashPanel::update()
{
}
