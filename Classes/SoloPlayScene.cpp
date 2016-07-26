#include "SoloPlayScene.h"
#pragma execution_character_set("utf-8")

#if (CC_TARGE_PLATFORM == CC_PLATFORM_ANDROID)
#inculde "platform/android/jni/JniHelper.h"
#endif
#include "CCCoreLocation.h"
#include <cstdlib>

#include <string>
#include <sstream>

USING_NS_CC;
USING_NS_CC_MATH;

using namespace std;

template <typename T>
std::string to_string(T value) {
	std::ostringstream os;
	os << value;
	return os.str();
}

Vec2 centerVec2(Vec2 origin, Size visibleSize) {
	return Vec2(origin.x + visibleSize.width / 2.0f , origin.y + visibleSize.height);
}


Scene* SoloPlayScene::createScene() {
	auto scene = Scene::create();
	auto layer = SoloPlayScene::create();
	scene->addChild(layer);

	return scene;
}

Vec2 SoloPlayScene::tileVec2(int pos) {
	return Vec2((pos % 10) * _quadSize, (int)(pos / 10) * _quadSize) - Vec2((int)(_puzzleSize/ 2) * _quadSize, (int)(_puzzleSize) * _quadSize);
}

bool SoloPlayScene::init() {
	if (!Layer::init())
		return false;

	mVisibleSize = Director::getInstance()->getVisibleSize();
	mOrigin = Director::getInstance()->getVisibleOrigin();

	auto closeButtonItem = MenuItemImage::create("res/close16-128.png", "res/close16-128.png",
		CC_CALLBACK_1(SoloPlayScene::closeCallback,this));
	closeButtonItem->setPosition(Vec2(mOrigin.x+mVisibleSize.width-closeButtonItem->getContentSize().width,mOrigin.y+closeButtonItem->getContentSize().height/2));

	auto menu = Menu::create(closeButtonItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	{
		locationLabel = Label::createWithTTF("LOCATION", "fonts/HYNAMM.TTF", 24);
		locationLabel->setPosition(Vec2(mOrigin.x + locationLabel->getContentSize().width / 2.0f, mOrigin.y + mVisibleSize.height - locationLabel->getContentSize().height - 200.0f));
		this->addChild(locationLabel, 1);

		auto locationButtonItem = MenuItemImage::create("res/close16-128.png", "res/close16-128.png",
			CC_CALLBACK_1(SoloPlayScene::locationButtonCallback, this));
		locationButtonItem->setPosition(Vec2(mOrigin.x + mVisibleSize.width - locationButtonItem->getContentSize().width, mOrigin.y + locationButtonItem->getContentSize().height / 2 + 100.0f));

		auto locationMenu = Menu::create(locationButtonItem, NULL);
		locationMenu->setPosition(Vec2::ZERO);
		this->addChild(locationMenu, 1);
	}

	
	answerLabel = Label::createWithTTF(u8"안녕?", "fonts/HYNAMM.TTF", 24);
	answerLabel->setPosition(Vec2(mOrigin.x + answerLabel->getContentSize().width/2.0f, mOrigin.y + mVisibleSize.height - answerLabel->getContentSize().height));
	this->addChild(answerLabel, 1);

/*	auto sprite = Sprite3D::create("res/cat_anim.c3b");
	sprite->setPosition(Vec2(visibleSize.width / 2, origin.y + visibleSize.height / 2));
	sprite->setScale(0.5f);
	sprite->setAnchorPoint(Point(0.5f, 0.5f));
	auto animation = Animation3D::create("res/cat_anim.c3b");
	auto animate = Animate3D::create(animation);
	sprite->runAction(RepeatForever::create(animate));
	this->addChild(sprite, 0);*/

	words = { "항","아","비","장","보","피","닉","스","치","파",
		"호","스","레","젤","앤","스","로","마","리","타",
		"이","놀","와","툼","르","하","턴","치","라","카",
		"던","타","룰","칸","코","스","시","란","프","샌",
		"오","런","쿤","루","피","티","토","시","톡","소",
		"바","그","다","드","뉴","시","시","론","드","이",
		"상","파","울","루","애","델","드","코","토","니",
		"킨","샤","사","틀","본","스","리","사","시","시",
		"나","트","발","메","밴","고","드","마","르","멕",
		"고","리","이","쿠","카","라","마","이","도","에", };
	answers = { "아비장",
		"샌프란시스코",
		"로스앤젤레스",
		"바그다드",
		"상파울루",
		"로마",
		"시드니",
		"멕시코시티",
		"시애틀",
		"리스본",
		"보스턴",
		"뉴델리",
		"오타와",
		"호놀룰루" };

	auto board = Node::create();
	board->setAnchorPoint(Vec2(0.5f, 0));
	board->setPosition(centerVec2(mOrigin, mVisibleSize));
	{
		tiles = vector<cocos2d::Sprite*>();
		for (int i = 0; i < _puzzleSize * _puzzleSize; i++) {
			auto tile = Sprite::create("res/tile.png");
			tile->setScale(_quadSize / tile->getContentSize().width);
			tile->setPosition(tileVec2(i));

			auto letter = Label::createWithTTF(words[i].c_str(), "fonts/HYNAMM.TTF", 24);
			letter->setColor(Color3B::GRAY);
			letter->setPosition(Vec2(tile->getContentSize().width / 2.0f, tile->getContentSize().height / 2.0f));

			tile->addChild(letter);
			board->addChild(tile,2);

			tiles.push_back(tile);
		}
	}
	{
		auto flag = Sprite::create("res/Flag_small.png");
		flag->setVisible(false);
		flag->setScale(_quadSize / flag->getContentSize().height);
		mFlag = flag;
		board->addChild(mFlag, 3);
	}

	mCat = Sprite3D::create("res/cat_anim.c3b");
	//	sprite->setPosition(Vec2(mVisibleSize.width / 2, origin.y + visibleSize.height / 2));
		mCat->setScale(_puzzleSize/mCat->getContentSize().width*10.0f);
	//mCat->setScale(1.0f);
	mCat->setAnchorPoint(Point(0.5f, 0.5f));
	mCat->setTexture("res/goose_pot.png");
	auto animation = Animation3D::create("res/cat_anim.c3b");
	auto animate = Animate3D::create(animation);
	mCat->runAction(RepeatForever::create(animate));
	Director::getInstance()->getScheduler()->setTimeScale(0.5f);
	board->addChild(mCat);
	mCat->setGlobalZOrder(10000);
	mCat->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));

	this->addChild(board,1);

	mCat->setPosition(tiles[currentPos]->getPosition());
