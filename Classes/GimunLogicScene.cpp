#include "GimunLogicScene.h"

USING_NS_CC;
using namespace std;


Scene* GimunLogicScene::createScene() {
	auto scene = Scene::create();
	auto layer = GimunLogicScene::create();
	scene->addChild(layer);

	return scene;
}

bool GimunLogicScene::init() {
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeButtonItem = MenuItemImage::create("res/close16-128.png", "res/close16-128.png",
		CC_CALLBACK_1(GimunLogicScene::closeCallback,this));
	closeButtonItem->setPosition(Vec2(origin.x+visibleSize.width-closeButtonItem->getContentSize().width,origin.y+closeButtonItem->getContentSize().height/2));

	auto menu = Menu::create(closeButtonItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF(u8"¾È³ç?", "fonts/HYNAMM.TTF", 24);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);

	log("gimun");

	auto sprite = Sprite3D::create("res/cat_anim.c3b");
	sprite->setPosition(Vec2(visibleSize.width / 2, origin.y + visibleSize.height / 2));
	sprite->setScale(0.5f);
	sprite->setAnchorPoint(Point(0.5f, 0.5f));
	auto animation = Animation3D::create("res/cat_anim.c3b");
	auto animate = Animate3D::create(animation);
	sprite->runAction(RepeatForever::create(animate));
	this->addChild(sprite, 0);

	return true;
}

void GimunLogicScene::closeCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}