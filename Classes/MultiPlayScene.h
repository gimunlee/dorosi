#pragma once
#include "cocos2d.h"
#include <network/SocketIO.h>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::network;
using namespace cocos2d::ui;

class MultiPlayScene :
	public Layer,
	public SocketIO::SIODelegate
{
public:
	static Scene* createScene();

	virtual bool init();

	//menu
	void closeCallback(Ref* pSender);

	//constants
	int _puzzleSize = 15;
	const float _quadSize = 30.0f;
	const float _dorosiScale = 0.1;
	const Vec3 _dorosiRotation = Vec3(90, 0, 0);

	//Screen info
	Size mVisibleSize;
	Vec2 mOrigin;

	//sprites
	Map<string, Sprite3D*> mDorosis;
//	Sprite3D* mCat;
	Sprite* mFlag;
	void showFlag();
	CCNode* mBoard;
	vector<Sprite*> tiles;
	Vec2 tileVec2(int pos);

	//Data
	vector<string> mWords;
	vector<string> mAnswers;

	Sprite3D* createCat(string dorosiid);

	//Network
	SIOClient* mClient;
	const string _dorosiid = "gimunDo";
	const string _socketServerUrl = "http://143.248.48.232:10240";

	void onTweet(SIOClient* client, const string& dataStr);
	void onUpdateBoard(SIOClient* client, const string& dataStr);
	void onLoad(SIOClient* client, const string& dataStr);

	void onTesta(SIOClient* client, const string& dataStr);

	void onConnect(SIOClient* client, const std::string& dataStr);
	virtual void onMessage(SIOClient* client, const std::string& dataStr);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& dataStr);

	void updateBoard();

	void emitMove(string direction);
	void emitFlag();

	CREATE_FUNC(MultiPlayScene);
};