#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    b2World* world;
    b2Body *ballBody ; // Body của bóng
    b2BodyDef bodyDef; // Định nghĩa cái Body trên
    b2FixtureDef fixtureDef; // Định nghĩa một số thuộc tính tĩnh: ma sát, độ đàn hồi, trọng lượng,v.v.
    b2CircleShape bodyShape; // Hình khối của body
    
    Sprite *ball; // Hình quả bóng
    float deltaTime; // Biến tính thời gian
    void addWall(float w,float h,float px,float py); // Tạo 1 khung Wall bao quanh màn hình để cho quả bóng va chạm
    
    void update(float dt); // Update scene theo thời gian
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
