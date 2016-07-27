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

	float longitude, latitude;

	Label* locationLabel;

	void pressButtonLeftCallback(Ref* pSender);
	void pressButtonRightCallback(Ref* pSender);
	void pressButtonUpCallback(Ref* pSender);
	void pressButtonDownCallback(Ref* pSender);

	void locationButtonCallback(Ref* pSender);
	void closeCallback(Ref* pSender);
	void abcCallBack(CCNode* target, void* data);
	const int _puzzleSize = 10;
	const float _quadSize = 40.0f;

	const float _buttonSize = 50.0f;
	const int opacity_button = 200;
	

	Size mVisibleSize;
	Vec2 mOrigin;

	Label* answerLabel;

	vector<string> words;
	vector<string> answers;

	Sprite3D* mCat;
	int currentPos = 0;
	void changePos(int,int);
	Sprite* mFlag;
	int flagPos = -1;
	void onFlag();
	void showFlag();

	vector<Sprite*> tiles;
	Vec2 tileVec2(int pos);

	CCSprite* leftSprigte;
	CCSprite* rightSprite;
	CCSprite* upSprite;
	CCSprite* downSprite;

	typedef struct landscape {
		char name[20];
		float longitude;
		float latitude;
	}landscape;

	landscape locationData[6] = { {"N1", 127.365544, 36.374734}, {"Library", 127.362475, 36.369571}, {"BranchLibrary", 127.363002, 36.371986},
								{"Amphitheater", 127.357820, 36.370837}, {"WestStudentUnion", 127.360663, 36.367140}, {"Papalado", 127.369837, 36.369377} };

	landscape myLocation;

	CREATE_FUNC(SoloPlayScene);
};

