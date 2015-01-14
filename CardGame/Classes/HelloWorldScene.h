#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace cocos2d;

// カード種類
enum CardType {
    Clubs = 0,
    Diamond,
    Hearts,
    Spades
};

// カード
struct Card
{
    int number;
    CardType type;
};

// カード表示位置
struct PosIndex
{
    int x;
    int y;
};

class HelloWorld : public cocos2d::Layer
{
protected:
    std::vector<Card> _cards;
    void initCards();
    Card getCard();
    void createCard(PosIndex pos); //カードを作成する
    void showInitCard(); //ゲーム開始時にカードを10枚表示
    void initGame(); //ゲームを初期化する
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    //implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
