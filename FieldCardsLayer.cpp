#include "FieldCardsLayer.h"
#include "../views/GameSceneView.h"

using namespace cocos2d;

FieldCardsLayer* FieldCardsLayer::create()
{
    FieldCardsLayer* layer = new (std::nothrow) FieldCardsLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool FieldCardsLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(170, 121, 66, 255)))
    {
        return false;
    }

    // 内部创建控制器实例
    _controller = SpriteController::create(nullptr);
    if (_controller)
    {
        this->addChild(_controller);
    }

    _clickedCard = nullptr;
    _currentFieldCards.clear();

    // 初始化触摸监听
    initTouchListener();

    return true;
}




void FieldCardsLayer::updateFieldCards(const std::vector<CardSprite*>& fieldCards)
{
    clearAllCards();
    _currentFieldCards = fieldCards;

    // 创建一个可排序的卡牌列表副本
    std::vector<CardSprite*> sortedCards = _currentFieldCards;

    // 按Y值从大到小排序（Y值大的先添加，Y值小的后添加）
    std::sort(sortedCards.begin(), sortedCards.end(),
        [](CardSprite* a, CardSprite* b) {
            if (!a) return true;  // 空指针放最前
            if (!b) return false; // 确保空指针排序正确

            // 按Y坐标降序排列，Y值大的排在前面
            return a->getPositionInScene().y > b->getPositionInScene().y;
        });

    // 按排序后的顺序添加卡牌，不特别指定zOrder（使用默认值0）
    for (size_t i = 0; i < sortedCards.size(); i++)
    {
        CardSprite* card = sortedCards[i];
        if (!card) continue;

        // 设置卡牌位置
        Vec2 pos = card->getPositionInScene();
        card->setPosition(pos);
        card->setOriginPosition(pos);
        addChild(card);
    }
}

void FieldCardsLayer::clearAllCards()
{
    removeAllChildrenWithCleanup(true);
    _currentFieldCards.clear();
    _clickedCard = nullptr;
}

void FieldCardsLayer::initTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 触摸开始
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_controller) return false;

        Vec2 touchLocation = touch->getLocation();

        // 遍历当前图层中的所有场上卡牌
        for (auto card : _currentFieldCards) {
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
        if (!_controller || !_clickedCard) return;

        // 检查触摸结束位置是否仍在卡牌上
        Vec2 endLocation = touch->getLocation();
        Vec2 spriteLocation = _clickedCard->convertToNodeSpace(endLocation);
        Rect rect = Rect(0, 0, _clickedCard->getContentSize().width, _clickedCard->getContentSize().height);

        if (rect.containsPoint(spriteLocation)) {
            // 调用控制器处理点击事件，传手牌位置
            _controller->onSpriteClicked(_clickedCard, Vec2(600, 200));
        }

        _clickedCard = nullptr;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}