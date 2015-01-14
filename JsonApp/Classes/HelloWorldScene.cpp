#include "HelloWorldScene.h"
#include "MyBodyParser.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    //enable debug draw
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = LabelTTF::create("Physics Body Loader Demo", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
	
    status_label = LabelTTF::create("Touch anywhere!", "Arial", 36);
    status_label->setPosition(Point(origin.x + visibleSize.width/2, 1.2*status_label->getContentSize().height));
    this->addChild(status_label);
    
    // add "poke.png"
    sp_2dx = Sprite::create("poke.png");
    
    // position the sprite on the center of the screen
    sp_2dx->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    
    // Nap Body thong qua lop MyBodyParser
	
    MyBodyParser::getInstance()->parseJsonFile("bodies.json");
    
    // Gan Physic Body vao Sprite
    auto _body = MyBodyParser::getInstance()->bodyFormJson(sp_2dx, "Name");
    if (_body != nullptr) {
        _body->setDynamic(false); // Thiet lap Body tinh, va cham nhung ko bi tac dong, dung im luon.
        _body->setCollisionBitmask(0x000000); //Khong tuoc tac vat ly voi vat the khac
        sp_2dx->setPhysicsBody(_body); // set Body
    }
    
    // add the sprite as a child to this layer
    this->addChild(sp_2dx, 0);
	
	
	
    
    //Cac Listener lang nghe su kien Touch EventListenerTouchOneByOne::create();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	// _eventDispatcher->addEventListenerWithScenceGraphPriority(touchListener,this);
    return true;
}

Node* HelloWorld::nodeUnderTouch(Touch *touch)
{
    Node* node = nullptr;
	
	// auto location = this->convertTouchToNodeSpace(touch);
	// auto scene = Director::getInstance()->getRunningScene();
	// auto arr = scene->getPhysicsWorld()->getShapes(location);
    auto location = this->convertTouchToNodeSpace(touch);
    auto scene = Director::getInstance()->getRunningScene();
    auto arr = scene->getPhysicsWorld()->getShapes(location); // 1 mang
    
	// Vong lap de
    for (auto& obj : arr)
    {
        //find it
        if ( obj->getBody()->getNode() == sp_2dx)
        {
            node = obj->getBody()->getNode();
            break;
        }
    }
    return node;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto current_node = nodeUnderTouch(touch);
    
    //get it!
    if (current_node == sp_2dx)
    {
        status_label->setColor(Color3B::GREEN);
        status_label->setString("Ohoo, DUNG CHAM VAO EM!");
    }
    else
    {
        status_label->setColor(Color3B::RED);
        status_label->setString("Haha, RA NGOAI ROI ANH!");
    }
    
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    status_label->setColor(Color3B::WHITE);
    status_label->setString("SO VAO DAU THI SO!");
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
