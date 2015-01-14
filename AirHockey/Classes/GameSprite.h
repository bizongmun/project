//
//  GameSprite.h
//  AirHockey
//
//  Created by dluan on 2014/09/30.
//
//

#ifndef __AirHockey__GameSprite__
#define __AirHockey__GameSprite__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class GameSprite : public Sprite {
    CC_SYNTHESIZE(cocos2d::Point, _nextPosition, NextPosition);
    CC_SYNTHESIZE(cocos2d::Point, _vector, Vector);
    CC_SYNTHESIZE(cocos2d::Touch, _touch, Touch);
    
    GameSprite(void);
    ~GameSprite(void);
    
    static GameSprite* gameSpriteWithFile(const char* pszFileName);
    
    virtual void setPosition(const Vec2& pos);
    
    float radius();
};
#endif /* defined(__AirHockey__GameSprite__) */
