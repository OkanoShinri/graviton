#include "GameScene.h"

GameScene::GameScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;
	transition_counter = 0;
	transition_time = 180;

	my_ship = std::make_unique<MyShip>();
	attraction_point.reset();
	repulsion_point.reset();

	verdana = std::make_unique<ofTrueTypeFont>();
	verdana->load("verdana.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	shot_se = std::make_unique<ofSoundPlayer>();
	shot_se->load("shotse01.mp3");
	shot_se->setVolume(setting_parameter->se_volume);
	shot_se->setMultiPlay(true);

	game_bgm = std::make_unique<ofSoundPlayer>();
	std::string bgms[] = { "Ž‡‰‘_-’Ç‰¯-_2.mp3","ƒ€ƒXƒJƒŠ‚Ì‰Ô.mp3" };
	int idx = std::rand() % 2;
	game_bgm->load(bgms[idx]);
	game_bgm->setLoop(true);
	game_bgm->setVolume(setting_parameter->bgm_volume);
	game_bgm->play();
}

GameScene::~GameScene()
{
	std::cout << "Remove: GameScene" << std::endl;
}

void GameScene::update()
{
	if (is_transiting) {
		game_bgm->setVolume(ofMap(transition_counter, 0.0, transition_time, setting_parameter->bgm_volume, 0.0));

		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}

	//-------my_ship update------------
	if (!attraction_point) {
		my_ship->resetAttraction();
	}
	else {
		my_ship->addAttraction(attraction_point->getPos());
	}
	if (!repulsion_point) {
		my_ship->resetRepulsion();
	}
	else {
		my_ship->addRepulsion(repulsion_point->getPos());
	}
	my_ship->update();
	my_ship->draw();

	//-------back ground------------
	back_ground->updata();

	counter++;
}

void GameScene::draw()
{
	ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	ofScale(setting_parameter->scale, setting_parameter->scale);

	ofSetColor(255, 255, 255);
	back_ground->draw();

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 4, setting_parameter->window_height);
	ofDrawRectangle(setting_parameter->window_width * 3 / 4, 0, setting_parameter->window_width / 4, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 4 - 5, setting_parameter->window_height);
	ofDrawRectangle(setting_parameter->window_width * 3 / 4 + 5, 0, setting_parameter->window_width / 4, setting_parameter->window_height);

	(is_transiting) ? ofSetColor(255, 10, 10) : ofSetColor(0, 0, 0);
	char minute[3];
	sprintf_s(minute, "%02d", finish_time / 3600);
	char second[3];
	sprintf_s(second, "%02d", (finish_time / 60) % 60);
	char m_second[3];
	sprintf_s(m_second, "%02d", (int)ofMap(finish_time % 60, 0, 60, 0, 99));
	verdana->drawString("" + std::string(minute) + ":" + std::string(second) + "." + std::string(m_second), 50, setting_parameter->window_height / 4);

	ofSetColor(0, 0, 0);
	if (game_bgm->isPlaying())
	{
		std::ostringstream bgm_param;
		bgm_param << setting_parameter->bgm_volume;
		ofDrawBitmapString("bgm: " + bgm_param.str(), 100, setting_parameter->window_height / 2);
	}

	std::ostringstream se_param;
	se_param << setting_parameter->se_volume;
	ofDrawBitmapString("se:  " + se_param.str(), 100, setting_parameter->window_height / 2 + 50);

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, setting_parameter->window_height - 50);

	//-------transition_in------------
	if (counter < 30)
	{
		ofSetColor(255, 255, 255, ofMap(counter, 0, 30, 255, 0));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
	//-------objects-----------------
	if (attraction_point != nullptr) {
		attraction_point->draw();
	}
	if (repulsion_point != nullptr) {
		repulsion_point->draw();
	}
	my_ship->draw();

	//-------transition_out-----------
	if (is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
	else
	{
		finish_time = counter;
	}
}

void GameScene::keyPressed(int key) {
	switch (key) {
	case 'q':

		can_change_scene = true;
		break;
	}
}

void GameScene::keyReleased(int key)
{
}

void GameScene::mouseMoved(int x, int y)
{
}

void GameScene::mousePressed(int x, int y, int button)
{
	if (button == 0) {
		if (repulsion_point != nullptr) {
			repulsion_point.reset();
		}

		if (!attraction_point) {
			attraction_point = std::make_unique<AttractionPoint>(x, y);
		}
		else if ((x - attraction_point->getPos().x)*(x - attraction_point->getPos().x) + (y - attraction_point->getPos().y)*(y - attraction_point->getPos().y) < 100) {
			attraction_point.reset();
		}
		else
		{
			attraction_point->setPos(x, y);
		}
	}
	else if (button == 2) {
		if (attraction_point != nullptr) {
			attraction_point.reset();
		}

		if (!repulsion_point) {
			repulsion_point = std::make_unique<RepulsionPoint>(x, y);
		}
		else if ((x - repulsion_point->getPos().x)*(x - repulsion_point->getPos().x) + (y - repulsion_point->getPos().y)*(y - repulsion_point->getPos().y) < 100) {
			repulsion_point.reset();
		}
		else
		{
			repulsion_point->setPos(x, y);
		}
	}
}