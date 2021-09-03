#include "Scene.h"

QuitScene::QuitScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);
}

TitleScene::TitleScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);
	can_change_scene = false;
	next_scene = Scene::menu_scene;
	game_state = TitleScene::opening;

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans_big = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	SourceHanSans_big->load("SourceHanSans001.ttf", 60);
}

TitleScene::~TitleScene()
{
	std::cout << "Remove: TitleScene" << std::endl;
}

void TitleScene::update()
{
	switch (game_state)
	{
	case TitleScene::opening:
		if (counter > 0) {
			game_state = TitleScene::play;
			counter = 0;
		}
		break;
	case TitleScene::play:
		break;
	case TitleScene::ending:
		if (counter == 60) {
			next_scene = Scene::menu_scene;
			can_change_scene = true;
		}
		break;
	default:
		break;
	}
	counter++;

}

void TitleScene::draw()
{
	ofSetColor(255,255,255);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofSetColor(0, 0, 0);
	SourceHanSans_big->drawString("graviton", 400, 300);

}

void TitleScene::keyPressed(int key) {
	if (game_state != TitleScene::ending) {
		game_state = TitleScene::ending;
		counter = 0;
	}
}

MenuScene::MenuScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);
	transition_counter = 0;
	transition_time = 60;
	can_change_scene = false;
	is_transiting = false;
	next_scene = Scene::game_scene1;

	choice_idx = 0;
	push_counter = 0;

	std::vector<SceneIdx> data{ game_scene1,game_scene2 };
	std::vector<std::string> data_str{ "room1","room2" };

	game_selector = std::make_unique<Selector<SceneIdx>>(data, data_str, true);

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans_big = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	SourceHanSans_big->load("SourceHanSans001.ttf", 60);
}

MenuScene::~MenuScene()
{
	std::cout << "Remove: TitleScene" << std::endl;
}

void MenuScene::update()
{
	if (is_transiting) {
		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}


	switch (choice_idx)
	{
	case 0:
		this->next_scene = Scene::game_scene1;
		break;
	case 1:
		this->next_scene = Scene::setting_scene;
		break;
	case 2:
		this->next_scene = Scene::quit_scene;
		break;
	default:
		break;
	}
}

