//
//  SushiSprite.h
//  SushiGame
//
//  Created by dluan on 2014/10/23.
//
//

#ifndef __SushiGame__SushiSprite__
#define __SushiGame__SushiSprite__

#include "cocos2d.h"
#include <iostream>

USING_NS_CC;
using namespace cocos2d;

typedef enum {
    DISPLAY_MODE_NORMAL = 0,
    DISPLAY_MODE_HORIZONTAL,
    DISPLAY_MODE_VERTICAL,
} DisplayMode;

class SushiSprite : public Sprite
{
public:
    SushiSprite();
    static SushiSprite* create(int row, int col);
    static float getContentWidth();
    // get random value in a range
    static int randomValueBetween(int low, int high);
    CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int,m_col,Col);
    CC_SYNTHESIZE(int,m_imgIndex,ImgIndex); // loai sushi
    CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove); // danh dau can loai bo
    CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck); // co bo qua kiem tra
    CC_SYNTHESIZE_READONLY(DisplayMode, m_displayMode, DisplayMode); // mode hien thi
    void setDisplayMode(DisplayMode mode); // thiet lap mode
};

#endif /* defined(__SushiGame__SushiSprite__) */
