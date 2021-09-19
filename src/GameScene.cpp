#include "GameScene.h"
#include <stdio.h>


//*****************game scene 01*****************//

GameScene01::GameScene01(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;

	timer = std::make_unique<Timer>();
	counter = 0;
	sum_x = 0;

	my_ship = std::make_unique<MyShip>();
	my_ship->setSpeedRate(1.5);
	my_ship->setMaxSpeed(4.0);
	attraction_point.reset();
	repulsion_point.reset();

	int stage_map_width = 500;
	int stage_map_height = 50;
	FILE *fp;
	if ((fopen_s(&fp, "data/room1.csv", "r")) == 0)
	{
		int map_x = 0, map_y = 0;
		char block;
		while ((block = fgetc(fp)) != EOF) {
			switch (block)
			{
			case '0':
				map_x++;
				break;
			case '1':
				dash_panels.emplace_back(std::make_unique<DashPanel>(map_x * 50, map_y * 50, 50, 50, 1));
				map_x++;
				break;
			case '2':
				dash_panels.emplace_back(std::make_unique<DashPanel>(map_x * 50, map_y * 50, 50, 50, 2));
				map_x++;
				break;
			case '3':
				dash_panels.emplace_back(std::make_unique<DashPanel>(map_x * 50, map_y * 50, 50, 50, 3));
				map_x++;
				break;
			case '4':
				dash_panels.emplace_back(std::make_unique<DashPanel>(map_x * 50, map_y * 50, 50, 50, 4));
				map_x++;
				break;
			case '5':
				obstacles.emplace_back(std::make_unique<Obstacle>(map_x * 50, map_y * 50, 50, 50));
				map_x++;
				break;
			case '6':
				my_ship->setClearArea(map_x * 50, map_y * 50, 50, 50);
				map_x++;
				break;
			case '\n':
				map_x = 0;
				map_y++;
				break;
			default:

				break;
			}
		}
		fclose(fp);
	}

	cam.removeAllInteractions();
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);

	cam.enableOrtho();
	cam.setNearClip(-1000000);
	cam.setFarClip(1000000);
	cam.setVFlip(true);

	cam.disableMouseInput();
	cam.disableMouseMiddleButton();

	game_state = opening;

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	att_clk_se = std::make_unique<ofSoundPlayer>();
	att_clk_se->load("マウス・シングルクリック02.mp3");
	att_clk_se->setVolume(setting_parameter->se_volume);
	att_clk_se->setMultiPlay(true);

	rep_clk_se = std::make_unique<ofSoundPlayer>();
	rep_clk_se->load("マウス・シングルクリック01.mp3");
	rep_clk_se->setVolume(setting_parameter->se_volume);
	rep_clk_se->setMultiPlay(true);

	dash_se = std::make_unique<ofSoundPlayer>();
	dash_se->load("決定、ボタン押下43.mp3");
	dash_se->setVolume(setting_parameter->se_volume);
	dash_se->setMultiPlay(false);

	//BGM
	std::ifstream ifs("data/config.txt");
	std::string str;
	if (!ifs.fail()) {
		for (int i = 0; i < 1; i++) {
			getline(ifs, str);
		}
		game_bgm = std::make_unique<ofSoundPlayer>();
		game_bgm->load(str + ".mp3");
		game_bgm->setLoop(true);
		game_bgm->setVolume(setting_parameter->bgm_volume);
	}
}

GameScene01::~GameScene01()
{
}

