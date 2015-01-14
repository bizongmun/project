#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <time.h>

using namespace std;
using namespace CocosDenshion;
using namespace cocos2d;

USING_NS_CC;

enum {
    KSHIP,
    KLASER,
    KASTEROID
};

#define kNumAsteroids   15
#define kNumLasers      5

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    Vect gravity(0.0f, 0.0f); // vector gia toc = 0
    scene->getPhysicsWorld()->setGravity(gravity);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    _batchNode = SpriteBatchNode::create("Spritesheets/Sprites.pvr.ccz");
    this->addChild(_batchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Spritesheets/Sprites.plist");
    _ship = Sprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    _ship->setPosition(Point(visibleSize.width * 0.1, visibleSize.height * 0.5));
    _ship->setTag(KSHIP);
    _batchNode->addChild(_ship, 1);
    // create body for ship
    auto _shipBody = PhysicsBody::createCircle(_ship->getContentSize().width / 2 - 23);
    _shipBody->setContactTestBitmask(0xf);
    // va cham tinh
    _shipBody->setDynamic(false);
    _ship->setPhysicsBody(_shipBody);
    
    // create parallax node
    //_backgroundNode = ParallaxNode::create();
    _backgroundNode = ParallaxNodeExtras::create();
    this->addChild(_backgroundNode, -1);
    
    _spacedust1 = Sprite::create("Backgrounds/bg_front_spacedust.png");
    _spacedust2 = Sprite::create("Backgrounds/bg_front_spacedust.png");
    _planetsunrise = Sprite::create("Backgrounds/bg_planetsunrise.png");
    _galaxy = Sprite::create("Backgrounds/bg_galaxy.png");
    _spacialnomaly = Sprite::create("Backgrounds/bg_spacialanomaly.png");
    _spacialnomaly2 = Sprite::create("Backgrounds/bg_spacialanomaly2.png");
    
    Point ductSpeed = Point(0.1, 0.1);
    Point bgSpeed = Point(0.05, 0.05);
    
    _backgroundNode->addChild(_spacedust1, 0, ductSpeed, Point(0, visibleSize.height / 2));
    _backgroundNode->addChild(_spacedust2, 0, ductSpeed, Point(_spacedust1->getContentSize().width, visibleSize.height / 2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, Point(0, visibleSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1, bgSpeed, Point(600, visibleSize.height * 0));
    _backgroundNode->addChild(_spacialnomaly, -1, bgSpeed, Point(900, visibleSize.height * 0.3));
    _backgroundNode->addChild(_spacialnomaly2, -1, bgSpeed, Point(1500, visibleSize.height * 0.9));
    
    // add star
    const int starNum = 3;
    std::string starsArray[starNum] = {"Stars1.plist", "Stars2.plist", "Stars3.plist"};
    for (int i = 0; i < starNum; i++) {
        std::string star = starsArray[i];
        auto starEffect = ParticleSystemQuad::create(star);
        this->addChild(starEffect, 1);
    }
    
    // add asteroids
    _asteroids = new Vector<Sprite*>(kNumAsteroids);
    for (int i = 0; i < kNumAsteroids; i ++) {
        Sprite* asteroid = Sprite::createWithSpriteFrameName("asteroid.png");
        asteroid->setVisible(false);
        asteroid->setTag(KASTEROID);
        _batchNode->addChild(asteroid);
        _asteroids->pushBack(asteroid);
    }
    
    // add lasers
    _shipLasers = new Vector<Sprite*>(kNumLasers);
    for (int i = 0; i < kNumLasers; i ++) {
        Sprite* laser = Sprite::createWithSpriteFrameName("laserbeam_blue.png");
        laser->setVisible(false);
        laser->setTag(KLASER);
        _batchNode->addChild(laser);
        _shipLasers->pushBack(laser);
    }
    // catch event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _lives = 3;
    _nextShipLaser = 0;
    _newtAsteroid = 0;
    _nextAsteroidTimer = 0;
    _nextAsteroidSpawn = 2.0f; // 1.6s / asteroid
    
    // game over
    _gameOver = false;
    // win after 30 seconds ago
    _gameOverTime = time(NULL) + 30.0;
    
    // score
    lblScore = LabelTTF::create("Score : 0", "Arial", 20);
    lblScore->setAnchorPoint(Point(1, 0.5));
    lblScore->setPosition(Point(visibleSize.width * 0.8f, visibleSize.height * 0.94f));
    this->addChild(lblScore);
    
    // load audio
    SimpleAudioEngine::getInstance()->playBackgroundMusic("SpaceGame.wav",true);
    SimpleAudioEngine::getInstance()->preloadEffect("explosion_large.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("laser_ship.wav");
    
    // listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    //this->schedule(schedule_selector(HelloWorld::update), 0.01);
    this->scheduleUpdate();
    
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touches, cocos2d::Event *event)
{
    SimpleAudioEngine::getInstance()->playEffect("laser_ship.wav");
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite* shipLaser = _shipLasers->at(_nextShipLaser++);
    //if (_nextShipLaser >= _shipLasers->size()) {
    if (_nextShipLaser >= 1) {
        _nextShipLaser = 0;
        // set laser in front of ship
        shipLaser->setPosition(Point(_ship->getPositionX() + shipLaser->getContentSize().width / 2, _ship->getPosition().y));
        shipLaser->setVisible(true);
        // set body
        auto laserbody = PhysicsBody::createBox(shipLaser->getContentSize() / 2);
        laserbody->setContactTestBitmask(0xf);
        laserbody->setDynamic(true);
        shipLaser->setPhysicsBody(laserbody);
        shipLaser->stopAllActions();
        shipLaser->runAction(Sequence::create(MoveBy::create(0.5, Point(winSize.width, 0)),CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible,this)),NULL));
    }
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touches, cocos2d::Event *event)
{
    Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touches));
    _ship->setPosition(touchLocation);
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touches, cocos2d::Event *event)
{
    
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto laser = (Sprite*)contact.getShapeA()->getBody()->getNode();
    int Tag1 = -1;
    if (laser) {
        Tag1 = laser->getTag();
        auto asteroid = (Sprite*)contact.getShapeB()->getBody()->getNode();
        int Tag2 = -1;
        if (asteroid) {
            Tag2 = asteroid->getTag();
        }
        // va cham giua laser va thien thach
        if((Tag1==KLASER&Tag2==KASTEROID)||(Tag2==KLASER&Tag1==KASTEROID)) {
            SimpleAudioEngine::getInstance()->playEffect("explosion_large.wav");
            _world->removeBody(laser->getPhysicsBody());
            laser->setVisible(false);
            _world->removeBody(asteroid->getPhysicsBody());
            asteroid->setVisible(false);
            score += 10;
            char val[100] = {0};
            sprintf(val, "Score : %i", score);
            lblScore->setString(val);
        }
        // va cham giua thuyen va thien thach
        if((Tag1==KSHIP&Tag2==KASTEROID)||(Tag2==KSHIP&Tag1==KASTEROID)){
            _lives--;
        }
    }
    return true;
}

