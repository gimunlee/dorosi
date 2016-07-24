#pragma once
#include "cocos2d.h"
class GimunLogicScene :
	public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void closeCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(GimunLogicScene);
};