void MenuScene::draw()
{
	ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	ofScale(setting_parameter->scale, setting_parameter->scale);


	ofVec2f start_pos = ofVec2f(setting_parameter->window_width / 4, setting_parameter->window_height * 3 / 4);
	ofVec2f setting_pos = ofVec2f(setting_parameter->window_width / 2, setting_parameter->window_height * 3 / 4);
	ofVec2f quit_pos = ofVec2f(setting_parameter->window_width * 3 / 4, setting_parameter->window_height * 3 / 4);

	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	SourceHanSans_big->drawString("**Insert Title**", 400, 300);
	std::string version = "ver ";
	version += setting_parameter->version;
	SourceHanSans->drawString(version, 850, 350);
	SourceHanSans->drawString("Start", start_pos.x, start_pos.y);
	SourceHanSans->drawString("Setting", setting_pos.x, setting_pos.y);
	SourceHanSans->drawString("Quit", quit_pos.x, quit_pos.y);

	game_selector->draw(400, 700, SourceHanSans);

	int size_of_r = 15;
	ofPushMatrix();
	switch (choice_idx)
	{
	case 0:
		ofTranslate(start_pos.x - 30, start_pos.y - size_of_r, 0);
		break;
	case 1:
		ofTranslate(setting_pos.x - 30, setting_pos.y - size_of_r, 0);
		break;
	case 2:
		ofTranslate(quit_pos.x - 30, quit_pos.y - size_of_r, 0);
		break;

	default:
		break;
	}
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(0, 0, size_of_r);
	ofSetLineWidth(1.0);
	ofFill();
	ofDrawTriangle(size_of_r, 0, size_of_r * cos(2 * PI / 3), size_of_r * sin(2 * PI / 3), size_of_r * cos(-2 * PI / 3), size_of_r * sin(-2 * PI / 3));
	ofPopMatrix();
	//-------transition_out-----------
	if (this->is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
}

void MenuScene::keyPressed(int key) {
	if (this->is_transiting) {
		return;
	}

	switch (key) {
	case OF_KEY_LEFT:
		choice_idx = (choice_idx + 2) % 3;
		break;
	case OF_KEY_RIGHT:
		choice_idx = (choice_idx + 1) % 3;
		break;
	case OF_KEY_RETURN:
		this->is_transiting = true;
		this->transition_counter = 0;
		break;
	}
}


SettingScene::SettingScene(std::unique_ptr<SettingParameter> _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	transition_counter = 0;
	transition_time = 30;
	can_change_scene = false;
	is_transiting = false;
	next_scene = Scene::title_scene;

	choice_idx = 0;
	push_counter = 0;
	push_counter2 = 0;
	SourceHanSans = std::make_unique<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
}

SettingScene::~SettingScene()
{
	std::cout << "Remove: SettingScene" << std::endl;
}

void SettingScene::update()
{
	if (is_transiting) {
		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}
}

void SettingScene::draw()
{
	ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	ofScale(setting_parameter->scale, setting_parameter->scale);

	ofVec2f volume_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 3 / 10);
	ofVec2f se_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 4 / 10);
	ofVec2f fullscreen_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 5 / 10);
	ofVec2f num_of_balls_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 6 / 10);
	ofVec2f return_pos = ofVec2f(setting_parameter->window_width / 2 - 30, setting_parameter->window_height * 5 / 6);

	float bar_left = setting_parameter->window_width * 4 / 8;
	float bar_right = setting_parameter->window_width * 7 / 8;

	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	SourceHanSans->drawString("Setting", setting_parameter->window_width / 2 - 30, 50);

	SourceHanSans->drawString("BGM Volume", volume_pos.x, volume_pos.y);
	ofSetLineWidth(2.0);
	ofDrawLine(bar_left, volume_pos.y, bar_right, volume_pos.y);
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(ofMap(setting_parameter->bgm_volume, 0, 1, bar_left, bar_right), volume_pos.y, 15);
	ofFill();
	ofSetLineWidth(1.0);
	ofDrawCircle(ofMap(setting_parameter->bgm_volume, 0, 1, bar_left, bar_right), volume_pos.y, 10);

	SourceHanSans->drawString("SE Volume", se_pos.x, se_pos.y);
	ofSetLineWidth(2.0);
	ofDrawLine(bar_left, se_pos.y, bar_right, se_pos.y);
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(ofMap(setting_parameter->se_volume, 0, 1, bar_left, bar_right), se_pos.y, 15);
	ofFill();
	ofSetLineWidth(1.0);
	ofDrawCircle(ofMap(setting_parameter->se_volume, 0, 1, bar_left, bar_right), se_pos.y, 10);

	SourceHanSans->drawString("Full Screen", fullscreen_pos.x, fullscreen_pos.y);
	SourceHanSans->drawString("ON", fullscreen_pos.x * 4, fullscreen_pos.y);
	SourceHanSans->drawString("OFF", fullscreen_pos.x * 6, fullscreen_pos.y);
	if (setting_parameter->is_fullscreen) {
		ofDrawRectangle(fullscreen_pos.x * 4 - 15, fullscreen_pos.y - 40, 75, 50);
		SourceHanSans->drawString("OFF", fullscreen_pos.x * 6, fullscreen_pos.y);
		ofSetColor(255, 255, 255);
		SourceHanSans->drawString("ON", fullscreen_pos.x * 4, fullscreen_pos.y);
	}
	else {
		ofDrawRectangle(fullscreen_pos.x * 6 - 20, fullscreen_pos.y - 40, 100, 50);
		SourceHanSans->drawString("ON", bar_left, fullscreen_pos.y);
		ofSetColor(255, 255, 255);
		SourceHanSans->drawString("OFF", fullscreen_pos.x * 6, fullscreen_pos.y);
	}
	ofSetColor(0, 0, 0);

	SourceHanSans->drawString("Number of balls", num_of_balls_pos.x, num_of_balls_pos.y);
	SourceHanSans->drawString(std::to_string(setting_parameter->num_ball), num_of_balls_pos.x * 5, num_of_balls_pos.y);

	SourceHanSans->drawString("Return", return_pos.x, return_pos.y);

	//----------cursor----------------
	int size_of_cursor_r = 15;
	ofPushMatrix();
	switch (choice_idx)
	{
	case 0:
		ofTranslate(volume_pos.x - 30, volume_pos.y - size_of_cursor_r, 0);
		break;
	case 1:
		ofTranslate(se_pos.x - 30, se_pos.y - size_of_cursor_r, 0);
		break;
	case 2:
		ofTranslate(fullscreen_pos.x - 30, fullscreen_pos.y - size_of_cursor_r, 0);
		break;
	case 3:
		ofTranslate(num_of_balls_pos.x - 30, num_of_balls_pos.y - size_of_cursor_r, 0);
		break;
	case 4:
		ofTranslate(return_pos.x - 30, return_pos.y - size_of_cursor_r, 0);
		break;

	default:
		break;
	}
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(0, 0, size_of_cursor_r);
	ofFill();
	ofSetLineWidth(1.0);
	ofDrawTriangle(size_of_cursor_r, 0, size_of_cursor_r * cos(2 * PI / 3), size_of_cursor_r * sin(2 * PI / 3), size_of_cursor_r * cos(-2 * PI / 3), size_of_cursor_r * sin(-2 * PI / 3));
	ofPopMatrix();

	//-------transition_out-----------
	if (this->is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
}

