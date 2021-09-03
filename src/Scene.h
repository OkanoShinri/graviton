#pragma once
#include "ofMain.h"
#include "SettingParameter.h"
#include <iostream>
#include <memory>
#include <list>

class Scene {
public:
	enum SceneIdx { quit_scene = 0, title_scene, setting_scene, game_scene1, game_scene2
	};

	virtual ~Scene() {};
	virtual std::unique_ptr<SettingParameter> getSettingParameter() = 0;
	virtual SceneIdx getNextScene() = 0;
	virtual bool canChangeScene() = 0;

	virtual void draw() {};
	virtual void feadout(int mun) {};
	virtual void keyPressed(int key) {};
	virtual void keyReleased(int key) {};
	virtual void mouseDragged(int x, int y, int button) {};
	virtual void mouseMoved(int x, int y) {};
	virtual void mousePressed(int x, int y, int button) {};
	virtual void mouseReleased(int x, int y, int button) {};
	virtual void update() {};
	virtual void windowResized(int w, int h) {};
};

class QuitScene :public Scene {
public:
	QuitScene(std::unique_ptr<SettingParameter>&& _setting_parameter);
	void update() { std::exit(0); };
	void draw() {};
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
	SceneIdx getNextScene() {
		return title_scene;
	}
	bool canChangeScene() {
		return false;
	}
	void mousePressed(int x, int y, int button) {};
private:
	std::unique_ptr<SettingParameter> setting_parameter;
};

class TitleScene :public Scene {
public:
	TitleScene(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~TitleScene();
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
	SceneIdx getNextScene() {
		return next_scene;
	}
	bool canChangeScene() {
		return can_change_scene;
	}
	void mousePressed(int x, int y, int button) {};
private:
	void update();
	void draw();
	void keyPressed(int key);
	void feadout(int mun) {};

	int transition_counter = 0;
	int transition_time = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	int choice_idx; //play = 0, setting, quit,
	int push_counter;

	std::unique_ptr<ofTrueTypeFont> SourceHanSans, SourceHanSans_big;
	std::unique_ptr<SettingParameter> setting_parameter;
	SceneIdx next_scene = game_scene1;
};

class SettingScene :public Scene {
public:
	SettingScene(std::unique_ptr<SettingParameter> _setting_parameter);
	~SettingScene();
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
	SceneIdx getNextScene() {
		return next_scene;
	}
	bool canChangeScene() {
		return can_change_scene;
	}
	void mousePressed(int x, int y, int button) {};
private:
	void update();
	void draw();
	void keyPressed(int key);
	void feadout(int mun) {};
	void toggle_fullscreen();

	int transition_counter = 0;
	int transition_time = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	int choice_idx; //play = 0, setting, quit,
	int push_counter, push_counter2;

	std::unique_ptr<ofTrueTypeFont> SourceHanSans;
	std::unique_ptr<SettingParameter> setting_parameter;
	SceneIdx next_scene = title_scene;
};
