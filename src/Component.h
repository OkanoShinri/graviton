#pragma once
#include "ofMain.h"
#include <iostream>
#include <memory>
#include <list>

class Timer
{
public:
	Timer() :
		counter(0), activate(false)
	{};
	~Timer() {};

	void update() {
		if (activate) {
			counter++;
		}
	};
	void reset() {
		counter = 0;
	};
	void stop() {
		activate = false;
	};
	void start() {
		activate = true;
	};
	void draw_time(int x,int y, std::shared_ptr<ofTrueTypeFont> font) {
		char minute[3];
		sprintf_s(minute, "%02d", counter / 3600);
		char second[3];
		sprintf_s(second, "%02d", (counter / 60) % 60);
		char m_second[3];
		sprintf_s(m_second, "%02d", (int)ofMap(counter % 60, 0, 60, 0, 99));
		ofSetColor(0, 0, 0);
		font->drawString("" + std::string(minute) + ":" + std::string(second) + "." + std::string(m_second), x, y);
	}
private:
	int counter;
	bool activate;
};


class Selector 
{
public:

private:
};

