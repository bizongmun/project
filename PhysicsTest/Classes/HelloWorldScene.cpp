#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    Size winSize = Director::getInstance()->getWinSize();
    PhysicsWorld* world = scene->getPhysicsWorld();
    Vect gravity;
    gravity.setPoint(0, -150);
    world->setGravity(gravity);
    
    // add floor
    Sprite* floor = Sprite::create("floor.png");
    floor->setPosition(Point(winSize.width / 2, floor->getContentSize().height / 2));
    PhysicsBody* floorPb = PhysicsBody::createBox(floor->getContentSize());
    floorPb->setDynamic(false);
    floor->setPhysicsBody(floorPb);
    layer->addChild(floor);
    
    // add character
    Sprite* charac = Sprite::create("chara01.png");
    charac->setPosition(Point(winSize.width / 5, winSize.height / 2));
    PhysicsBody* characPb = PhysicsBody::createCircle(40);
    characPb->setMass(1.0f);
    charac->setPhysicsBody(characPb);
    charac->setTag(1);
    layer->addChild(charac);
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, layer);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, layer);
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
    
    // return the scene
    return scene;
}

Rect HelloWorld::getRect(cocos2d::Node *node)
{
    Point p = node->getPosition();
    int width = node->getContentSize().width;
    int height = node->getContentSize().height;
    return Rect(p.x - (width / 2), p.y - (height / 2), width, height);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    Sprite* character = (Sprite*)this->getChildByTag(1);
    Rect characterRect = getRect(character);
    touchPoint = touch->getLocation();
    CCLOG("begin");
    return characterRect.containsPoint(touchPoint);
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    Sprite* character = (Sprite*)this->getChildByTag(1);
    Point endPoint = touch->getLocation();
    Vect force = Vect(touchPoint.x - endPoint.x, touchPoint.y - endPoint.y) * 4;
    character->getPhysicsBody()->applyImpulse(force);
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
    
    return true;
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
