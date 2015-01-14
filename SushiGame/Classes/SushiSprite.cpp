//
//  SushiSprite.cpp
//  SushiGame
//
//  Created by dluan on 2014/10/23.
//
//

#include "SushiSprite.h"

USING_NS_CC;

#define TOTAL_SUSHI 6 //tong so loai sushi

static const char* sushiArray[TOTAL_SUSHI] = {
    "sushi_1n.png",
    "sushi_2n.png",
    "sushi_3n.png",
    "sushi_4n.png",
    "sushi_5n.png",
    "sushi_6n.png",
};

static const char* sushiVertical[TOTAL_SUSHI] = {
    "sushi_1v.png",
    "sushi_2v.png",
    "sushi_3v.png",
    "sushi_4v.png",
    "sushi_5v.png",
    "sushi_6v.png"
};

static const char* sushiHorizontal[TOTAL_SUSHI] = {
    "sushi_1h.png",
    "sushi_2h.png",
    "sushi_3h.png",
    "sushi_4h.png",
    "sushi_5h.png",
    "sushi_6h.png"
};

SushiSprite::SushiSprite() : m_col(0), m_row(0), m_imgIndex(0), m_ignoreCheck(false),
m_isNeedRemove(false), m_displayMode(DISPLAY_MODE_NORMAL){
    
}

void SushiSprite::setDisplayMode(DisplayMode mode)
{
    m_displayMode = mode;
    SpriteFrame* frame;
    switch (mode) {
        case DISPLAY_MODE_VERTICAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushiVertical[m_imgIndex]);
            break;
        case DISPLAY_MODE_HORIZONTAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sushiHorizontal[m_imgIndex]);
        default:
            return;
    }
    setDisplayFrame(frame);
}

float SushiSprite::getContentWidth()
{
    static float itemWidth = 0;
    if (itemWidth == 0) {
        // tao ra 1 sushi
        auto sushi = Sprite::createWithSpriteFrameName(sushiArray[0]);
        itemWidth = sushi->getContentSize().width;
    }
    return itemWidth;
}

SushiSprite* SushiSprite::create(int row, int col)
{
    SushiSprite* sushi = new SushiSprite();
    sushi->m_row = row;
    sushi->m_col = col;
    //sushi->m_imgIndex = rand() % TOTAL_SUSHI;
    sushi->m_imgIndex = randomValueBetween(0, TOTAL_SUSHI);
    sushi->initWithSpriteFrameName(sushiArray[sushi->m_imgIndex]);
    sushi->autorelease(); //auto release
    return sushi;
}

int SushiSprite::randomValueBetween(int low, int high)
{
    //return (((int)rand() / RAND_MAX)*(high - low)) + low;
    return (int)rand() % high;
}