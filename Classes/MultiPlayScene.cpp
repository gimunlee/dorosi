#include "MultiPlayScene.h"
#include "commonFunc.h"
#include "json/json.h"
using namespace cocos2d::network;

Scene* MultiPlayScene::createScene() {
	auto scene = Scene::create();
	auto layer = MultiPlayScene::create();
	scene->addChild(layer);

	return scene;
}

Vec2 MultiPlayScene::tileVec2(int pos) {
	return Vec2(((pos % _puzzleSize)-_puzzleSize/2) * _quadSize, ((int)(pos / _puzzleSize)-_puzzleSize/2)*_quadSize);// - Vec2((int)(_puzzleSize / 2)*_quadSize, (int)(_puzzleSize)*_quadSize);
}

Sprite3D* MultiPlayScene::createCat(string dorosiid) {
	auto cat = Sprite3D::create("res/cat_anim.c3b");
	mDorosis.insert(dorosiid, cat);
	cat->setScale(_dorosiScale);
	cat->setAnchorPoint(Point(0, 0));
	cat->setTexture("res/cat_pot.png");
	auto animation = Animation3D::create("res/cat_anim.c3b");
	auto animate = Animate3D::create(animation);
	cat->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0f), animate, NULL)));
	cat->setGlobalZOrder(10000);
	cat->setRotation3D(_dorosiRotation);
	cat->setPosition(Point(-1, -1));

	mBoard->addChild(cat);

	return cat;

	//	mCat->setPosition(tiles[0]->getPosition());
}

