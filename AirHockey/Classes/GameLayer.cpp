#include "GameLayer.h"

USING_NS_CC;

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    _play1Score = 0;
    _play2Score = 0;
    _screenSize = Director::getInstance()->getWinSize();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("court.png");

    // position the sprite on the center of the screen
//    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setPosition(Vec2(_screenSize.width * 0.5, _screenSize.height * 0.5));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    // add player1
    _player1 = GameSprite::gameSpriteWithFile("mallet.png");
    _player1->setPosition(Vec2(_screenSize.width * 0.5, _player1->radius() * 2));
    this->addChild(_player1);
    
    // add player2
    _player2 = GameSprite::gameSpriteWithFile("mallet.png");
    _player2->setPosition(Vec2(_screenSize.width * 0.5, _player2->radius() * 2));
    this->addChild(_player2);
    
    // add ball
    _ball = GameSprite::gameSpriteWithFile("puck.png");
    _ball->setPosition(Vec2(_screenSize.width * 0.5, _screenSize.height * 0.5 - 2 * _ball->radius()));
    this->addChild(_ball);
    
    // create a player list
    _players = __Array::create(_player1, _player2, NULL);
    _players->retain();
    
    // create label 1
    _play1ScoreLabel = Label::createWithTTF("0", "arial.ttf", 30);
    _play1ScoreLabel->setPosition(Vec2(_screenSize.width - 60, _screenSize.height * 0.5 - 80));
    _play1ScoreLabel->setRotation(90);
    this->addChild(_play1ScoreLabel);
    
    // create label 2
    _play2ScoreLabel = Label::createWithTTF("0", "arial.ttf", 30);
    _play2ScoreLabel->setPosition(Vec2(_screenSize.width - 60, _screenSize.height * 0.5 + 80));
    _play2ScoreLabel->setRotation(90);
    this->addChild(_play2ScoreLabel);
    
    // listen for touches
    //setTouchEnabled(true);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // create main loop
    //this->schedule(schedule_selector(GameLayer::update()));
    return true;
}

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(_players);
}

void GameLayer::onTouchesBegan(__Set *pTouches,Event *event)
{
    __SetIterator i;
    Touch *touch;
    Vec2 tap;
    GameSprite *player;
    
    for (i = pTouches->begin(); i!= pTouches->end(); i++) {
        touch = (Touch*)(*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p < 2; p++) {
                player = (GameSprite*)_players->getObjectAtIndex(p);
                if (player->boundingBox().containsPoint(tap)) {
                    player->setTouch(*touch);
                }
            }
        }
    }
}

void GameLayer::onTouchesMoved(__Set *pTouches, Event *event)
{
    __SetIterator i;
    Touch *touch;
    Vec2 tap;
    GameSprite *player;
    for (i = pTouches->begin(); i!= pTouches->end(); i++) {
        touch = (Touch*)(*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p < _players->count(); p++) {
                player = (GameSprite*)_players->getObjectAtIndex(p);
//                if (player->getTouch() == touch) {
                        Vec2 nexPosition = tap;
                        // keep player inside screen
                if (nexPosition.x < player->radius()) {
                    nexPosition.x = player->radius();
                }
                if (nexPosition.x > _screenSize.width - player->radius()) {
                    nexPosition.x = _screenSize.width - player->radius();
                }
                if (nexPosition.y < player->radius()) {
                    nexPosition.y = player->radius();
                }
                if (nexPosition.y > _screenSize.width - player->radius()) {
                    nexPosition.y = _screenSize.width - player->radius();
                }
                // inside his court
                if (player->getPositionX() < _screenSize.height*0.5f) {
                    if (nexPosition.y > _screenSize.height*0.5 - player->radius()) {
                        nexPosition.y = _screenSize.height*0.5 - player->radius();
                    }
                } else {
                    if (nexPosition.y < _screenSize.height*0.5 + player->radius()) {
                        nexPosition.y = _screenSize.height*0.5 + player->radius();
                    }
                }
                player->setNextPosition(nexPosition);
                player->setVector(Point(tap.x - player->getPositionX(), tap.y - player->getPositionY()));
//                }
            }
        }
    }
}

