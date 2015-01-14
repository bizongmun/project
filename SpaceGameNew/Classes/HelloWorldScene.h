#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"
#include "chipmunk.h"

USING_NS_CC;

using namespace cocos2d;

typedef enum {
    kEndReasonWin,
    kEndReasonLose
} EndReason;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // add space ship
    SpriteBatchNode *_batchNode;
    Sprite *_ship;
    
    // add asteroids
    Vector<cocos2d::Sprite*>* _asteroids;
    int _newtAsteroid;
    float _nextAsteroidSpawn;
    float _nextAsteroidTimer;
    
    // add lasers of ship
    Vector<cocos2d::Sprite*>* _shipLasers;
    int _nextShipLaser;
    
    // add lives
    int _lives;
    
    // add physics
    PhysicsWorld* _world;
    void setPhyWorld(PhysicsWorld* world) {_world = world;};
    
    // add background
    //cocos2d::ParallaxNode *_backgroundNode;
    ParallaxNodeExtras* _backgroundNode;
    Sprite *_spacedust1;
    Sprite *_spacedust2;
    Sprite *_planetsunrise;
    Sprite *_galaxy;
    Sprite *_spacialnomaly;
    Sprite *_spacialnomaly2;
    
    // game over
    double _gameOverTime;
    bool _gameOver;
    
    // score
    int score;
    LabelTTF* lblScore;
    
    // move spaces dust and backgrounds
    void update(float dt);
    
    bool onContactBegin(PhysicsContact& contact);
    bool onTouchBegan(cocos2d::Touch* touches, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touches, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touches, cocos2d::Event* event);
    
    // get random value in a range
    float randomValueBetween(float low, float high);
    
    void setInvisible(cocos2d::Node* node);
    
    // game end
    void endScene(EndReason endReason);
    
    //restart game
    void resetGame(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
