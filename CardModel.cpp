#include "CardModel.h"
#include "cocos2d.h"

using namespace cocos2d;

// 实现
CardSprite* CardSprite::create(CardSuitType suit, CardFaceType face, const Vec2& cardPosition)
{
    //创建对象
    CardSprite* pCard = new (std::nothrow) CardSprite();
    pCard->setPositionInScene(cardPosition);
    if (pCard && pCard->init(suit, face))
    {
        pCard->autorelease();
        return pCard;
    }
    CC_SAFE_DELETE(pCard);
    return nullptr;
}

bool CardSprite::init(CardSuitType suit, CardFaceType face)
{
    //对象初始化
    if (!Sprite::init())
    {
        return false;
    }
    //设置牌面花色和数字
    m_suit = suit;
    m_face = face;

    // 设置卡牌背景
    if (!this->initWithFile("res/background.png"))  // 检查资源是否加载成功182*282
    {
        CCLOG("Failed to load card background: res/background.png");
        return false;
    }

    // 获取卡牌尺寸（182x282）
    Size cardSize = this->getContentSize();
    CCLOG("Card size: width=%.0f, height=%.0f", cardSize.width, cardSize.height);

    // 初始化子精灵
    m_pSuitSprite = Sprite::create();
    m_pBigFaceSprite = Sprite::create();
    m_pSmallFaceSprite = Sprite::create();

    // 将子精灵加入父节点
    this->addChild(m_pSuitSprite, 1);  // Z轴值1：显示在背景(0)上方
    this->addChild(m_pBigFaceSprite, 1);
    this->addChild(m_pSmallFaceSprite, 1);

    // 更新显示
    updateCardDisplay();

    return true;
}

void CardSprite::setCard(const CardSuitType suit, const CardFaceType face)
{
    m_suit = suit;
    m_face = face;
    updateCardDisplay();
}

void CardSprite::updateCardDisplay()
{
    // 获取卡牌尺寸作为布局参考
    Size cardSize = this->getContentSize();

    // 更新花色（右上角）
    Sprite* tempSuitSprite = loadSuitSprite(m_suit);
    if (tempSuitSprite) {
        // 移除旧精灵
        if (m_pSuitSprite) {
            m_pSuitSprite->removeFromParent();
        }
        m_pSuitSprite = tempSuitSprite;
        this->addChild(m_pSuitSprite, 1);

        // 右上角布局：距离右边缘和上边缘各10px
        float suitX = cardSize.width * 0.9 - tempSuitSprite->getContentSize().width / 2;
        float suitY = cardSize.height * 0.9 - tempSuitSprite->getContentSize().height / 2;
        m_pSuitSprite->setPosition(Vec2(suitX, suitY));
    }

    // 更新数字（大数字居中，小数字左上角）
    std::pair<Sprite*, Sprite*> tempFaceSprites = loadFaceSprite(m_face, m_suit);
    if (tempFaceSprites.first && tempFaceSprites.second) {
        // 处理大数字（居中）
        if (m_pBigFaceSprite) {
            m_pBigFaceSprite->removeFromParent();
        }
        m_pBigFaceSprite = tempFaceSprites.first;
        this->addChild(m_pBigFaceSprite, 1);

        // 居中布局：卡牌正中心
        m_pBigFaceSprite->setPosition(Vec2(cardSize.width / 2, cardSize.height / 2));

        // 处理小数字（左上角）
        if (m_pSmallFaceSprite) {
            m_pSmallFaceSprite->removeFromParent();
        }
        m_pSmallFaceSprite = tempFaceSprites.second;
        this->addChild(m_pSmallFaceSprite, 1);

        // 左上角布局：距离左边缘和上边缘各10px
        float smallX = tempFaceSprites.second->getContentSize().width;
        float smallY = cardSize.height * 0.9 - tempFaceSprites.second->getContentSize().height / 2;
        m_pSmallFaceSprite->setPosition(Vec2(smallX, smallY));
    }
}

Sprite* CardSprite::loadSuitSprite(CardSuitType suit)
{
    // 统一资源路径格式，确保所有资源放在res目录下
    std::string suitPath;
    switch (suit)
    {
    case CST_CLUBS:
        suitPath = "res/suits/club_3.png";
        break;
    case CST_DIAMONDS:
        suitPath = "res/suits/diamond_4.png";
        break;
    case CST_HEARTS:
        suitPath = "res/suits/heart_2.png";
        break;
    case CST_SPADES:
        suitPath = "res/suits/spade_1.png";
        break;
    default:
        CCLOG("Invalid suit type");
        return nullptr;
    }

    // 加载资源
    Sprite* suitSprite = Sprite::create(suitPath);
    if (!suitSprite) {
        CCLOG("Failed to load suit sprite: %s", suitPath.c_str());
    }
    return suitSprite;
}

std::pair<Sprite*, Sprite*> CardSprite::loadFaceSprite(CardFaceType face, CardSuitType suit)
{
    std::string color;
    std::string number;

    // 确定颜色
    if (suit == CST_CLUBS || suit == CST_SPADES) {
        color = "black";
    }
    else {
        color = "red";
    }

    // 确定数字/字母
    switch (face)
    {
    case CFT_ACE: this->_faceStr = "A"; number = "A"; break;
    case CFT_TWO: this->_faceStr = "2"; number = "2"; break;
    case CFT_THREE:this->_faceStr = "3"; number = "3"; break;
    case CFT_FOUR: this->_faceStr = "4"; number = "4"; break;
    case CFT_FIVE: this->_faceStr = "5"; number = "5"; break;
    case CFT_SIX: this->_faceStr = "6"; number = "6"; break;
    case CFT_SEVEN: this->_faceStr = "7"; number = "7"; break;
    case CFT_EIGHT: this->_faceStr = "8"; number = "8"; break;
    case CFT_NINE: this->_faceStr = "9"; number = "9"; break;
    case CFT_TEN: this->_faceStr = "10"; number = "10"; break;
    case CFT_JACK: this->_faceStr = "J"; number = "J"; break;
    case CFT_QUEEN: this->_faceStr = "Q"; number = "Q"; break;
    case CFT_KING: this->_faceStr = "K"; number = "K"; break;
    default:
        return std::pair<Sprite*, Sprite*>(nullptr, nullptr);
    }

    // 构建路径
    std::string cardPath1 = StringUtils::format("res/number/big_%s_%s.png", color.c_str(), number.c_str());
    std::string cardPath2 = StringUtils::format("res/number/small_%s_%s.png", color.c_str(), number.c_str());

    // 创建精灵
    Sprite* bigFaceSprite = Sprite::create(cardPath1);
    Sprite* smallFaceSprite = Sprite::create(cardPath2);

    if (!bigFaceSprite) {
        CCLOG("Failed to load big face sprite: %s", cardPath1.c_str());
    }
    if (!smallFaceSprite) {
        CCLOG("Failed to load small face sprite: %s", cardPath2.c_str());
    }
    // 返回一大一小
    return std::pair<Sprite*, Sprite*>(bigFaceSprite, smallFaceSprite);
}

//处理点击事件
bool CardSprite::onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (m_clickCallback) {
        m_clickCallback(this); // 通知控制器：当前卡牌被点击
    }
    return true;
}