float HelloWorld::randomValueBetween(float low, float high)
{
    return (((float)rand() / RAND_MAX)*(high - low)) + low;
}

void HelloWorld::setInvisible(cocos2d::Node *node)
{
    node->setVisible(false);
    _world->removeBody(node->getPhysicsBody());
}

void HelloWorld::update(float dt)
{
    Point scrollDecrement = Point(5,0);
    Size winSize = Director::getInstance()->getWinSize();
    // update position on parallax
    _backgroundNode->setPosition(_backgroundNode->getPosition() - scrollDecrement);
    _backgroundNode->updatePosition();
    
    // dem thoi gian
    _nextAsteroidTimer += dt;
    if (_nextAsteroidTimer > _nextAsteroidSpawn) {
        _nextAsteroidTimer = 0;
        float randY = randomValueBetween(0, winSize.height); // vi tri random
        float randDuration = randomValueBetween(2.0, 10.0); // thoi gian di chuyen tren man hinh
        Sprite* asteroid = (Sprite*)_asteroids->at(_newtAsteroid);
        _newtAsteroid++;
        
        // reset index
        if (_newtAsteroid > _asteroids->size()) {
            _newtAsteroid = 0;
        }
        // stop action
        asteroid->stopAllActions();
        asteroid->setPosition(Point(winSize.width + asteroid->getContentSize().width / 2, randY));
        asteroid->setVisible(true);
        // set body
        auto asbody = PhysicsBody::createCircle(asteroid->getContentSize().width / 2 - 15);
        // xu ly va cham
        asbody->setContactTestBitmask(0xf);
        asteroid->setPhysicsBody(asbody);
        asteroid->runAction(Sequence::create(MoveBy::create(randDuration, Point( - winSize.width - asteroid->getContentSize().width, 0)),CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible,this)),NULL));
    }
    
    // game over check
    if (_lives <= 0) {
        _ship->stopAllActions();
        _ship->setVisible(false);
        _world->removeBody(_ship->getPhysicsBody());
        this->endScene(kEndReasonLose);
    } else if (time(NULL) >= _gameOverTime) {
        this->endScene(kEndReasonWin);
    }
}

void HelloWorld::endScene(EndReason endReason)
{
    if (_gameOver) {
        return;
    }
    _gameOver = true;
    Size winSize = Director::getInstance()->getWinSize();
    char mess[10] = "";
    if (endReason == kEndReasonLose) {
        strcpy(mess, "You Lose");
    } else {
        strcpy(mess, "You Win");
    }
    // tao 2 label thong bao
    auto lbl = LabelTTF::create(mess, "Arial", 20);
    lbl->setScale(0.1);
    lbl->setPosition(Point(winSize.width / 2, winSize.height * 0.6));
    this->addChild(lbl);
    
    // tao 1 label reset game
    strcpy(mess, "Restart");
    auto lblReset = LabelTTF::create(mess, "Arial", 20);
    MenuItemLabel* rsItem = MenuItemLabel::create(lblReset, CC_CALLBACK_1(HelloWorld::resetGame, this));
    rsItem->setScale(0.1);
    rsItem->setPosition(Point(winSize.width / 2, winSize.height * 0.4));
    
    Menu* menu = Menu::create(rsItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    rsItem->runAction(ScaleTo::create(0.5, 1.0));
    lbl->runAction(ScaleTo::create(0.5, 1.0));
    this->unscheduleUpdate();
}


void HelloWorld::resetGame(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, scene));
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
