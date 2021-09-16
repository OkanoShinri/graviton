#pragma once

#include "Scene.h"
#include "GameObject.h"
#include "BackGroundImage.h"
#include "Component.h"

constexpr auto MAX_BULLETS_NUM = 3000;

class GameScene01 :
	public Scene
{
public:
	GameScene01(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~GameScene01();
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
	bool attraction_or_repulsion = false;
	bool can_change_scene = false;
	bool is_transiting = false;
	float sum_x;
	int counter;
	ofEasyCam cam;
	SceneIdx next_scene = game_scene02;
	std::list< std::unique_ptr<Obstacle> > obstacles;
	std::list< std::unique_ptr<Wall> > walls;
	std::shared_ptr<ofTrueTypeFont> SourceHanSans;
	std::unique_ptr<AttractionPoint> attraction_point;
	std::unique_ptr<BackGroundImage> back_ground;
	std::unique_ptr<MyShip> my_ship;
	std::unique_ptr<ofSoundPlayer> game_bgm, att_clk_se, rep_clk_se;
	std::unique_ptr<RepulsionPoint> repulsion_point;
	std::unique_ptr<SettingParameter> setting_parameter;
	std::unique_ptr<Timer> timer;
};

class GameScene02 :
	public Scene
{
public:
	GameScene02(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~GameScene02();
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
	bool attraction_or_repulsion = false;
	bool can_change_scene = false;
	bool is_transiting = false;
	float sum_x;
	int counter;
	ofEasyCam cam;
	SceneIdx next_scene = game_scene03;
	std::list< std::unique_ptr<Obstacle> > obstacles;
	std::list< std::unique_ptr<Wall> > walls;
	std::shared_ptr<ofTrueTypeFont> SourceHanSans;
	std::unique_ptr<AttractionPoint> attraction_point;
	std::unique_ptr<BackGroundImage> back_ground;
	std::unique_ptr<MyShip> my_ship;
	std::unique_ptr<ofSoundPlayer> game_bgm, att_clk_se, rep_clk_se;
	std::unique_ptr<RepulsionPoint> repulsion_point;
	std::unique_ptr<SettingParameter> setting_parameter;
	std::unique_ptr<Timer> timer;
};

class GameScene03 :
	public Scene
{
public:
	GameScene03(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~GameScene03();
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
	SceneIdx next_scene = game_scene03;
	std::shared_ptr<ofTrueTypeFont> SourceHanSans;
};