void SettingScene::keyPressed(int key)
{
	if (this->is_transiting) {
		return;
	}

	switch (key) {
	case OF_KEY_DOWN:
		choice_idx = (choice_idx + 1) % 5;
		break;
	case OF_KEY_UP:
		choice_idx = (choice_idx + 4) % 5;
		break;
	case OF_KEY_LEFT:
		if (choice_idx == 0)
		{
			setting_parameter->bgm_volume -= 0.02;
		}
		else if (choice_idx == 1)
		{
			setting_parameter->se_volume -= 0.02;
		}
		else if (choice_idx == 2)
		{
			toggle_fullscreen();
		}
		else if (choice_idx == 3)
		{
			setting_parameter->num_ball--;
		}
		break;
	case OF_KEY_RIGHT:
		if (choice_idx == 0)
		{
			setting_parameter->bgm_volume += 0.02;
		}
		else if (choice_idx == 1)
		{
			setting_parameter->se_volume += 0.02;
		}
		else if (choice_idx == 2)
		{
			toggle_fullscreen();
		}
		else if (choice_idx == 3)
		{
			setting_parameter->num_ball++;
		}
		break;
	case OF_KEY_RETURN:
		if (choice_idx == 2)
		{
			toggle_fullscreen();
		}
		else if (choice_idx == 4)
		{
			this->is_transiting = true;
			this->transition_counter = 0;
		}

		break;
	}
	if (setting_parameter->num_ball < 1)
	{
		setting_parameter->num_ball = 1;
	}
	else if (setting_parameter->MAX_BALL < setting_parameter->num_ball)
	{
		setting_parameter->num_ball = setting_parameter->MAX_BALL;
	}
}

void SettingScene::toggle_fullscreen()
{
	assert(setting_parameter != nullptr);
	ofToggleFullscreen();
	setting_parameter->is_fullscreen = !setting_parameter->is_fullscreen;
	float w = ofGetWindowWidth(), h = ofGetWindowHeight();
	setting_parameter->scale = std::min(w / 1024, h / 768);
	setting_parameter->offset_x = (w - 1024 * setting_parameter->scale) / 2;
	setting_parameter->offset_y = (h - 768 * setting_parameter->scale) / 2;
}
