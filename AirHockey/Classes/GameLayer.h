#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include <CCGeometry.h>
#include "GameSprite.h"
#include <CCVector.h>

#define GOAL_WIDTH 400

using namespace cocos2d;

class GameLayer : public cocos2d::Layer
{
    GameSprite* _player1;
    GameSprite* _player2;
    GameSprite* _ball;
    __Array* _players;
    Label* _play1ScoreLabel;
    Label* _play2ScoreLabel;
    Size _screenSize;
    int _play1Score;
    int _play2Score;
    
    void playerScore(int player);    
public:
    ~GameLayer();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
    
    virtual void onTouchesBegan(__Set* pTouches, Event* event);
    virtual void onTouchesMoved(__Set* pTouches, Event* event);
    virtual void onTouchesEnd(__Set* pTouches, Event* event);
    void update(float dt);
};

#endif // __GAMELAYER_H__
