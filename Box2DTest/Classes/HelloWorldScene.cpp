#include "HelloWorldScene.h"

USING_NS_CC;

#define SCALE_RATIO 32.0

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
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
    b2Vec2 gravity = b2Vec2(0.0f,-10.0f); // Vector gia tốc ( dấu - là chỉ hướng xuống, vì trục y hướng lên trên)
    
    world = new b2World(gravity); // Tạo world với vector gia tốc
    
    // Tạo 1 Sprite quả bóng
    ball = Sprite::create("ball.png");
    
    // Đặt vị trí giữa màn hình
    ball->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    // Đoạn này quan trọng nhất để app body trong Box2D
    
    //---------------------KHUNG VAT LY BOX2D--------------------
    
    bodyShape.m_radius = 45 / SCALE_RATIO; // Bán kính của khối body
    
    //fixtureDef
    fixtureDef.density=10; // Trọng lượng
    fixtureDef.friction=0.8; // Ma sát
    fixtureDef.restitution=0.6; // Đàn hồi
    fixtureDef.shape=&bodyShape; // Trỏ vào bodyShape
    
    //bodyDef
    bodyDef.type = b2_dynamicBody; // Va chạm động
    bodyDef.userData = ball; // gắn với Sprite ball
    
    // Đặt vị trí, và nhớ chuyển đổi đơn vị
    bodyDef.position.Set(ball->getPosition().x/SCALE_RATIO,ball->getPosition().y/SCALE_RATIO);
    
    //ballBody
    ballBody = world->CreateBody(&bodyDef); // Tạo Body
    ballBody->CreateFixture(&fixtureDef); // Tạo các thuộc tính tĩnh
    ballBody->SetGravityScale(10); // Đặt tỷ lệ gia tốc, càng cao rơi càng nhanh
    
    //-----------------------------------------------------------
    
    // Đặt quả bóng vào layer của Scene
    this->addChild(ball, 0);
    
    scheduleUpdate(); // Update lại scene theo thời gian, phải có cái này nhé
    
    return true;
}

void HelloWorld::update(float dt){
    int positionIterations = 10;  // Vị trí
    int velocityIterations = 10; // Vận tốc
    
    deltaTime = dt; // Bước thời gian
    
    // Mô phỏng chuyển động vật lý theo thời gian, hãy nghiên cứu ở đây http://www.box2d.org/manual.html và đây http://www.iforce2d.net/b2dtut/worlds\
    
    // Có thể hiểu thế này, mỗi Step xảy ra trong dt giây , dt này trong file AppDelegate.cpp định nghĩa = dòng lệnh director->setAnimationInterval(1.0 / 60); Bạn thử thay 1/60 = 1/1 xem, rơi cực chậm theo từng giây
    
    world->Step(dt, velocityIterations, positionIterations);
    
    // Duyệt tất cả body của world
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
        // Xét những body có gắn vào Sprite
        if (body->GetUserData())
        {
            
            // Trả về sprite quả bóng ( có mỗi sprite trong bài này )
            Sprite *sprite = (Sprite *) body->GetUserData();
            // Đặt lại vị trí của Sprite này theo vị trí của body ( body sẽ bị rơi dần theo time), nhớ nhân RATIO để chuyển sang tọa độ pixel
            sprite->setPosition(Point(body->GetPosition().x * SCALE_RATIO,body->GetPosition().y * SCALE_RATIO));
            // Đặt khả năng quay tròn
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
            
        }
    world->ClearForces(); // Xóa mọi áp đặt cho Body
    world->DrawDebugData();  // Không hiểu, chắc là debug
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