void GameScene01::update()
{
	back_ground->updata();
	timer->update();
	ofVec2f cam_pos = my_ship->getPos();
	switch (game_state)
	{
	case GameScene01::opening:
		cam.setPosition(ofVec3f(my_ship->getPos().x, my_ship->getPos().y, 0));
		if (counter == 60) {
			game_state = GameScene01::play;
			counter = 0;
			timer->start();
			game_bgm->play();
		}
		break;
	case GameScene01::play:
	{
		if (attraction_point) {
			if (attraction_or_repulsion) {
				my_ship->resetRepulsion();
				my_ship->addAttraction(attraction_point->getPos());
			}
		}
		else {
			my_ship->resetAttraction();
		}
		if (repulsion_point) {
			if (!attraction_or_repulsion) {
				my_ship->resetAttraction();
				my_ship->addRepulsion(repulsion_point->getPos());
			}
		}
		else {
			my_ship->resetRepulsion();
		}

		my_ship->simulate_move();

		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->update(cam_pos);
			my_ship->getBoxIntersection((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH());

			++it;
		}
		for (auto it = this->dash_panels.begin(); it != this->dash_panels.end();)
		{
			(*it)->update();
			if (my_ship->onDashPanel((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH(), (*it)->getDirection())) {
				dash_se->play();
			}

			++it;
		}

		my_ship->update();
		cam_pos = my_ship->getPos();
		cam.setPosition(ofVec3f(cam_pos.x, cam_pos.y, 0));

		//is gameover
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			if (my_ship->isInBox((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH(), cam_pos.x, cam_pos.y)) {
				game_state = GameScene01::game_over;
				timer->stop();
				counter = 0;
				break;
			}
			++it;
		}

		if (my_ship->isClear())
		{
			game_state = GameScene01::ending;
			timer->stop();
			counter = 0;
		}
		break;
	}
	case GameScene01::game_over:
		game_bgm->setVolume(ofMap(counter, 0.0, 90, setting_parameter->bgm_volume, 0.0)); //90fでフェードアウト
		if (counter == 90) {
			next_scene = GameScene01::menu_scene;
			can_change_scene = true;
		}
		break;
	case GameScene01::ending:
		my_ship->move();
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

void GameScene01::draw()
{
	back_ground->draw();
	cam.begin();
	ofVec2f cam_pos = my_ship->getPos();
	switch (game_state)
	{
	case GameScene01::opening:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 60, 0, 255));

		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		break;
	case GameScene01::play:
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		for (auto it = this->dash_panels.begin(); it != this->dash_panels.end();)
		{
			(*it)->draw(cam_pos);
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
	case GameScene01::game_over:
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		for (auto it = this->dash_panels.begin(); it != this->dash_panels.end();)
		{
			(*it)->draw(cam_pos);
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
	case GameScene01::ending:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 180, 255, 0));
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		for (auto it = this->dash_panels.begin(); it != this->dash_panels.end();)
		{
			(*it)->draw(cam_pos);
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
	default:
		break;
	}
	cam.end();

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5 - 2, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	if (game_bgm->isPlaying())
	{
		std::ostringstream bgm_param;
		bgm_param << setting_parameter->bgm_volume;
		ofDrawBitmapString("bgm: " + bgm_param.str(), 50, setting_parameter->window_height / 2 + 100);
	}

	std::ostringstream se_param;
	se_param << setting_parameter->se_volume;
	ofDrawBitmapString("se:  " + se_param.str(), 50, setting_parameter->window_height / 2 + 150);

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, setting_parameter->window_height - 50);

	timer->drawTime(30, 100, SourceHanSans);

}

void GameScene01::keyPressed(int key) {
	switch (key) {
	case 'q':
		counter = 0;
		game_state = GameScene01::game_over;
		break;
	case 'r':
		next_scene = game_scene01;
		can_change_scene = true;
		break;
	}
}

void GameScene01::keyReleased(int key)
{
}

void GameScene01::mouseMoved(int x, int y)
{
}

void GameScene01::mousePressed(int x, int y, int button)
{
	if (game_state != play) {
		return;
	}

	if (button == 0) {
		attraction_or_repulsion = !attraction_or_repulsion;
		if (attraction_or_repulsion) {
			if (!attraction_point) {
				attraction_point = std::make_unique<AttractionPoint>(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			else if ((x - attraction_point->getPos().x)*(x - attraction_point->getPos().x) + (y - attraction_point->getPos().y)*(y - attraction_point->getPos().y) < 100) {
				//attraction_point.reset();
			}
			else
			{
				attraction_point->setPos(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			att_clk_se->play();
		}
		else {
			if (!repulsion_point) {
				repulsion_point = std::make_unique<RepulsionPoint>(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			else if ((x - repulsion_point->getPos().x)*(x - repulsion_point->getPos().x) + (y - repulsion_point->getPos().y)*(y - repulsion_point->getPos().y) < 100) {
				//repulsion_point.reset();
			}
			else
			{
				repulsion_point->setPos(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			rep_clk_se->play();
		}
	}
	else if (button == 2) {
		next_scene = game_scene02;
		can_change_scene = true;
	}
}


//*****************~game scene 01*****************//

//*****************game scene 02*****************//
GameScene02::GameScene02(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;

	timer = std::make_unique<Timer>();
	counter = 0;
	sum_x = 0;

	my_ship = std::make_unique<MyShip>();
	attraction_point.reset();
	repulsion_point.reset();

	int stage_map_width = 500;
	int stage_map_height = 50;
	FILE *fp;
	if ((fopen_s(&fp, "data/room2.csv", "r")) == 0)
	{
		int map_x = 0, map_y = 0;
		char block;
		while ((block = fgetc(fp)) != EOF) {
			switch (block)
			{
			case '0':
				map_x++;
				break;
			case '1':
				obstacles.emplace_back(std::make_unique<Obstacle>(map_x * 50, map_y * 50, 50, 50));
				map_x++;
				break;
			case '2':
				obstacles.emplace_back(std::make_unique<MovingObstacle>(map_x * 50, map_y * 50, 50, 50, map_x * 50 + 400, map_y * 50));
				map_x++;
				break;
			case '3':
				obstacles.emplace_back(std::make_unique<MovingObstacle>(map_x * 50, map_y * 50, 50, 50, map_x * 50, map_y * 50 + 400));
				map_x++;
				break;
			case '4':
				my_ship->setClearArea(map_x * 50, map_y * 50, 50, 50);
				map_x++;
				break;
			case '\n':
				map_x = 0;
				map_y++;
				break;
			default:

				break;
			}
		}
		fclose(fp);
	}

	cam.removeAllInteractions();
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);

	cam.enableOrtho();
	cam.setNearClip(-1000000);
	cam.setFarClip(1000000);
	cam.setVFlip(true);

	cam.disableMouseInput();
	cam.disableMouseMiddleButton();

	game_state = opening;

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	att_clk_se = std::make_unique<ofSoundPlayer>();
	att_clk_se->load("マウス・シングルクリック02.mp3");
	att_clk_se->setVolume(setting_parameter->se_volume);
	att_clk_se->setMultiPlay(true);

	rep_clk_se = std::make_unique<ofSoundPlayer>();
	rep_clk_se->load("マウス・シングルクリック01.mp3");
	rep_clk_se->setVolume(setting_parameter->se_volume);
	rep_clk_se->setMultiPlay(true);

	//BGM
	std::ifstream ifs("data/config.txt");
	std::string str;
	if (!ifs.fail()) {
		for (int i = 0; i < 2; i++) {
			getline(ifs, str);
		}
		game_bgm = std::make_unique<ofSoundPlayer>();
		game_bgm->load(str + ".mp3");
		game_bgm->setLoop(true);
		game_bgm->setVolume(setting_parameter->bgm_volume);
	}
}

GameScene02::~GameScene02()
{
}

void GameScene02::update()
{
	back_ground->updata();
	timer->update();

	switch (game_state)
	{
	case GameScene02::opening:
		cam.setPosition(ofVec3f(my_ship->getPos().x, my_ship->getPos().y, 0));
		if (counter == 60) {
			game_state = GameScene02::play;
			counter = 0;
			timer->start();
			game_bgm->play();
		}
		break;
	case GameScene02::play:
	{
		if (attraction_point) {
			if (attraction_or_repulsion) {
				my_ship->resetRepulsion();
				my_ship->addAttraction(attraction_point->getPos());
			}
		}
		else {
			my_ship->resetAttraction();
		}
		if (repulsion_point) {
			if (!attraction_or_repulsion) {
				my_ship->resetAttraction();
				my_ship->addRepulsion(repulsion_point->getPos());
			}
		}
		else {
			my_ship->resetRepulsion();
		}

		my_ship->simulate_move();

		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->update(my_ship->getPos());
			my_ship->getBoxIntersection((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH());

			++it;
		}

		my_ship->update();
		cam.setPosition(ofVec3f(my_ship->getPos().x, my_ship->getPos().y, 0));

		//is gameover
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			if (my_ship->isInBox((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH(), my_ship->getPos().x, my_ship->getPos().y)) {
				game_state = GameScene02::game_over;
				timer->stop();
				counter = 0;
				break;
			}
			++it;
		}

		if (my_ship->isClear())
		{
			game_state = GameScene02::ending;
			timer->stop();
			counter = 0;
		}
		break;
	}
	case GameScene02::game_over:
		game_bgm->setVolume(ofMap(counter, 0.0, 90, setting_parameter->bgm_volume, 0.0)); //90fでフェードアウト
		if (counter == 90) {
			next_scene = GameScene02::menu_scene;
			can_change_scene = true;
		}
		break;
	case GameScene02::ending:
		my_ship->move();
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

void GameScene02::draw()
{
	back_ground->draw();
	cam.begin();
	switch (game_state)
	{
	case GameScene02::opening:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 60, 0, 255));

		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(my_ship->getPos());
			it++;
		}
		break;
	case GameScene02::play:
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(my_ship->getPos());
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
	case GameScene02::game_over:
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(my_ship->getPos());
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
	case GameScene02::ending:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 180, 255, 0));
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(my_ship->getPos());
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
	default:
		break;
	}
	cam.end();

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5 - 2, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	if (game_bgm->isPlaying())
	{
		std::ostringstream bgm_param;
		bgm_param << setting_parameter->bgm_volume;
		ofDrawBitmapString("bgm: " + bgm_param.str(), 50, setting_parameter->window_height / 2 + 100);
	}

	std::ostringstream se_param;
	se_param << setting_parameter->se_volume;
	ofDrawBitmapString("se:  " + se_param.str(), 50, setting_parameter->window_height / 2 + 150);

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, setting_parameter->window_height - 50);

	timer->drawTime(30, 100, SourceHanSans);

	/*
	char sum_x_str[5];
	sprintf_s(sum_x_str, "%d", int(sum_x / 10));
	SourceHanSans->drawString(std::string(sum_x_str) + "m", 50, 300);
	*/


}

void GameScene02::keyPressed(int key) {
	switch (key) {
	case 'q':
		counter = 0;
		game_state = GameScene02::game_over;
		break;
	case 'r':
		next_scene = game_scene02;
		can_change_scene = true;
		break;
	}
}

void GameScene02::keyReleased(int key)
{
}

void GameScene02::mouseMoved(int x, int y)
{
}

void GameScene02::mousePressed(int x, int y, int button)
{
	if (game_state != play) {
		return;
	}

	if (button == 0) {
		attraction_or_repulsion = !attraction_or_repulsion;
		if (attraction_or_repulsion) {
			if (!attraction_point) {
				attraction_point = std::make_unique<AttractionPoint>(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			else if ((x - attraction_point->getPos().x)*(x - attraction_point->getPos().x) + (y - attraction_point->getPos().y)*(y - attraction_point->getPos().y) < 100) {
				//attraction_point.reset();
			}
			else
			{
				attraction_point->setPos(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			att_clk_se->play();
		}
		else {
			if (!repulsion_point) {
				repulsion_point = std::make_unique<RepulsionPoint>(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			else if ((x - repulsion_point->getPos().x)*(x - repulsion_point->getPos().x) + (y - repulsion_point->getPos().y)*(y - repulsion_point->getPos().y) < 100) {
				//repulsion_point.reset();
			}
			else
			{
				repulsion_point->setPos(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			rep_clk_se->play();
		}
	}
	else if (button == 2) {
		next_scene = game_scene02;
		can_change_scene = true;
	}
}

//*****************~game scene 02*****************//

//*****************game scene 03*****************//

GameScene03::GameScene03(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	can_change_scene = false;
	is_transiting = false;

	timer = std::make_unique<Timer>();
	counter = 0;
	sum_x = 0;

	my_ship = std::make_unique<MyShip>();
	my_ship->setSpeedRate(1.5);
	my_ship->setMaxSpeed(4.0);
	attraction_point.reset();
	repulsion_point.reset();

	int stage_map_width = 500;
	int stage_map_height = 50;
	FILE *fp;
	if ((fopen_s(&fp, "data/room3.csv", "r")) == 0)
	{
		int map_x = 0, map_y = 0;
		char block;
		while ((block = fgetc(fp)) != EOF) {
			switch (block)
			{
			case '0':
				map_x++;
				break;
			case '1':
				obstacles.emplace_back(std::make_unique<Obstacle>(map_x * 50, map_y * 50, 50, 50));
				map_x++;
				break;
			case '2':
				targets.emplace_back(std::make_unique<Target>(map_x * 50 + 25, map_y * 50 + 25));
				map_x++;
				break;
			case '6':
				my_ship->setClearArea(map_x * 50, map_y * 50, 50, 50);
				map_x++;
				break;
			case '\n':
				map_x = 0;
				map_y++;
				break;
			default:

				break;
			}
		}
		fclose(fp);
	}

	cam.removeAllInteractions();
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);

	cam.enableOrtho();
	cam.setNearClip(-1000000);
	cam.setFarClip(1000000);
	cam.setVFlip(true);

	cam.disableMouseInput();
	cam.disableMouseMiddleButton();

	game_state = opening;

	SourceHanSans = std::make_shared<ofTrueTypeFont>();
	SourceHanSans->load("SourceHanSans001.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	att_clk_se = std::make_unique<ofSoundPlayer>();
	att_clk_se->load("マウス・シングルクリック02.mp3");
	att_clk_se->setVolume(setting_parameter->se_volume);
	att_clk_se->setMultiPlay(true);

	rep_clk_se = std::make_unique<ofSoundPlayer>();
	rep_clk_se->load("マウス・シングルクリック01.mp3");
	rep_clk_se->setVolume(setting_parameter->se_volume);
	rep_clk_se->setMultiPlay(true);

	target_break_se = std::make_unique<ofSoundPlayer>();
	target_break_se->load("ガラスが割れる4.mp3");
	target_break_se->setVolume(setting_parameter->se_volume);
	target_break_se->setMultiPlay(true);

	//BGM
	std::ifstream ifs("data/config.txt");
	std::string str;
	if (!ifs.fail()) {
		for (int i = 0; i < 3; i++) {
			getline(ifs, str);
		}
		game_bgm = std::make_unique<ofSoundPlayer>();
		game_bgm->load(str + ".mp3");
		game_bgm->setLoop(true);
		game_bgm->setVolume(setting_parameter->bgm_volume);
	}
}

GameScene03::~GameScene03()
{
}

void GameScene03::update()
{
	back_ground->updata();
	timer->update();
	ofVec2f cam_pos = my_ship->getPos();
	switch (game_state)
	{
	case GameScene03::opening:
		cam.setPosition(ofVec3f(my_ship->getPos().x, my_ship->getPos().y, 0));
		if (counter == 60) {
			game_state = GameScene03::play;
			counter = 0;
			timer->start();
			game_bgm->play();
		}
		break;
	case GameScene03::play:
	{
		if (attraction_point) {
			if (attraction_or_repulsion) {
				my_ship->resetRepulsion();
				my_ship->addAttraction(attraction_point->getPos());
			}
		}
		else {
			my_ship->resetAttraction();
		}
		if (repulsion_point) {
			if (!attraction_or_repulsion) {
				my_ship->resetAttraction();
				my_ship->addRepulsion(repulsion_point->getPos());
			}
		}
		else {
			my_ship->resetRepulsion();
		}

		my_ship->simulate_move();

		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->update(cam_pos);
			my_ship->getBoxIntersection((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH());

			++it;
		}
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->update();
			if (my_ship->isInCircle((*it)->getX(), (*it)->getY(), (*it)->getR()))
			{
				it = this->targets.erase(it);
				target_break_se->play();
			}
			else {
				++it;
			}

		}

		my_ship->update();
		cam_pos = my_ship->getPos();
		cam.setPosition(ofVec3f(cam_pos.x, cam_pos.y, 0));

		//is gameover
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			if (my_ship->isInBox((*it)->getX(), (*it)->getY(), (*it)->getW(), (*it)->getH(), cam_pos.x, cam_pos.y)) {
				game_state = GameScene03::game_over;
				timer->stop();
				counter = 0;
				break;
			}
			++it;
		}

		if (targets.empty())
		{
			game_state = GameScene03::ending;
			timer->stop();
			counter = 0;
		}
		break;
	}
	case GameScene03::game_over:
		game_bgm->setVolume(ofMap(counter, 0.0, 90, setting_parameter->bgm_volume, 0.0)); //90fでフェードアウト
		if (counter == 90) {
			next_scene = GameScene03::menu_scene;
			can_change_scene = true;
		}
		break;
	case GameScene03::ending:
		my_ship->move();
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

void GameScene03::draw()
{
	back_ground->draw();
	cam.begin();
	ofVec2f cam_pos = my_ship->getPos();
	switch (game_state)
	{
	case GameScene03::opening:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 60, 0, 255));

		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		break;
	case GameScene03::play:
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw(cam_pos);
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
	case GameScene03::game_over:
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw(cam_pos);
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
	case GameScene03::ending:
		ofSetColor(255, 255, 255, ofMap(counter, 0, 180, 255, 0));
		for (auto it = this->obstacles.begin(); it != this->obstacles.end();)
		{
			(*it)->draw(cam_pos);
			it++;
		}
		for (auto it = this->targets.begin(); it != this->targets.end();)
		{
			(*it)->draw(cam_pos);
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
	default:
		break;
	}
	cam.end();

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 5 - 2, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	if (game_bgm->isPlaying())
	{
		std::ostringstream bgm_param;
		bgm_param << setting_parameter->bgm_volume;
		ofDrawBitmapString("bgm: " + bgm_param.str(), 50, setting_parameter->window_height / 2 + 100);
	}

	std::ostringstream se_param;
	se_param << setting_parameter->se_volume;
	ofDrawBitmapString("se:  " + se_param.str(), 50, setting_parameter->window_height / 2 + 150);

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, setting_parameter->window_height - 50);

	timer->drawTime(30, 100, SourceHanSans);

}

void GameScene03::keyPressed(int key) {
	switch (key) {
	case 'q':
		counter = 0;
		game_state = GameScene03::game_over;
		break;
	case 'r':
		next_scene = game_scene03;
		can_change_scene = true;
		break;
	}
}

void GameScene03::keyReleased(int key)
{
}

void GameScene03::mouseMoved(int x, int y)
{
}

void GameScene03::mousePressed(int x, int y, int button)
{
	if (game_state != play) {
		return;
	}

	if (button == 0) {
		attraction_or_repulsion = !attraction_or_repulsion;
		if (attraction_or_repulsion) {
			if (!attraction_point) {
				attraction_point = std::make_unique<AttractionPoint>(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			else if ((x - attraction_point->getPos().x)*(x - attraction_point->getPos().x) + (y - attraction_point->getPos().y)*(y - attraction_point->getPos().y) < 100) {
				//attraction_point.reset();
			}
			else
			{
				attraction_point->setPos(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			att_clk_se->play();
		}
		else {
			if (!repulsion_point) {
				repulsion_point = std::make_unique<RepulsionPoint>(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			else if ((x - repulsion_point->getPos().x)*(x - repulsion_point->getPos().x) + (y - repulsion_point->getPos().y)*(y - repulsion_point->getPos().y) < 100) {
				//repulsion_point.reset();
			}
			else
			{
				repulsion_point->setPos(my_ship->getPos().x - 512 + x, my_ship->getPos().y - 384 + y);
			}
			rep_clk_se->play();
		}
	}
	else if (button == 2) {
		next_scene = game_scene02;
		can_change_scene = true;
	}
}

//*****************game scene 03*****************//