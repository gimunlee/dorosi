#include "commonFunc.h"
#include "cocos2d.h"
USING_NS_CC;

Vec2 centerVec2(Vec2 origin, Size visibleSize) {
	return Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height/2.0f);
}