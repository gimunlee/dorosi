#ifndef __PUZZLEMAPSCENE_H__
#define __PUZZLEMAPSCENE_H__

#include "cocos2d.h"

class PuzzleMap : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	CREATE_FUNC(PuzzleMap);
};

#endif  // __PUZZLEMAPSCENE_H__