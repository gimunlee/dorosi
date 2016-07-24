#pragma once
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class SoloPlayScene :
	public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	void closeCallback(Ref* pSender);

	const int _puzzleSize = 10;
	const float _quadSize = 40.0f;

	int currentPos = 0;
	void changePos(int newPos);

	vector<Sprite*> tiles;

	Vec2 tileVec2(int pos);

	CREATE_FUNC(SoloPlayScene);
};

