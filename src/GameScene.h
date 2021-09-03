#pragma once

#include "Scene.h"
#include "GameObject.h"
#include "BackGroundImage.h"
#include "Component.h"

constexpr auto MAX_BULLETS_NUM = 3000;

class GameScene :
	public Scene
{
public:
	GameScene(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~GameScene();
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
	SceneIdx getNextScene() {
		return next_scene;
	}
	bool canChangeScene() {
		return can_change_scene;
	}
private:
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void update();

	enum State {
		opening = 0, play, game_over, ending
	};
	State game_state;
	std::unique_ptr<Timer> timer;
	int counter = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	bool attraction_or_repulsion = false;
	std::unique_ptr<SettingParameter> setting_parameter;
	std::unique_ptr<BackGroundImage> back_ground;
	std::unique_ptr<MyShip> my_ship;
	std::unique_ptr<AttractionPoint> attraction_point;
	std::unique_ptr<RepulsionPoint> repulsion_point;
	std::list< std::unique_ptr<Target> > targets;
	std::unique_ptr<ofSoundPlayer> game_bgm, shot_se;
	SceneIdx next_scene = title_scene;
	std::shared_ptr<ofTrueTypeFont> SourceHanSans;
};

class GameScene2 :
	public Scene
{
public:
	GameScene2(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~GameScene2();
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
	SceneIdx getNextScene() {
		return next_scene;
	}
	bool canChangeScene() {
		return can_change_scene;
	}
private:
	void draw();
	void feadout(int mun) {};
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void update();

	int counter = 0;
	int finish_time = 0;
	int transition_counter = 0;
	int transition_time = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	bool attraction_or_repulsion = false;
	std::unique_ptr<SettingParameter> setting_parameter;
	std::unique_ptr<BackGroundImage> back_ground;
	std::unique_ptr<MyShip> my_ship;
	std::unique_ptr<AttractionPoint> attraction_point;
	std::unique_ptr<RepulsionPoint> repulsion_point;
	std::list< std::unique_ptr<Target> > targets;
	std::unique_ptr<ofSoundPlayer> game_bgm, shot_se;
	SceneIdx next_scene = title_scene;
	std::unique_ptr<ofTrueTypeFont> SourceHanSans;
};
