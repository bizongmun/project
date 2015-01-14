#include "HelloWorldScene.h"
#include "SushiSprite.h"

USING_NS_CC;

#define MATRIX_WIDTH 7
#define MATRIX_HEIGHT 9
#define SUSHI_DISTANCE 1

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
    // tao anh nen
    auto bg = Sprite::create("background.png");
    bg->setAnchorPoint(Point(0, 1));
    bg->setPosition(Point(0, visibleSize.height));
    
    // khoi tao spriteframe
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sushi.plist");
    spriteSheet = SpriteBatchNode::create("sushi.pvr.ccz");
    addChild(spriteSheet);
    
    m_width = MATRIX_WIDTH; // =6
    m_height = MATRIX_HEIGHT; //=8
    m_matrixLeftBottomX = (visibleSize.width - SushiSprite::getContentWidth() * m_width - (m_width - 1) * SUSHI_DISTANCE) / 2;
    m_matrixLeftBottomY = (visibleSize.height - SushiSprite::getContentWidth() * m_height - (m_height - 1) * SUSHI_DISTANCE) / 2;
    int arraySize = sizeof(SushiSprite*)*m_width*m_height;
    m_matrix = (SushiSprite**)malloc(arraySize);
    memset((void*)m_matrix, 0, arraySize);
    
    initMatrix();
    scheduleUpdate();
    // touch event
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

HelloWorld::HelloWorld() : spriteSheet(NULL), m_matrix(NULL), m_width(0), m_height(0), m_matrixLeftBottomX(0), m_matrixLeftBottomY(0), m_isAnimationing(true), m_isNeedFillVacancies(false), m_isTouchEnable(true),
    m_srcSushi(NULL), m_desSushi(NULL), m_movingVertical(true) // roi sushi
{
    
}

HelloWorld::~HelloWorld()
{
    if (m_matrix) {
        free(m_matrix);
    }
}

void HelloWorld::initMatrix()
{
    for (int row = 0; row < m_height; row++) {
        for (int col = 0; col < m_width; col++) {
            createSushi(row, col);
        }
    }
}

void HelloWorld::createSushi(int row, int col)
{
    Size size = Director::getInstance()->getWinSize();
    SushiSprite* sushi = SushiSprite::create(row, col);
    Point endPosition = positionOfItem(row, col);
    Point startPosition = Point(endPosition.x, endPosition.y + size.height / 2);
    sushi->setPosition(startPosition);
    
    float speed = startPosition.y / (2 * size.height);
    
    sushi->runAction(MoveTo::create(speed, endPosition));
    spriteSheet->addChild(sushi);
    
    m_matrix[row * m_width + col] = sushi;
}

Point HelloWorld::positionOfItem(int row, int col)
{
    float x = m_matrixLeftBottomX + (SushiSprite::getContentWidth() + SUSHI_DISTANCE) * col + SushiSprite::getContentWidth() / 2;
    float y = m_matrixLeftBottomY + (SushiSprite::getContentWidth() + SUSHI_DISTANCE) * row + SushiSprite::getContentWidth() / 2;
    return Point(x, y);
}

void HelloWorld::update(float dt)
{
    if (m_isAnimationing) {
        m_isAnimationing = false;
        // duyet toan bo ma tran
        for (int i = 0; i < m_width * m_height; i++) {
            SushiSprite* sushi = m_matrix[i];
            // neu 1 sushi dang action => exit
            if (sushi && sushi->getNumberOfRunningActions() > 0) {
                m_isAnimationing = true;
                break;
            }
        }
    }
    // kiem tra an sushi
    /*if (!m_isAnimationing) {
        checkAndRemoveChain();
    }*/
    m_isTouchEnable = !m_isAnimationing;
    // neu khong co chuyen dong
    if (!m_isAnimationing) {
        if (m_isNeedFillVacancies) {
            m_movingVertical = true;
            m_isAnimationing = true;
            m_isNeedFillVacancies = false;
        } else {
            // kiem tra va an chuoi
            checkAndRemoveChain();
        }
    }
}

