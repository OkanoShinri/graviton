#include "GameScene.h"

GameScene::GameScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;

	timer = std::make_unique<Timer>();

	my_ship = std::make_unique<MyShip>();
	attraction_point.reset();
	repulsion_point.reset();

	for (int i = 0; i < 10; i++)
	{
		targets.emplace_back(std::make_unique<Target>(i));
	}

	game_state = opening;

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	shot_se = std::make_unique<ofSoundPlayer>();
	shot_se->load("shotse01.mp3");
	shot_se->setVolume(setting_parameter->se_volume);
	shot_se->setMultiPlay(true);

	game_bgm = std::make_unique<ofSoundPlayer>();
	std::string bgms[] = { "紫苑_-追憶-_2.mp3","ムスカリの花.mp3" };
	int idx = std::rand() % 2;
	game_bgm->load(bgms[idx]);
	game_bgm->setLoop(true);
	game_bgm->setVolume(setting_parameter->bgm_volume);
}

GameScene::~GameScene()
{
	std::cout << "Remove: GameScene" << std::endl;
}

void GameScene::update()
{
	back_ground->updata();
	timer->update();

	switch (game_state)
	{
	case GameScene::opening:
		if (counter == 120) {
			game_state = play;
			counter = 0;
			timer->start();
			game_bgm->play();
		}
		break;
	case GameScene::play:
		if (attraction_or_repulsion && attraction_point) {
			my_ship->addAttraction(attraction_point->getPos());
		}
		else {
			my_ship->resetAttraction();
		}
		if (!attraction_or_repulsion && repulsion_point) {
			my_ship->addRepulsion(repulsion_point->getPos());
		}
		else {
			my_ship->resetRepulsion();
		}

		my_ship->update();
		my_ship->draw();

		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->update(my_ship->getPos());
			if ((*it)->canRemove())
			{
				it = this->targets.erase(it);
			}
			else {
				++it;
			}
		}

		if (targets.empty())
		{
			game_state = ending;
			timer->stop();
			counter = 0;
		}
		break;
	case GameScene::game_over:
		break;
	case GameScene::ending:
		game_bgm->setVolume(ofMap(counter, 0.0, 180, setting_parameter->bgm_volume, 0.0)); //180fでフェードアウト
		if (counter == 180) {
			can_change_scene = true;
		}
		break;
	default:
		break;
	}

	counter++;

}

void GameScene::draw()
{
	
	//ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	//ofScale(setting_parameter->scale, setting_parameter->scale);
	//back_ground->draw();
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	switch (game_state)
	{
	case GameScene::opening:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 120, 255, 0));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
		break;
	case GameScene::play:
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw();
			it++;
		}

		if (attraction_or_repulsion && attraction_point) {
			attraction_point->draw();
		}
		if (!attraction_or_repulsion && repulsion_point) {
			repulsion_point->draw();
		}
		my_ship->draw();
		break;
	case GameScene::game_over:
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw();
			it++;
		}

		if (attraction_or_repulsion && attraction_point) {
			attraction_point->draw();
		}
		if (!attraction_or_repulsion && repulsion_point) {
			repulsion_point->draw();
		}
		my_ship->draw();
		break;
	case GameScene::ending:
		break;
	default:
		break;
	}

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5 - 5, setting_parameter->window_height);

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

	timer->draw_time(30, 100, SourceHanSans);

}

void GameScene::keyPressed(int key) {
	switch (key) {
	case 'q':
		next_scene = title_scene;
		can_change_scene = true;
		break;
	case 'r':
		next_scene = game_scene1;
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
		attraction_or_repulsion = !attraction_or_repulsion;
		if (attraction_or_repulsion) {
			if (!attraction_point) {
				attraction_point = std::make_unique<AttractionPoint>(x, y);
			}
			else if ((x - attraction_point->getPos().x)*(x - attraction_point->getPos().x) + (y - attraction_point->getPos().y)*(y - attraction_point->getPos().y) < 100) {
				//attraction_point.reset();
			}
			else
			{
				attraction_point->setPos(x, y);
			}
		}
		else {
			if (!repulsion_point) {
				repulsion_point = std::make_unique<RepulsionPoint>(x, y);
			}
			else if ((x - repulsion_point->getPos().x)*(x - repulsion_point->getPos().x) + (y - repulsion_point->getPos().y)*(y - repulsion_point->getPos().y) < 100) {
				//repulsion_point.reset();
			}
			else
			{
				repulsion_point->setPos(x, y);
			}
		}
	}
	else if (button == 2) {
		next_scene = game_scene2;
		can_change_scene = true;
	}
}