void GameLayer::onTouchesEnd(__Set *pTouches, Event *event)
{
    __SetIterator i;
    Touch *touch;
    Vec2 tap;
    GameSprite *player;
    for (i = pTouches->begin(); i!= pTouches->end(); i++) {
        touch = (Touch*)(*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p < _players->count(); p ++) {
                player = (GameSprite*)_players->getObjectAtIndex(p);
//                if (player->getTouch() != NULL) {
                //player->setTouch(NULL);
                player->setVector(Point(0, 0));
                
//                }
            }
        }
    }
}

void GameLayer::update(float dt)
{
    Vec2 ballNextPosition = _ball->getNextPosition();
    Vec2 ballVector = _ball->getVector();
    ballVector = Vec2(ballVector.x*0.98f, ballVector.y*0.98f);
    ballNextPosition.x += ballVector.x;
    ballNextPosition.y += ballVector.y;
    
    GameSprite *player;
    Vec2 playerNextPosition;
    Vec2 playerVector;
    float squared_radii = pow(_player1->radius() + _ball->radius(), 2);
    for (int p = 0; p < _players->count(); p++) {
        // ball hit a player
        player = (GameSprite*)_players->getObjectAtIndex(p);
        playerNextPosition = player->getNextPosition();
        playerVector = player->getVector();
        float diffx = ballNextPosition.x - player->getPositionX();
        float diffy = ballNextPosition.y - player->getPositionY();
        float distance1 = pow(diffx, 2) + pow(diffy, 2);;
        float distance2 = pow(_ball->getPositionX() - playerNextPosition.x, 2) + pow(_ball->getPositionY() - playerNextPosition.y, 2);
        if (distance1 <= squared_radii || distance2 <= squared_radii) {
            float mag_ball = pow(ballVector.x, 2) + pow(ballVector.y, 2);
            float mag_player = pow(playerVector.x, 2) + pow(playerVector.y, 2);
            float force = sqrt(mag_ball + mag_player);
            float angle = atan2(diffy, diffx);
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            ballNextPosition.x = playerNextPosition.x + (player->radius() + _ball->radius() + force) * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (player->radius() + _ball->radius() + force) * sin(angle);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    // ball hit a wall
    if (ballNextPosition.x < _ball->radius()) {
        ballNextPosition.x = _ball->radius();
        ballVector.x *= -0.8f;
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }
    if (ballNextPosition.x > _screenSize.width - _ball->radius()) {
        ballNextPosition.x = _screenSize.width - _ball->radius();
        ballVector.x *= -0.8f;
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }
    if (ballNextPosition.y > _screenSize.height - _ball->radius()) {
        if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
            _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPosition.y =_screenSize.height - _ball->radius();
            ballVector.y *= -0.8f;
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    if (ballNextPosition.y < _ball->radius()) {
        if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
            _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPosition.y = _ball->radius();
            ballVector.y *= -0.8f;
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    _ball->setVector(ballVector);
    _ball->setNextPosition(ballNextPosition);
    //Check for goal
    if (ballNextPosition.y < -_ball->radius() * 2) {
        this->playerScore(2);
    }
    if (ballNextPosition.y > _screenSize.height + _ball->radius() * 2) {
        this->playerScore(1);
    }
    _player1->setPosition(_player1->getNextPosition());
    _player2->setPosition(_player2->getNextPosition());
    _ball->setPosition(_ball->getNextPosition());
}

void GameLayer::playerScore(int player)
{
    const Vec2 CCPointZero;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("score.wav");
    _ball->setVector(CCPointZero);
    char scoreBuffer[10];
    if (player == 1) {
        _play1Score++;
        sprintf(scoreBuffer, "%i", _play1Score);
        _play1ScoreLabel->setString(scoreBuffer);
        _ball->setNextPosition(Vec2(_screenSize.width *0.5, _screenSize.height*0.5 + 2*_ball->radius()));
    } else {
        _play2Score++;
        sprintf(scoreBuffer, "%i", _play2Score);
        _play2ScoreLabel->setString(scoreBuffer);
        _ball->setNextPosition(Vec2(_screenSize.width *0.5, _screenSize.height*0.5 - 2*_ball->radius()));
    }
    // move players to original position
    _player1->setPosition(Vec2(_screenSize.width * 0.5, _player1->radius() * 2));
    _player2->setPosition(Vec2(_screenSize.width * 0.5, _screenSize.height - _player1->radius() * 2));
    //clear current touches
    _player1->release();
    _player2->release();
//    _player1->setTouch(NULL);
//    _player2->setTouch(NULL);
}
void GameLayer::menuCloseCallback(Ref* pSender)
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
