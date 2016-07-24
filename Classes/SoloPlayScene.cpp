#include "SoloPlayScene.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;
using namespace std;

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeButtonItem = MenuItemImage::create("res/close16-128.png", "res/close16-128.png",
		CC_CALLBACK_1(SoloPlayScene::closeCallback,this));
	closeButtonItem->setPosition(Vec2(origin.x+visibleSize.width-closeButtonItem->getContentSize().width,origin.y+closeButtonItem->getContentSize().height/2));

	auto menu = Menu::create(closeButtonItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	auto label = Label::createWithTTF(u8"안녕?", "fonts/HYNAMM.TTF", 24);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);

/*	auto sprite = Sprite3D::create("res/cat_anim.c3b");
	sprite->setPosition(Vec2(visibleSize.width / 2, origin.y + visibleSize.height / 2));
	sprite->setScale(0.5f);
	sprite->setAnchorPoint(Point(0.5f, 0.5f));
	auto animation = Animation3D::create("res/cat_anim.c3b");
	auto animate = Animate3D::create(animation);
	sprite->runAction(RepeatForever::create(animate));
	this->addChild(sprite, 0);*/

	vector<string> word = { "항","아","비","장","보","피","닉","스","치","파",
		"호","스","레","젤","앤","스","로","마","리","타",
		"이","놀","와","툼","르","하","턴","치","라","카",
		"던","타","룰","칸","코","스","시","란","프","샌",
		"오","런","쿤","루","피","티","토","시","톡","소",
		"바","그","다","드","뉴","시","시","론","드","이",
		"상","파","울","루","애","델","드","코","토","니",
		"킨","샤","사","틀","본","스","리","사","시","시",
		"나","트","발","메","밴","고","드","마","르","멕",
		"고","리","이","쿠","카","라","마","이","도","에", };

	auto board = Node::create();
	board->setAnchorPoint(Vec2(0.5f, 0));
	board->setPosition(centerVec2(origin, visibleSize));

	tiles = vector<cocos2d::Sprite*>();
	for (int i = 0; i < _puzzleSize * _puzzleSize; i++) {
		auto tile = Sprite::create("res/tile.png");
		tile->setScale(_quadSize / tile->getContentSize().width);
		tile->setPosition(tileVec2(i));

		auto letter = Label::createWithTTF(word[i].c_str(), "fonts/HYNAMM.TTF", 24);
		letter->setColor(Color3B::GRAY);
		letter->setPosition(Vec2(tile->getContentSize().width / 2.0f, tile->getContentSize().height / 2.0f));

		tile->addChild(letter);
		board->addChild(tile);

		tiles.push_back(tile);
	}

	this->addChild(board);

	tiles[currentPos]->setColor(Color3B::BLUE);

	auto keyboardEventListener = EventListenerKeyboard::create();

	return true;
}

void SoloPlayScene::closeCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SoloPlayScene::changePos(int newPos) {
	tiles[currentPos]->setColor(Color3B::WHITE);
	tiles[newPos]->setColor(Color3B::BLUE);
	currentPos = newPos;
}