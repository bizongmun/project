#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace cocos2d;

class SushiSprite;

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    ~HelloWorld();
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
    virtual void update(float dt) override;
    // bat su kien
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchMoved(Touch* touch, Event* event) override;
private:
    SpriteBatchNode* spriteSheet;
    SushiSprite** m_matrix;
    // kich thuoc ma tran hang va cot
    int m_width;
    int m_height;
    // vi tri can chinh tren man hinh
    float m_matrixLeftBottomX;
    float m_matrixLeftBottomY;
    // tao ma tran
    void initMatrix();
    // tao sushi va cho roi o vi tri bat ky
    void createSushi(int row, int col);
    Point positionOfItem(int row, int col);
    // kiem tra trang thai sushi (dang roi, an)
    bool m_isAnimationing;
    // kiem tra va an day sushi
    void checkAndRemoveChain();
    // kiem tra su ton tai cua sushi theo cot hay khong?
    void getColChain(SushiSprite* sushi, std::list<SushiSprite*> &chainList);
    // kiem tra su ton tai cua sushi theo hang hay khong?
    void getRowChain(SushiSprite* sushi, std::list<SushiSprite*> &chainList);
    // xoa sushi
    void removeSushi();
    // hieu ung no khi an
    void explodeSushi(SushiSprite* sushi);
    // lap day sushi
    void fillSushi();
    // xoa sushi
    void createAndDropSushi(int row, int col);
    
    bool m_isTouchEnable; // cho phep touch hoac khong
    SushiSprite* m_srcSushi;
    SushiSprite* m_desSushi;
    bool m_isNeedFillVacancies;
    bool m_movingVertical;
    
    void actionEndCallback(Node* node); // dung action
    void explodeSpecialH(Point point); // no theo chieu ngang
    void explodeSpecialV(Point point); // no theo chieu doc
    SushiSprite* sushiOfPoint(Point* point);
    void swapSushi(); // dao 2 sushi
    void markRemove(SushiSprite* sushi); // loai bo danh' dau
};

#endif // __HELLOWORLD_SCENE_H__