//	tiles[currentPos]->setColor(Color3B::BLUE);

	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
//		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			((SoloPlayScene*)(event->getCurrentTarget()))->changePos(-1, 0);
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			((SoloPlayScene*)(event->getCurrentTarget()))->changePos(1, 0);
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			((SoloPlayScene*)(event->getCurrentTarget()))->changePos(0, 1);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			((SoloPlayScene*)(event->getCurrentTarget()))->changePos(0, -1);
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_ENTER:
//			((SoloPlayScene*)(event->getCurrentTarget()))->showFlag();
			((SoloPlayScene*)(event->getCurrentTarget()))->onFlag();
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

	CCCoreLocation::getInstance()->getLocation(this, callfuncND_selector(SoloPlayScene::abcCallBack));

	return true;
}

void SoloPlayScene::abcCallBack(CCNode* target, void* data) {
	longitude = *((float *)data);
	latitude = *((float *)data + 1);

	log("longitude");

	log("longitude: %f, latitude: %f", longitude, latitude);
}

void SoloPlayScene::locationButtonCallback(Ref* pSender) {
	CCCoreLocation::getInstance()->getLocation(this, callfuncND_selector(SoloPlayScene::abcCallBack));
	locationLabel->setString(to_string(longitude) + to_string(latitude));
}

void SoloPlayScene::closeCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SoloPlayScene::changePos(int deltaX, int deltaY) {
	int currentX = currentPos%_puzzleSize;
	int currentY = currentPos / _puzzleSize;
	if (currentX + deltaX >= _puzzleSize || currentX + deltaX < 0 || currentY + deltaY >= _puzzleSize || currentY + deltaY < 0)
		return;
	int newPos = currentPos + deltaX + _puzzleSize*deltaY;
	if (newPos >= 0 && newPos < _puzzleSize*_puzzleSize) {
//		tiles[currentPos]->setColor(Color3B::WHITE);
//		tiles[newPos]->setColor(Color3B::BLUE);
		mCat->setPosition(tiles[newPos]->getPosition());
		currentPos = newPos;
	}
}

void SoloPlayScene::showFlag() {
	flagPos = currentPos;
	mFlag->setVisible(true);
	mFlag->setPosition(tiles[flagPos]->getPosition());
	//mFlag->setPosition(Vec2(50, 50));
}

int norm(int t) {
	if (t == 0)
		return 0;
	return t / abs(t);
}
void SoloPlayScene::onFlag() {
	int flagX = flagPos%_puzzleSize;
	int flagY = flagPos / _puzzleSize;
	int currentX = currentPos%_puzzleSize;
	int currentY = currentPos / _puzzleSize;

	int deltaX, deltaY;
	deltaX = norm(currentX-flagX);
	deltaY = norm(currentY- flagY);
	//log((string("deltaX = ") + std::to_string(deltaX)).c_str());
	//log((string("deltaY = ") + std::to_string(deltaY)).c_str());
	//log("==========");

	if ((deltaX*deltaY == 0 || abs(currentX - flagX) == abs(currentY - flagY)) && flagPos!=-1) {

		string word;

		int tempPos = flagPos;
		while (tempPos != currentPos) {
			word.append(words[tempPos]);
			tempPos += deltaX + deltaY*_puzzleSize;
		}
		word.append(words[currentPos]);
		int i;
		for (i = 0; i < answers.size(); i++) {
			if (word.compare(answers[i]) == 0)
				break;
		}
		if (i < answers.size()) {
			tempPos = flagPos;
			while (tempPos != currentPos) {
				tiles[tempPos]->setColor(Color3B::BLUE);
//				word.append(words[tempPos]);
				tempPos += deltaX + deltaY*_puzzleSize;
			}
			tiles[currentPos]->setColor(Color3B::BLUE);
		}

		answerLabel->setString(word);
		answerLabel->setPosition(Vec2(mOrigin.x + answerLabel->getContentSize().width / 2.0f, mOrigin.y + mVisibleSize.height - answerLabel->getContentSize().height));
	}
	showFlag();
}