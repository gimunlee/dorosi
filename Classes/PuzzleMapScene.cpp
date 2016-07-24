#include "PuzzleMapScene.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8")

#define SIZE_PUZZLE 10
#define SIZE_QUAD 40

USING_NS_CC;

Scene* PuzzleMap::createScene() {
	auto scene = Scene::create();
	auto layer = PuzzleMap::create();
	scene->addChild(layer);
	return scene;
}

bool PuzzleMap::init() {
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::vector<std::string> word = { "항","아","비","장","보","피","닉","스","치","파",
		"호","스","레","젤","앤","스","로","마","리","타",
		"이","놀","와","툼","르","하","턴","치","라","카",
		"던","타","룰","칸","코","스","시","란","프","샌",
		"오","런","쿤","루","피","티","토","시","톡","소",
		"바","그","다","드","뉴","시","시","론","드","이",
		"상","파","울","루","애","델","드","코","토","니",
		"킨","샤","사","틀","본","스","리","사","시","시",
		"나","트","발","메","밴","고","드","마","르","멕",
		"고","리","이","쿠","카","라","마","이","도","에", };

	auto puzzle = Node::create();
	puzzle->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f));
	this->addChild(puzzle);

	std::vector<Sprite*> tiles;
	for (int i = 0; i < SIZE_PUZZLE * SIZE_PUZZLE; i++) {
		auto tile = cocos2d::Sprite::create("res/tile.png");  // tile은 항상 정사각형이 되게 넣을 것!!
		tile->setScale(SIZE_QUAD / tile->getContentSize().width);
		tiles.push_back(tile);
		puzzle->addChild(tiles[i]);
		tiles[i]->setPosition(Vec2((i % 10) * SIZE_QUAD, (int)(i / 10) * SIZE_QUAD) - Vec2((int)(SIZE_PUZZLE / 2) * SIZE_QUAD, (int)(SIZE_PUZZLE / 2) * SIZE_QUAD));

		auto letter = Label::createWithTTF(word[i].c_str(), "fonts/HYGSRB.ttf", 24, Size(300, 100), TextHAlignment::CENTER, TextVAlignment::CENTER);
		letter->setColor(Color3B::BLUE);
		tiles[i]->addChild(letter);
		letter->setPosition(Vec2(tiles[i]->getContentSize().width / 2.0f, tiles[i]->getContentSize().height / 2.0f));
	}

	return true;
}