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
	counter = 0;

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
		if (counter == 60) {
			game_state = TitleScene::play;
			counter = 0;
		}
		break;
	case TitleScene::play:
		break;
	case TitleScene::ending:
		if (counter >= 0) {
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
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	switch (game_state)
	{
	case TitleScene::opening:
	{
		ofSetColor(0, 0, 0, ofMap(counter, 0, 60, 0, 255));
		SourceHanSans_big->drawString("graviton", 370, 400);
		std::string version = "ver ";
		version += setting_parameter->version;
		SourceHanSans->drawString(version, 850, 350);

		break;
	}
	case TitleScene::play:
	case TitleScene::ending:
	{
		ofSetColor(0, 0, 0);
		SourceHanSans_big->drawString("graviton", 370, 400);
		std::string version = "ver ";
		version += setting_parameter->version;
		SourceHanSans->drawString(version, 850, 350);
		break;
	}
	default:
		break;
	}
}

void TitleScene::keyPressed(int key) {
	if (game_state != TitleScene::ending) {
		game_state = TitleScene::ending;
		counter = 0;
	}
}

void TitleScene::mousePressed(int x, int y, int button)
{
	/*SourceHanSans_big->drawString("graviton", x, y);
	std::string x_s, y_s;
	x_s = std::to_string(x);
	y_s = std::to_string(y);
	ofDrawBitmapString("Quit:  " + x_s + "," + y_s, 600, 50);*/
}

MenuScene::MenuScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;
	next_scene = Scene::game_scene01;

	choice_idx = 0;
	game_state = opening;
	counter = 0;

	std::vector<SceneIdx> data{ game_scene01,game_scene02,game_scene03 };
	std::vector<std::string> data_str{ "room1","room2", "room3" };

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

void MenuScene::mousePressed(int x, int y, int button)
{
	/*SourceHanSans->drawString("room1", x, y);
	std::string x_s, y_s;
	x_s = std::to_string(x);
	y_s = std::to_string(y);
	ofDrawBitmapString("Quit:  " + x_s + "," + y_s, 600, 50);*/
}

void MenuScene::update()
{
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	ofSetColor(0, 0, 0);

	switch (game_state)
	{
	case MenuScene::opening:
		if (counter > 60) {
			game_state = MenuScene::play;
			counter = 0;
		}
		break;
	case MenuScene::play:
		switch (choice_idx)
		{
		case 0:
			break;
		case 1:
			next_scene = setting_scene;
			break;
		case 2:
			next_scene = quit_scene;
			break;
		default:
			break;
		}
		break;
	case MenuScene::ending:
		if (counter == 60) {
			can_change_scene = true;
		}
		break;
	default:
		break;
	}
	counter++;
}

