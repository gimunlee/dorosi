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

	std::vector<std::string> word = { "��","��","��","��","��","��","��","��","ġ","��",
		"ȣ","��","��","��","��","��","��","��","��","Ÿ",
		"��","��","��","��","��","��","��","ġ","��","ī",
		"��","Ÿ","��","ĭ","��","��","��","��","��","��",
		"��","��","��","��","��","Ƽ","��","��","��","��",
		"��","��","��","��","��","��","��","��","��","��",
		"��","��","��","��","��","��","��","��","��","��",
		"Ų","��","��","Ʋ","��","��","��","��","��","��",
		"��","Ʈ","��","��","��","��","��","��","��","��",
		"��","��","��","��","ī","��","��","��","��","��", };

	auto puzzle = Node::create();
	puzzle->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f));
	this->addChild(puzzle);

	std::vector<Sprite*> tiles;
	for (int i = 0; i < SIZE_PUZZLE * SIZE_PUZZLE; i++) {
		auto tile = cocos2d::Sprite::create("res/tile.png");  // tile�� �׻� ���簢���� �ǰ� ���� ��!!
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