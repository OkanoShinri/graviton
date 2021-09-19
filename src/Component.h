#pragma once
#include "ofMain.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
	void drawTime(int x,int y, std::shared_ptr<ofTrueTypeFont> font) {
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


template <typename T> class Selector
{
public:
	inline Selector(std::vector<T> init_choices,std::vector<std::string> init_choices_str, bool roop) :
		choice_idx(0)
	{
		assert(init_choices.size() > 0);
		assert(init_choices.size() == init_choices_str.size());

		choices = init_choices;
		choices_str = init_choices_str;
		is_roop = roop;
	};
	inline void changeLeft() {
		choice_idx = (choice_idx + choices.size() - 1) % choices.size();
	};
	inline void changeRight() {
		choice_idx = (choice_idx + 1) % choices.size();
	};
	inline void draw(int x, int y, std::shared_ptr<ofTrueTypeFont> font) {
		font->drawString(choices_str[choice_idx], x, y);
	};
	void update() {};
	inline T enter() {
		return choices[choice_idx];
	};
private:
	std::vector<T> choices;
	std::vector<std::string> choices_str;
	int counter, choice_idx;
	bool is_roop;
};

class ObjectHitChecker {

};