void HelloWorld::checkAndRemoveChain()
{
    SushiSprite* sushi;
    for (int i = 0; i < m_width * m_height; i++) {
        sushi = m_matrix[i];
        if (!sushi) {
            continue;
        }
        sushi->setIgnoreCheck(false);
    }
    // kiem tra
    for (int i = 0; i < m_width * m_height; i++) {
        sushi = m_matrix[i];
        if (!sushi) {
            continue;
        }
        if (sushi->getIsNeedRemove()) {
            continue;
        }
        if (sushi->getIgnoreCheck()) {
            continue;
        }
        // dem chuoi
        std::list<SushiSprite*> colChainList;
        getColChain(sushi, colChainList);
        std::list<SushiSprite*> rowChainList;
        getRowChain(sushi, rowChainList);
        std::list<SushiSprite*> &longList = colChainList.size() > rowChainList.size() ? colChainList : rowChainList;
        if (longList.size() < 3) {
            continue;
        }
        
        std::list<SushiSprite*>::iterator iList;
        bool isSetedIgnoreCheck = false;
        for (iList = longList.begin(); iList != longList.end(); iList++) {
            sushi = (SushiSprite*)*iList;
            if (!sushi) {
                continue;
            }
            if (longList.size() > 3) {
                // neu la sushi dac biet
                if (sushi == m_srcSushi || sushi == m_desSushi) {
                    isSetedIgnoreCheck = true;
                    sushi->setIgnoreCheck(true);
                    sushi->setIsNeedRemove(false);
                    sushi->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
                    continue;
                }
            }
            // danh dau loai bo sushi
            markRemove(sushi);
        }
        if (!isSetedIgnoreCheck && longList.size() > 3) {
            sushi->setIgnoreCheck(true);
            sushi->setIsNeedRemove(false);
            sushi->setDisplayMode(m_movingVertical ? DISPLAY_MODE_VERTICAL : DISPLAY_MODE_HORIZONTAL);
        }
    }
    // xoa sushi
    removeSushi();
}

void HelloWorld::getColChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
    chainList.push_back(sushi);
    int neighborCol = sushi->getCol() - 1; // xet cot ben trai
    while (neighborCol >= 0) {
        SushiSprite* neighSushi = m_matrix[sushi->getRow() * m_width + neighborCol];
        if (neighSushi && (neighSushi->getImgIndex() == sushi->getImgIndex())) {
            chainList.push_back(neighSushi);
            neighborCol --;
        } else {
            break;
        }
    }
    // xet cot ben phai
    neighborCol = sushi->getCol() + 1;
    while (neighborCol < m_width) {
        SushiSprite* neighSushi = m_matrix[sushi->getRow() * m_width + neighborCol];
        if (neighSushi && (neighSushi->getImgIndex() == sushi->getImgIndex())
            && !neighSushi->getIsNeedRemove() && !neighSushi->getIgnoreCheck()) {
            chainList.push_back(neighSushi);
            neighborCol++;
        } else {
            break;
        }
    }
}

void HelloWorld::getRowChain(SushiSprite *sushi, std::list<SushiSprite *> &chainList)
{
    chainList.push_back(sushi);
    // xet dong o duoi
    int neighborRow = sushi->getRow() - 1;
    while (neighborRow >= 0) {
        SushiSprite* neighSushi = m_matrix[sushi->getRow() * m_width + neighborRow];
        if (neighSushi && (neighSushi->getImgIndex() == sushi->getImgIndex())
            && !neighSushi->getIsNeedRemove() && !neighSushi->getIgnoreCheck()) {
            chainList.push_back(neighSushi);
            neighborRow --;
        } else {
            break;
        }
    }
    // xet cot ben phai
    neighborRow = sushi->getRow() + 1;
    while (neighborRow < m_width) {
        SushiSprite* neighSushi = m_matrix[sushi->getRow() * m_width + neighborRow];
        if (neighSushi && (neighSushi->getImgIndex() == sushi->getImgIndex())) {
            chainList.push_back(neighSushi);
            neighborRow++;
        } else {
            break;
        }
    }
}

void HelloWorld::explodeSushi(SushiSprite *sushi)
{
    // thoi gian hieu ung 0.3 s
    float time = 0.3;
    sushi->runAction(Sequence::create(
                                      ScaleTo::create(time, 0.0),
                                      CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionEndCallback, this)),
                                      NULL));

    // mo phong vu no
    auto circleSprite = Sprite::create("circle.png");
    addChild(circleSprite, 10);
    circleSprite->setPosition(sushi->getPosition());
    circleSprite->setScale(0);
    // tang kich thuoc len 1 trong thoi gian 3s
    circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0), CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)), NULL));
    // tao hieu ung particle stars
    auto stars = ParticleSystemQuad::create("stars.plist");
    stars->setAutoRemoveOnFinish(true); // tu dong xoa
    stars->setBlendAdditive(false);
    stars->setPosition(sushi->getPosition());
    stars->setScale(0.3);
    addChild(stars, 20);
}