bool MultiPlayScene::init() {
	if (!Layer::init())
		return false;

	mVisibleSize = Director::getInstance()->getVisibleSize();
	mOrigin = Director::getInstance()->getVisibleOrigin();

	auto closeButtonItem = MenuItemImage::create("res/close16-128.png", "res/close16-128.png", CC_CALLBACK_1(MultiPlayScene::closeCallback, this));
	closeButtonItem->setPosition(Vec2(mOrigin.x + mVisibleSize.width - closeButtonItem->getContentSize().width, mOrigin.y + closeButtonItem->getContentSize().height / 2));

	auto menu = Menu::create(closeButtonItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	mBoard = Layer::create();
	mBoard->setAnchorPoint(Vec2(0.5, 0.5));
	mBoard->setPosition(centerVec2(Point(0,0),mVisibleSize));
	log("%f %f",mVisibleSize.width,mVisibleSize.height);
	log("%f %f", mBoard->getPosition().x, mBoard->getPosition().y);
	{
		tiles = vector<cocos2d::Sprite*>();
		for (int i = 0; i < _puzzleSize * _puzzleSize; i++) {
			auto tile = Sprite::create("res/tile.png");
			tile->setScale(_quadSize / tile->getContentSize().width);
			tile->setPosition(tileVec2(i));

//			auto letter = Label::createWithTTF("", "fonts/HYNAMM.TTF", 24);
	//		letter->setColor(Color3B::GRAY);
		//	letter->setPosition(centerVec2(Point(0, 0), tile->getContentSize()));

			//tile->addChild(letter);
			mBoard->addChild(tile, 2);

			tiles.push_back(tile);
		}
	}
	{
		auto buttonLeftItem = MenuItemImage::create("res/control_button/button_left.png", "res/control_button/button_left.png", CC_CALLBACK_1(MultiPlayScene::onLeftButtonCallback, this));
		buttonLeftItem->setScale(_buttonSize / buttonLeftItem->getContentSize().width);
		buttonLeftItem->setPosition(Vec2(mOrigin.x + _buttonSize *0.5f, mOrigin.y + _buttonSize * 1.5f));
		auto buttonLeftMenu = Menu::create(buttonLeftItem, NULL);
		buttonLeftMenu->setPosition(Point(0, 0));
		this->addChild(buttonLeftMenu);
	}
	{
		auto buttonRightItem = MenuItemImage::create("res/control_button/button_right.png", "res/control_button/button_right.png", CC_CALLBACK_1(MultiPlayScene::onRightButtonCallback, this));
		buttonRightItem->setScale(_buttonSize / buttonRightItem->getContentSize().width);
		buttonRightItem->setPosition(Vec2(mOrigin.x + _buttonSize *2.5f, mOrigin.y + _buttonSize * 1.5f));
		auto buttonRightMenu = Menu::create(buttonRightItem, NULL);
		buttonRightMenu->setPosition(Point(0, 0));
		this->addChild(buttonRightMenu);
	}
	{
		auto buttonUpItem = MenuItemImage::create("res/control_button/button_up.png", "res/control_button/button_up.png", CC_CALLBACK_1(MultiPlayScene::onUpButtonCallback, this));
		buttonUpItem->setScale(_buttonSize / buttonUpItem->getContentSize().width);
		buttonUpItem->setPosition(Vec2(mOrigin.x + _buttonSize *1.5f, mOrigin.y + _buttonSize * 2.5f));
		auto buttonUpMenu = Menu::create(buttonUpItem, NULL);
		buttonUpMenu->setPosition(Point(0, 0));
		this->addChild(buttonUpMenu);
	}
	{
		auto buttonDownItem = MenuItemImage::create("res/control_button/button_down.png", "res/control_button/button_down.png", CC_CALLBACK_1(MultiPlayScene::onDownButtonCallback, this));
		buttonDownItem->setScale(_buttonSize / buttonDownItem->getContentSize().width);
		buttonDownItem->setPosition(Vec2(mOrigin.x + _buttonSize *1.5f, mOrigin.y + _buttonSize * 0.5f));
		auto buttonDownMenu = Menu::create(buttonDownItem, NULL);
		buttonDownMenu->setPosition(Point(0, 0));
		this->addChild(buttonDownMenu);
	}
	{
		auto buttonFlagItem = MenuItemImage::create("res/control_button/button_down.png", "res/control_button/button_down.png", CC_CALLBACK_1(MultiPlayScene::onFlagButtonCallback, this));
		buttonFlagItem->setScale(_buttonSize*2 / buttonFlagItem->getContentSize().width);
		buttonFlagItem->setPosition(Vec2(mOrigin.x + mVisibleSize.width - _buttonSize*1.5, mOrigin.y + 300));
		log("tea : %f %f", buttonFlagItem->getPosition().x, buttonFlagItem->getPosition().y);
		auto buttonFlagMenu = Menu::create(buttonFlagItem, NULL);
		buttonFlagMenu->setPosition(Point(0, 0));
		this->addChild(buttonFlagMenu);
	}
	{
		auto letters = Sprite::create("res/capitals.png");
		letters->setPosition(Point(0, 0));
		letters->setScale(mVisibleSize.height / letters->getContentSize().height);
//		letters->setScale(_lettersScale);
		mBoard->addChild(letters, 3);
	}
	{
		auto flag = Sprite::create("res/Flag_small.png");
		flag->setVisible(false);
		flag->setScale(_quadSize / flag->getContentSize().height);
		mFlag = flag;
		mBoard->addChild(mFlag, 3);
	}

	this->addChild(mBoard, 1);

	//Network
	mClient = SocketIO::connect(_socketServerUrl, *this);
	mClient->on("load", CC_CALLBACK_2(MultiPlayScene::onLoad, this));
//	mClient->on("tweet", CC_CALLBACK_2(MultiPlayScene::onTweet, this));
	mClient->on("testa", CC_CALLBACK_2(MultiPlayScene::onTesta, this));
	mClient->on("connect", CC_CALLBACK_2(MultiPlayScene::onConnect, this));

	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			emitMove(string("left"));
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			emitMove(string("right"));
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			emitMove(string("up"));
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			emitMove(string("down"));
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_ENTER:
			emitFlag();
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

	return true;
}

void MultiPlayScene::emitMove(string direction) {
	mClient->emit("move", (string("{\"roomtag\":\"gimunRoom\",\"dorosiid\":\"") + _dorosiid + string("\",\"direction\":\"") + direction + string("\"}")).c_str());
}
void MultiPlayScene::emitFlag() {
	mClient->emit("flag", (string("{\"roomtag\":\"gimunRoom\",\"dorosiid\":\"") + _dorosiid + string("\"}")).c_str());
}

void MultiPlayScene::onTesta(SIOClient* client, const string& dataStr) {
	log("onTesta");
}

void MultiPlayScene::closeCallback(Ref* pSender) {
	Director::getInstance()->end();

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MultiPlayScene::onLoad(SIOClient* client, const string& dataStr) {
	Json::Reader reader;
	Json::Value dataValue;

	if (reader.parse(dataStr, dataValue)) {
		Json::Value& answersValue = dataValue["answers"];
		log("answers.size() = %d", answersValue.size());
		for (int i = 0; i < answersValue.size(); i++)
			mAnswers.push_back(answersValue[i].asString());

		Json::Value& wordsValue = dataValue["words"];
		log("words.size() = %d", wordsValue.size());
		for (int i = 0; i < wordsValue.size(); i++) {
			mWords.push_back(wordsValue[i].asString());
			log("%s",mWords[i].c_str());
			/*auto letter = Label::createWithTTF(mWords[i], "fonts/HYNAMM.TTF", 30);
			letter->setColor(Color3B::BLACK);
			letter->setPosition(centerVec2(Point(0, 0), tiles[i]->getContentSize()));
			tiles[i]->addChild(letter);*/
		}

		mClient->on("updateboard", CC_CALLBACK_2(MultiPlayScene::onUpdateBoard, this));
		mClient->emit("load completed", (string("{\"dorosiid\":\"") + _dorosiid + string("\",\"result\":\"true\"}")).c_str());
	}
	else {
		log("json parsing failed %s",reader.getFormattedErrorMessages().c_str());
		return;
	}
}

void MultiPlayScene::onUpdateBoard(SIOClient* client, const string& dataStr) {
	Json::Reader reader;
	Json::Value dataValue;

//	log("==updating board");
	if (reader.parse(dataStr, dataValue)) {
	//	log("====updating tiles");
		Json::Value& tileInfos = dataValue["tiles"];
//		log("tileinfos.size() = %s",to_string(tileInfos.size()).c_str());
		for (int i = 0; i < tileInfos.size();i++) {
			int oldTileInfo=-1;
			if (tiles[i]->getColor() == Color3B::GRAY) oldTileInfo = 0;
			if (tiles[i]->getColor() == Color3B::YELLOW) oldTileInfo = 1;
			if (tiles[i]->getColor() == Color3B::BLUE) oldTileInfo = 2;
			if (tiles[i]->getColor() == Color3B::RED) oldTileInfo = 3;

			if (tileInfos[i].asInt() != oldTileInfo) {
				float oldScale = tiles[i]->getScale();
				tiles[i]->setScale(oldScale/100);
				auto animate = EaseElasticOut::create(ScaleTo::create(0.8f, oldScale),0.8f);
				tiles[i]->stopAllActions();
				tiles[i]->runAction(animate);

				switch (tileInfos[i].asInt()) {
				case 0: tiles[i]->setColor(Color3B::GRAY); break;
				case 1: tiles[i]->setColor(Color3B::YELLOW); break;
				case 2: tiles[i]->setColor(Color3B::BLUE); break;
				case 3: tiles[i]->setColor(Color3B::RED); break;
				}
			}
		}
		log("==========================================updating dorosis");
		Json::Value& dorosis = dataValue["dorosis"];
		for (int i = 0; i < dorosis.size(); i++) {
			Json::Value& dorosi = dorosis[i];
//			log("====\t %s at %s",dorosi["dorosiid"].asCString(),dorosi["dorosiposition"].asCString());
			Sprite3D* dorosiSprite = mDorosis.at(dorosi["dorosiid"].asString());
//			Sprite3D* dorosiSprite=(Sprite3D*)getChildByName(dorosi["dorosiid"].asString());
			if (dorosiSprite == NULL) {
				dorosiSprite = createCat(dorosi["dorosiid"].asString());
				switch (dorosi["team"].asInt()) {
				case 2: dorosiSprite->setTexture("res/goose_pot.png"); break;
				case 3: dorosiSprite->setTexture("res/magpie_pot.png"); break;
				}
			}
			Vec2 oldPos = dorosiSprite->getPosition();
			Vec2 newPos = tiles[dorosi["dorosiposition"].asInt()]->getPosition();
			if (oldPos != newPos) {
				dorosiSprite->stopActionByTag(7);
				auto animate = EaseOut::create(MoveTo::create(0.05f, newPos),0.8f);
				animate->setTag(7);
				dorosiSprite->runAction(animate);
			}
			if (dorosi["dorosiid"].asString() == _dorosiid) {
				// If it's me,
				if (dorosi["flagposition"].asInt() != -1) {
					mFlag->setVisible(true);
					mFlag->setPosition(tiles[dorosi["flagposition"].asInt()]->getPosition());
				}
			}
//			dorosiSprite->setPosition(tiles[dorosi["dorosiposition"].asInt()]->getPosition());
		}
		client->emit("update success", (string("{\"dorosiid\":\"") + _dorosiid + string("\",\"roomtag\":\"") + string("gimunRoom") + string("\"}")).c_str());
	}
	else {
		log("json parsing failed %s",reader.getFormattedErrorMessages().c_str());
		client->emit("update failed", (string("{\"dorosiid\":\"") + _dorosiid + string("\",\"error\" : \"json parsing failed\"}")).c_str());
		return;
	}
}

void MultiPlayScene::onTweet(SIOClient* client, const string& dataStr) {
	Json::Reader reader;
	Json::Value dataValue;
	if (reader.parse(dataStr, dataValue)) {
		log("%s",dataValue["user"].asCString());
		log("%s",dataValue["text"].asCString());
	}
	else {
		log("json parsing failed %s",reader.getFormattedErrorMessages().c_str());
		return;
	}
}
void MultiPlayScene::onConnect(SIOClient* client, const string& dataStr) {
	log("== Connected");
	mClient->emit("join", (string("{\"roomtag\":\"gimunRoom\",\"dorosiid\":\"") + _dorosiid +  string("\",\"team\":\"") + string("1") + string("\"}")).c_str());
}
void MultiPlayScene::onMessage(SIOClient* client, const string& dataStr) {
//	log((string("== Message ") + dataStr).c_str());
}
void MultiPlayScene::onClose(SIOClient* client) {
	log("== Close");
	closeCallback(NULL);
}
void MultiPlayScene::onError(SIOClient* client, const string& dataStr) {
	log("== Error %s",dataStr.c_str());
}

void MultiPlayScene::onLeftButtonCallback(Ref* pSender) {
	log("left");
	emitMove(string("left"));
}
void MultiPlayScene::onRightButtonCallback(Ref* pSender) {
	log("right");
	emitMove(string("right"));
}
void MultiPlayScene::onUpButtonCallback(Ref* pSender) {
	log("up");
	emitMove(string("up"));
}
void MultiPlayScene::onDownButtonCallback(Ref* pSender) {
	log("down");
	emitMove(string("down"));
}
void MultiPlayScene::onFlagButtonCallback(Ref* pSender) {
	log("flag");
	emitFlag();
}