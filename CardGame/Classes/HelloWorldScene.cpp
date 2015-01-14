#include "HelloWorldScene.h"
#include <random>

USING_NS_CC;

#define CARD_NUM 13 //1種類あたりのカード枚数
#define CARD_TYPE_NUM 4 //　カードの種類
#define CARD_1_POS_X 200 //１番のカード位置
#define CARD_1_POS_Y 320 //１番のカード位置
#define CARD_DISTANCE_X 140
#define CARD_DISTANCE_Y 200
#define ZORDER_SHOW_CARD 1 //表示しているカードのZカード

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
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    initGame();
    
    return true;
}

void HelloWorld::initCards()
{
    // カードのクリア
    _cards.clear();
    
    for (int type = 0; type < 4; type ++) {
        for (int number = 1; number <= 13; number++) {
            // カード情報作成
            Card card;
            card.number = number;
            card.type = (CardType)type;
            
            _cards.push_back(card);
        }
    }
}

Card HelloWorld::getCard()
{
    std::random_device rd;
    std::mt19937 rand = std::mt19937(rd());
    //indexをrandomに取得
    int index = std::uniform_int_distribution<int>(0, (int)_cards.size() - 1)(rand);
    auto card = _cards[index];
    //一時カードから削除
    _cards.erase(_cards.begin() + index);
    
    return card;
}

void HelloWorld::createCard(PosIndex pos)
{
    auto card = Sprite::create("card_spades.png");
    card->setPosition(CARD_1_POS_X + CARD_DISTANCE_X * pos.x,
                      CARD_1_POS_Y + CARD_DISTANCE_Y * pos.y);
    addChild(card, ZORDER_SHOW_CARD);
}

void HelloWorld::showInitCard()
{
    for (int tag = 1; tag <= 10; tag++) {
        auto card = getChildByTag(tag);
        if (card) {
            //カードが残っている場合は削除する
            card->removeFromParent();
        }
    }
    
    // 5x2
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 2; y++) {
            PosIndex pos;
            pos.x = x;
            pos.y = y;
            
            //カードの作成
            createCard(pos);
        }
    }
}

void HelloWorld::initGame()
{
    //カードを初期化する
    initCards();
    
    //カードを表示する
    showInitCard();
}