void HelloWorld::fillSushi()
{
    Size size = Director::getInstance()->getWinSize();
    int *colEmptyInfo = (int *)malloc(sizeof(int) * m_width);
    memset((void *)colEmptyInfo, 0, sizeof(int) * m_width);
    // roi sushi khi co khoang trong
    SushiSprite* sushi = NULL;
    // duyet
    for (int col = 0; col < m_width; col++) {
        int removeSushiOfCol = 0;
        for (int row = 0; row < m_height; row++) {
            sushi = m_matrix[row * m_width + col];
            if (sushi == NULL) {
                // dem so sushi bi an
                removeSushiOfCol ++;
            } else {
                // neu ben duoi con` o trong
                if (removeSushiOfCol > 0) {
                    // lam roi xuong
                    int newRow = row - removeSushiOfCol;
                    m_matrix[newRow * m_width + col] = sushi;
                    m_matrix[row * m_width + col] = NULL;
                    Point startPosition = sushi->getPosition();
                    Point endPosition = positionOfItem(newRow, col);
                    float speed = (startPosition.y - endPosition.y) / size.height;
                    sushi->stopAllActions();
                    sushi->runAction(MoveTo::create(speed, endPosition));
                    sushi->setRow(newRow);
                }
            }
        }
        // luu mang sushi bi an
        colEmptyInfo[col] = removeSushiOfCol;
    }
    // tai moi va lam roi cac sushi xuong khoang trong
    for (int col = 0; col < m_width; col++) {
        for (int row = m_height - colEmptyInfo[col]; row < m_height; row++) {
            createAndDropSushi(row, col);
        }
    }
    free(colEmptyInfo);
}

void HelloWorld::createAndDropSushi(int row, int col)
{
    Size size = Director::getInstance()->getWinSize();
    
    SushiSprite *sushi = SushiSprite::create(row, col);
    
    // create animation
    Point endPosition = positionOfItem(row, col);
    Point startPosition = Point(endPosition.x, endPosition.y + size.height / 2);
    sushi->setPosition(startPosition);
    float speed = startPosition.y / (2 * size.height);
    sushi->runAction(MoveTo::create(speed, endPosition));
    // add to BatchNode
    spriteSheet->addChild(sushi);
    
    m_matrix[row * m_width + col] = sushi;
}

void HelloWorld::removeSushi()
{
    // make sequence remove
    m_isAnimationing = true;
    
    for (int i = 0; i < m_height * m_width; i++) {
        SushiSprite* sushi = m_matrix[i];
        if (!sushi) {
            continue;
        }
        if (sushi->getIsNeedRemove()) {
            m_isNeedFillVacancies = true;
            // no cac sushi dac biet
            if (sushi->getDisplayMode() == DISPLAY_MODE_HORIZONTAL) {
                explodeSpecialH(sushi->getPosition());
            } else if (sushi->getDisplayMode() == DISPLAY_MODE_VERTICAL) {
                explodeSpecialV(sushi->getPosition());
            }
            // no sushi binh thuong
            explodeSushi(sushi);
        }
    }
}

void HelloWorld::explodeSpecialH(Point point)
{
    Size size = Director::getInstance()->getWinSize();
    float scaleX = 4 ;
    float scaleY = 0.7 ;
    float time = 0.3;
    Point startPosition = point;
    float speed = 0.6f;
    
    auto colorSpriteRight = Sprite::create("colorHRight.png");
	addChild(colorSpriteRight, 10);
    Point endPosition1 = Point(point.x - size.width, point.y);
    colorSpriteRight->setPosition(startPosition);
    colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                 MoveTo::create(speed, endPosition1),
                                                 CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
                                                 NULL));
    
    auto colorSpriteLeft = Sprite::create("colorHLeft.png");
	addChild(colorSpriteLeft, 10);
    Point endPosition2 = Point(point.x + size.width, point.y);
    colorSpriteLeft->setPosition(startPosition);
    colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                MoveTo::create(speed, endPosition2),
                                                CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
                                                NULL));
    
    
}

