#include "HandCardsLayer.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
// 初始化静态成员变量，解决链接错误
HandCardsLayer* HandCardsLayer::_instance = nullptr;

HandCardsLayer::HandCardsLayer()
    : _clickedCard(nullptr)
    , _handPosition(0)
{
}

HandCardsLayer* HandCardsLayer::getInstance() {
    if (!_instance) {
        _instance = new (std::nothrow) HandCardsLayer();
        if (_instance && _instance->init()) {
            _instance->autorelease();
        }
        else {
            CC_SAFE_DELETE(_instance);
            _instance = nullptr;
        }
    }
    return _instance;
}

void HandCardsLayer::destroyInstance() {
    if (_instance) {
        _instance->removeAllChildren();
        _instance = nullptr;
    }
}

// 实现文件 HandCardsLayer.cpp
bool HandCardsLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(148, 33, 146, 255))) // 直接设置背景色和透明度
    {
        return false;
    }

    _clickedCard = nullptr;
    _handPosition = 0;
    _spriteController = SpriteController::create(nullptr);
    initTouchListener();

    return true;
}

void HandCardsLayer::updateHandCards(const std::vector<CardSprite*>& handCards)
{
    clearAllCards();
    _cardSprites = handCards;
    //按x轴从小到大排序
    std::sort(_cardSprites.begin(), _cardSprites.end(), [&](CardSprite* a, CardSprite* b) {
        
        return a->getPosition().x < b->getPosition().x;
        });

    // 添加新的手牌
    for (size_t i = 0; i < _cardSprites.size(); i++)
    {
        CardSprite* card = _cardSprites.at(i);
        if (!card) continue;

        card->setPosition(card->getPositionInScene());
        card->setOriginPosition(card->getPositionInScene());
        card->setPresentationOrder(i);
        card->setOriginPresentationOrder(i);
        addChild(card);
    }
}

void HandCardsLayer::clearAllCards()
{
    removeAllChildren();
    _cardSprites.clear();
    _clickedCard = nullptr;
}

void HandCardsLayer::initTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 触摸开始
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // 遍历当前图层中的所有手牌
        for (auto card : _cardSprites) {
            if (!card || !card->getParent()) continue;

            Vec2 spriteLocation = card->convertToNodeSpace(touchLocation);
            Rect rect = Rect(0, 0, card->getContentSize().width, card->getContentSize().height);

            if (rect.containsPoint(spriteLocation)) {
                _clickedCard = card;
                return true;
            }
        }
        return false;
    };

    // 触摸结束
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (_clickedCard) {
            // 获取触摸结束位置
            Vec2 touchEndPos = touch->getLocation();

            // 检查结束位置是否也在卡牌上
            Vec2 endSpriteLocation = _clickedCard->convertToNodeSpace(touchEndPos);
            Rect rect = Rect(0, 0, _clickedCard->getContentSize().width, _clickedCard->getContentSize().height);

            // 找到横坐标最大的精灵
            float maxX = -FLT_MAX;
            for (auto card : _cardSprites) {
                if (card && card->getParent() && card->getPositionX() > maxX) {
                    maxX = card->getPositionX();
                }
            }

            // 判断条件：开始和结束位置都在卡牌上，且结束位置x小于最大x的精灵
            if (rect.containsPoint(endSpriteLocation) && touchEndPos.x < maxX) {
                _spriteController->onHandCardSpriteClicked();
            }

            // 重置点击的卡牌
            _clickedCard = nullptr;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
//添加精灵
void HandCardsLayer::addCardSprite(CardSprite* card) {
    this->_cardSprites.push_back(card);
}

//查看所有精灵
std::vector<CardSprite*> HandCardsLayer::getCardSprites() {
    return _cardSprites;
}