GameScene2::GameScene2(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;

	timer = std::make_unique<Timer>();

	my_ship = std::make_unique<MyShip>();
	attraction_point.reset();
	repulsion_point.reset();

	for (int i = 0; i < 10; i++)
	{
		targets.emplace_back(std::make_unique<Target>(i));
	}

	game_state = opening;

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	shot_se = std::make_unique<ofSoundPlayer>();
	shot_se->load("shotse01.mp3");
	shot_se->setVolume(setting_parameter->se_volume);
	shot_se->setMultiPlay(true);

	game_bgm = std::make_unique<ofSoundPlayer>();
	std::string bgms[] = { "紫苑_-追憶-_2.mp3","ムスカリの花.mp3" };
	int idx = std::rand() % 2;
	game_bgm->load(bgms[idx]);
	game_bgm->setLoop(true);
	game_bgm->setVolume(setting_parameter->bgm_volume);
}

GameScene2::~GameScene2()
{
	std::cout << "Remove: GameScene2" << std::endl;
}

void GameScene2::update()
{
	back_ground->updata();
	timer->update();

	switch (game_state)
	{
	case GameScene2::opening:
		if (counter == 120) {
			game_state = play;
			counter = 0;
			timer->start();
			game_bgm->play();
		}
		break;
	case GameScene2::play:
		if (attraction_or_repulsion && attraction_point) {
			my_ship->addAttraction(attraction_point->getPos());
		}
		else {
			my_ship->resetAttraction();
		}
		if (!attraction_or_repulsion && repulsion_point) {
			my_ship->addRepulsion(repulsion_point->getPos());
		}
		else {
			my_ship->resetRepulsion();
		}

		my_ship->update();
		my_ship->draw();

		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->update(my_ship->getPos());
			if ((*it)->canRemove())
			{
				it = this->targets.erase(it);
			}
			else {
				++it;
			}
		}

		if (targets.empty())
		{
			game_state = ending;
			timer->stop();
			counter = 0;
		}
		break;
	case GameScene2::game_over:
		break;
	case GameScene2::ending:
		game_bgm->setVolume(ofMap(counter, 0.0, 180, setting_parameter->bgm_volume, 0.0)); //180fでフェードアウト
		if (counter == 180) {
			can_change_scene = true;
		}
		break;
	default:
		break;
	}

	counter++;

}

void GameScene2::draw()
{

	//ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	//ofScale(setting_parameter->scale, setting_parameter->scale);
	//back_ground->draw();
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	switch (game_state)
	{
	case GameScene2::opening:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 120, 255, 0));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
		break;
	case GameScene2::play:
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw();
			it++;
		}

		if (attraction_or_repulsion && attraction_point) {
			attraction_point->draw();
		}
		if (!attraction_or_repulsion && repulsion_point) {
			repulsion_point->draw();
		}
		my_ship->draw();
		break;
	case GameScene2::game_over:
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw();
			it++;
		}

		if (attraction_or_repulsion && attraction_point) {
			attraction_point->draw();
		}
		if (!attraction_or_repulsion && repulsion_point) {
			repulsion_point->draw();
		}
		my_ship->draw();
		break;
	case GameScene2::ending:
		break;
	default:
		break;
	}

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5 - 5, setting_parameter->window_height);

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

	timer->draw_time(30, 100, SourceHanSans);

}

void GameScene2::keyPressed(int key) {
	switch (key) {
	case 'q':
		next_scene = title_scene;
		can_change_scene = true;
		break;
	case 'r':
		next_scene = game_scene1;
		can_change_scene = true;
		break;
	}
}

void GameScene2::keyReleased(int key)
{
}

void GameScene2::mouseMoved(int x, int y)
{
}

void GameScene2::mousePressed(int x, int y, int button)
{
	if (button == 0) {
		attraction_or_repulsion = !attraction_or_repulsion;
		if (attraction_or_repulsion) {
			if (!attraction_point) {
				attraction_point = std::make_unique<AttractionPoint>(x, y);
			}
			else if ((x - attraction_point->getPos().x)*(x - attraction_point->getPos().x) + (y - attraction_point->getPos().y)*(y - attraction_point->getPos().y) < 100) {
				//attraction_point.reset();
			}
			else
			{
				attraction_point->setPos(x, y);
			}
		}
		else {
			if (!repulsion_point) {
				repulsion_point = std::make_unique<RepulsionPoint>(x, y);
			}
			else if ((x - repulsion_point->getPos().x)*(x - repulsion_point->getPos().x) + (y - repulsion_point->getPos().y)*(y - repulsion_point->getPos().y) < 100) {
				//repulsion_point.reset();
			}
			else
			{
				repulsion_point->setPos(x, y);
			}
		}
	}
	else if (button == 2) {
		next_scene = game_scene2;
		can_change_scene = true;
	}
}