void HelloWorld::explodeSpecialV(Point point)
{
    Size size = Director::getInstance()->getWinSize();
    float scaleY = 4 ;
    float scaleX = 0.7 ;
    float time = 0.3;
    Point startPosition = point;
    float speed = 0.6f;
    
    auto colorSpriteDown = Sprite::create("colorVDown.png");
	addChild(colorSpriteDown, 10);
    Point endPosition1 = Point(point.x , point.y - size.height);
    colorSpriteDown->setPosition(startPosition);
    colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                MoveTo::create(speed, endPosition1),
                                                CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
                                                NULL));
    
    auto colorSpriteUp = Sprite::create("colorVUp.png");
	addChild(colorSpriteUp, 10);
    Point endPosition2 = Point(point.x , point.y + size.height);
    colorSpriteUp->setPosition(startPosition);
    colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                              MoveTo::create(speed, endPosition2),
                                              CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
                                              NULL));
}

void HelloWorld::markRemove(SushiSprite *sushi)
{
    if (sushi->getIsNeedRemove()) {
        return;
    }
    
    if (sushi->getIgnoreCheck()) {
        return;
    }
    
    sushi->setIsNeedRemove(true);
    // sushi doc
    if (sushi->getDisplayMode() == DISPLAY_MODE_VERTICAL) {
        for (int row = 0; row < m_height; row++) {
            SushiSprite* tmp = m_matrix[row * m_width + sushi->getCol()];
            if (!tmp || tmp == sushi) {
                continue;
            }
            if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
                tmp->setIsNeedRemove(true);
            } else {
                markRemove(tmp);
            }
        }
    } else if (sushi->getDisplayMode() == DISPLAY_MODE_HORIZONTAL) {
        for (int col = 0; col < m_width; col++) {
            SushiSprite* tmp = m_matrix[sushi->getRow() * m_width + col];
            if (!tmp || tmp == sushi) {
                continue;
            }
            if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
                tmp->setIsNeedRemove(true);
            } else {
                markRemove(tmp);
            }
        }
    }
}

SushiSprite* HelloWorld::sushiOfPoint(Point *point)
{
    SushiSprite* sushi = NULL;
    Rect rect = Rect(0, 0, 0, 0);
    // duyen ma tran sushi
    for (int i = 0; i < m_height * m_width; i++) {
        sushi = m_matrix[i];
        // tinh kich thuoc hinh chu nhat bao quanh sushi
        if (sushi) {
            rect.origin.x = sushi->getPositionX() - (sushi->getContentSize().width / 2);
            rect.origin.y = sushi->getPositionY() - (sushi->getContentSize().height / 2);
            rect.size = sushi->getContentSize();
            
            if (rect.containsPoint(*point)) {
                return sushi;
            }
        }
    }
    return NULL;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_srcSushi = NULL; // sushi nguon
    m_desSushi = NULL; // sushi dich
    if (m_isTouchEnable) { // khi chua an thi duoc phep touch
        auto location = touch->getLocation();
        m_srcSushi = sushiOfPoint(&location);
    }
    return m_isTouchEnable;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!m_srcSushi || !m_isTouchEnable) { // neu touch ra ngoai hoac k dc phep touch
        return;
    }
    int row = m_srcSushi->getRow();
    int col = m_srcSushi->getCol();
    
    auto location = touch->getLocation();
    auto haftWidth = m_srcSushi->getContentSize().width / 2;
    auto haftHeight = m_srcSushi->getContentSize().height / 2;
    
    // check sushi o phia tren
    auto upRect = Rect(m_srcSushi->getPositionX() - haftWidth, m_srcSushi->getPositionY() + haftHeight,
                       m_srcSushi->getContentSize().width, m_srcSushi->getContentSize().height);
    // neu khung nay chua diem touch thi se di chuyen 1 sushi len tren
    if (upRect.containsPoint(location)) {
        row++;
        if (row < m_height) {
            m_desSushi = m_matrix[row * m_width + col];
        }
        m_movingVertical = true;
        swapSushi();
        return;
    }
    
    // check sushi o phia duoi
    auto downRect = Rect(m_srcSushi->getPositionX() - haftWidth, m_srcSushi->getPositionY() - (haftHeight * 3), m_srcSushi->getContentSize().width, m_srcSushi->getContentSize().height);
    // neu khung nay chua diem touch thi se di chuyen 1 sushi xuong duoi
    if (downRect.containsPoint(location)) {
        row--;
        if (row >= 0) {
            m_desSushi = m_matrix[row * m_width + col];
        }
        m_movingVertical = true;
        swapSushi();
        return;
    }
    // check sushi o phia trai
    auto leftRect = Rect(m_srcSushi->getPositionX() - (haftWidth * 3), m_srcSushi->getPositionY() - haftHeight, m_srcSushi->getContentSize().width, m_srcSushi->getContentSize().height);
    // neu khung nay chua diem touch thi se di chuyen 1 sushi xuong duoi
    if (leftRect.containsPoint(location)) {
        col--;
        if (col >= 0) {
            m_desSushi = m_matrix[row * m_width + col];
        }
        m_movingVertical = false;
        swapSushi();
        return;
    }
    // check sushi o phia phai
    auto rightRect = Rect(m_srcSushi->getPositionX() + haftWidth, m_srcSushi->getPositionY() - haftHeight, m_srcSushi->getContentSize().width, m_srcSushi->getContentSize().height);
    // neu khung nay chua diem touch thi se di chuyen 1 sushi xuong duoi
    if (rightRect.containsPoint(location)) {
        col++;
        if (col <= m_width) {
            m_desSushi = m_matrix[row * m_width + col];
        }
        m_movingVertical = false;
        swapSushi();
        return;
    }
}

