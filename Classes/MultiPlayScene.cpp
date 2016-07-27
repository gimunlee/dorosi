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

			auto letter = Label::createWithTTF("", "fonts/HYNAMM.TTF", 24);
			letter->setColor(Color3B::GRAY);
			letter->setPosition(centerVec2(Point(0, 0), tile->getContentSize()));

			tile->addChild(letter);
			mBoard->addChild(tile, 2);

			tiles.push_back(tile);
		}
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
	_client = SocketIO::connect(_socketServerUrl, *this);
	_client->on("updateboard", CC_CALLBACK_2(MultiPlayScene::onUpdateBoard, this));
//	_client->on("tweet", CC_CALLBACK_2(MultiPlayScene::onTweet, this));
	_client->on("testa", CC_CALLBACK_2(MultiPlayScene::onTesta, this));
	_client->on("connect", CC_CALLBACK_2(MultiPlayScene::onConnect, this));

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
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

	return true;
}

void MultiPlayScene::emitMove(string direction) {
	_client->emit("move", (string("{\"roomtag\":\"gimunRoom\",\"dorosiid\":\"") + _dorosiid + string("\",\"direction\":\"") + direction + string("\"}")).c_str());
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

void MultiPlayScene::onUpdateBoard(SIOClient* client, const string& dataStr) {
	Json::Reader reader;
	Json::Value dataValue;

//	log("==updating board");
	if (reader.parse(dataStr, dataValue)) {
	//	log("====updating tiles");
		Json::Value& tileInfos = dataValue["tiles"];
		log((string("tileinfos.size() = ") +to_string(tileInfos.size())).c_str());
		for (int i = 0; i < tileInfos.size();i++) {
			switch (tileInfos[i].asInt()) {
			case 0: tiles[i]->setColor(Color3B::GRAY); break;
			case 1: tiles[i]->setColor(Color3B::YELLOW); break;
			case 2: tiles[i]->setColor(Color3B::BLUE); break;
			case 3: tiles[i]->setColor(Color3B::RED); break;
			}
		}
		log("==========================================updating dorosis");
		Json::Value& dorosis = dataValue["dorosis"];
		for (int i = 0; i < dorosis.size(); i++) {
			Json::Value& dorosi = dorosis[i];
			log((string("====\t") + dorosi["dorosiid"].asString() + string(" at ") + dorosi["dorosiposition"].asString()).c_str());
			Sprite3D* dorosiSprite = mDorosis.at(dorosi["dorosiid"].asString());
//			Sprite3D* dorosiSprite=(Sprite3D*)getChildByName(dorosi["dorosiid"].asString());
			if (dorosiSprite == NULL)
				dorosiSprite = createCat(dorosi["dorosiid"].asString());
			dorosiSprite->setPosition(tiles[dorosi["dorosiposition"].asInt()]->getPosition());
		}
		client->emit("update success", (string("{\"dorosiid\":\"") + _dorosiid + string("\",\"roomtag\":\"") + string("gimunRoom") + string("\"}")).c_str());
	}
	else {
		log((string("json parsing failed") + reader.getFormattedErrorMessages()).c_str());
		client->emit("update failed", (string("{\"dorosiid\":\"") + _dorosiid + string("\",\"error\" : \"json parsing failed\"}")).c_str());
		return;
	}
}

void MultiPlayScene::onTweet(SIOClient* client, const string& dataStr) {
	Json::Reader reader;
	Json::Value dataValue;
	if (reader.parse(dataStr, dataValue)) {
		log(dataValue["user"].asCString());
		log(dataValue["text"].asCString());
	}
	else {
		log((string("json parsing failed") + reader.getFormattedErrorMessages()).c_str());
		return;
	}
}
void MultiPlayScene::onConnect(SIOClient* client, const string& dataStr) {
	log("== Connected");
	_client->emit("join", (string("{\"roomtag\":\"gimunRoom\",\"dorosiid\":\"") + _dorosiid +  string("\",\"team\":\"1\"}")).c_str());
}
void MultiPlayScene::onMessage(SIOClient* client, const string& dataStr) {
//	log((string("== Message ") + dataStr).c_str());
}
void MultiPlayScene::onClose(SIOClient* client) {
	log("== Close");
}
void MultiPlayScene::onError(SIOClient* client, const string& dataStr) {
	log((string("== Error ") + dataStr).c_str());
}