void MenuScene::draw()
{
	switch (game_state)
	{
	case MenuScene::opening:
	{
		ofSetColor(0, 0, 0);
		SourceHanSans_big->drawString("graviton", 370, 300 + 100 * cos(counter / 60.0 *PI));

		ofSetColor(0, 0, 0, ofMap(counter, 0, 60, 0, 255));
		game_selector->draw(455, 417, SourceHanSans);
		SourceHanSans->drawString("Setting", 444, 550);
		SourceHanSans->drawString("Quit", 467, 672);

		int size_of_r = 15;
		ofPushMatrix();
		switch (choice_idx)
		{
		case 0:
			ofTranslate(455 - 30, 417 - size_of_r, 0);
			break;
		case 1:
			ofTranslate(444 - 30, 550 - size_of_r, 0);
			break;
		case 2:
			ofTranslate(467 - 30, 672 - size_of_r, 0);
			break;
		default:
			break;
		}
		ofNoFill();
		ofSetLineWidth(1.0);
		ofDrawCircle(0, 0, size_of_r);
		ofFill();
		ofDrawTriangle(size_of_r, 0, size_of_r * cos(2 * PI / 3), size_of_r * sin(2 * PI / 3), size_of_r * cos(-2 * PI / 3), size_of_r * sin(-2 * PI / 3));
		ofPopMatrix();
		break;
	}
	case MenuScene::play:
	{
		SourceHanSans_big->drawString("graviton", 370, 200);
		game_selector->draw(455, 417, SourceHanSans);
		SourceHanSans->drawString("Setting", 444, 550);
		SourceHanSans->drawString("Quit", 467, 672);

		int size_of_r = 15;
		ofPushMatrix();
		switch (choice_idx)
		{
		case 0:
			ofTranslate(455 - 30, 417 - size_of_r, 0);
			break;
		case 1:
			ofTranslate(444 - 30, 550 - size_of_r, 0);
			break;
		case 2:
			ofTranslate(467 - 30, 672 - size_of_r, 0);
			break;
		default:
			break;
		}
		ofNoFill();
		ofSetLineWidth(1.0);
		ofDrawCircle(0, 0, size_of_r);
		ofFill();
		ofDrawTriangle(size_of_r, 0, size_of_r * cos(2 * PI / 3), size_of_r * sin(2 * PI / 3), size_of_r * cos(-2 * PI / 3), size_of_r * sin(-2 * PI / 3));
		ofPopMatrix();
		break;
	}
	case MenuScene::ending:
	{
		ofSetColor(0, 0, 0, ofMap(counter, 0, 60, 255, 0));
		SourceHanSans_big->drawString("graviton", 370, 200);
		game_selector->draw(455, 417, SourceHanSans);
		SourceHanSans->drawString("Setting", 444, 550);
		SourceHanSans->drawString("Quit", 467, 672);

		int size_of_r = 15;
		ofPushMatrix();
		switch (choice_idx)
		{
		case 0:
			ofTranslate(455 - 30, 417 - size_of_r, 0);
			break;
		case 1:
			ofTranslate(444 - 30, 550 - size_of_r, 0);
			break;
		case 2:
			ofTranslate(467 - 30, 672 - size_of_r, 0);
			break;
		default:
			break;
		}
		ofNoFill();
		ofSetLineWidth(1.0);
		ofDrawCircle(0, 0, size_of_r);
		ofFill();
		ofDrawTriangle(size_of_r, 0, size_of_r * cos(2 * PI / 3), size_of_r * sin(2 * PI / 3), size_of_r * cos(-2 * PI / 3), size_of_r * sin(-2 * PI / 3));
		ofPopMatrix();
		break;
	}
	default:
		break;
	}
}

void MenuScene::keyPressed(int key) {
	if (game_state == MenuScene::opening && key == OF_KEY_RETURN) {
		return;
	}
	else if (game_state == MenuScene::opening) {
		game_state = MenuScene::play;
	}
	else if (game_state == MenuScene::ending) {
		return;
	}

	switch (key) {
	case OF_KEY_UP:
		choice_idx = (choice_idx + 2) % 3;
		break;
	case OF_KEY_DOWN:
		choice_idx = (choice_idx + 1) % 3;
		break;
	case OF_KEY_LEFT:
		if (choice_idx == 0) {
			game_selector->changeLeft();
		}
		break;
	case OF_KEY_RIGHT:
		if (choice_idx == 0) {
			game_selector->changeRight();
		}
		break;
	case OF_KEY_RETURN:
		if (choice_idx == 0) {
			next_scene = game_selector->enter();
		}
		game_state = ending;
		counter = 0;
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

	if (setting_parameter->bgm_volume < 0) {
		setting_parameter->bgm_volume = 0;
	}
	else if (setting_parameter->bgm_volume > 1) {
		setting_parameter->bgm_volume = 1;
	}
	if (setting_parameter->se_volume < 0) {
		setting_parameter->se_volume = 0;
	}
	else if (setting_parameter->se_volume > 1) {
		setting_parameter->se_volume = 1;
	}
}

void SettingScene::draw()
{
	ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	ofScale(setting_parameter->scale, setting_parameter->scale);

	ofVec2f volume_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 3 / 10);
	ofVec2f se_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 4 / 10);
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
	

	ofSetColor(0, 0, 0);
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
		choice_idx = (choice_idx + 1) % 3;
		break;
	case OF_KEY_UP:
		choice_idx = (choice_idx + 2) % 3;
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
		break;
	case OF_KEY_RETURN:
		if (choice_idx == 2)
		{
			this->is_transiting = true;
			this->transition_counter = 0;
		}
		break;
	}

}