void HelloWorld::swapSushi()
{
    m_isAnimationing = true;
    m_isTouchEnable = false;
    
    if (!m_srcSushi || !m_desSushi) {
        m_movingVertical = true;
        return;
    }
    
    Point posOfSrc = m_srcSushi->getPosition();
    Point posOfDest = m_desSushi->getPosition();
    float time = 0.2;
    // hoan doi
    m_matrix[m_srcSushi->getRow() * m_width + m_srcSushi->getCol()] = m_desSushi;
    m_matrix[m_desSushi->getRow() * m_width + m_desSushi->getRow()] = m_srcSushi;
    int tmpRow = m_srcSushi->getRow();
    int tmpCol = m_srcSushi->getCol();
    m_srcSushi->setRow(m_desSushi->getRow());
    m_srcSushi->setCol(m_desSushi->getCol());
    m_desSushi->setCol(tmpCol);
    m_desSushi->setRow(tmpRow);
    
    // kiem tra xem co >=3 sushi giong nhau sau khi hoan doi
    std::list<SushiSprite*> colChainListOfFirst;
    getColChain(m_srcSushi, colChainListOfFirst);
    std::list<SushiSprite*> rowChainListOfFirst;
    getRowChain(m_srcSushi, rowChainListOfFirst);
    
    std::list<SushiSprite*> colChainListOfSecond;
    getColChain(m_srcSushi, colChainListOfSecond);
    std::list<SushiSprite*> rowChainListOfSecond;
    getRowChain(m_srcSushi, rowChainListOfSecond);
    
    if (colChainListOfFirst.size() >= 3 || rowChainListOfFirst.size() >= 3 ||
        colChainListOfSecond.size() >= 3 || rowChainListOfSecond.size() >= 3) {
        // dao vi tri sushi
        m_srcSushi->runAction(MoveTo::create(time, posOfDest));
        m_desSushi->runAction(MoveTo::create(time, posOfSrc));
        return;
    }
    
    // neu khong an duoc thi dao ve vi tri cu
    m_matrix[m_srcSushi->getRow() * m_width + m_srcSushi->getCol()] = m_desSushi;
    m_matrix[m_desSushi->getRow() * m_width + m_desSushi->getCol()] = m_srcSushi;
    tmpRow = m_srcSushi->getRow();
    tmpCol = m_srcSushi->getCol();
    m_srcSushi->setRow(m_desSushi->getRow());
    m_srcSushi->setCol(m_desSushi->getCol());
    m_desSushi->setRow(tmpRow);
    m_desSushi->setCol(tmpCol);
    
    // di chuyen 2 buoc
    m_srcSushi->runAction(Sequence::create(
                                           MoveTo::create(time, posOfDest),
                                           MoveTo::create(time, posOfSrc),
                                           NULL));
    m_desSushi->runAction(Sequence::create(
                                            MoveTo::create(time, posOfSrc),
                                            MoveTo::create(time, posOfDest),
                                            NULL));
}

void HelloWorld::actionEndCallback(cocos2d::Node *node)
{
    // loai bo sushi
    SushiSprite* sushi = (SushiSprite*)node;
    m_matrix[sushi->getRow() * m_width + sushi->getCol()] = NULL;
    sushi->removeFromParent();
}