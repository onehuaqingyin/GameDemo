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

    // �ڲ�����������ʵ��
    _controller = SpriteController::create(nullptr);
    if (_controller)
    {
        this->addChild(_controller);
    }

    _clickedCard = nullptr;
    _currentFieldCards.clear();

    // ��ʼ����������
    initTouchListener();

    return true;
}




void FieldCardsLayer::updateFieldCards(const std::vector<CardSprite*>& fieldCards)
{
    clearAllCards();
    _currentFieldCards = fieldCards;

    // ����һ��������Ŀ����б���
    std::vector<CardSprite*> sortedCards = _currentFieldCards;

    // ��Yֵ�Ӵ�С����Yֵ�������ӣ�YֵС�ĺ���ӣ�
    std::sort(sortedCards.begin(), sortedCards.end(),
        [](CardSprite* a, CardSprite* b) {
            if (!a) return true;  // ��ָ�����ǰ
            if (!b) return false; // ȷ����ָ��������ȷ

            // ��Y���꽵�����У�Yֵ�������ǰ��
            return a->getPositionInScene().y > b->getPositionInScene().y;
        });

    // ��������˳����ӿ��ƣ����ر�ָ��zOrder��ʹ��Ĭ��ֵ0��
    for (size_t i = 0; i < sortedCards.size(); i++)
    {
        CardSprite* card = sortedCards[i];
        if (!card) continue;

        // ���ÿ���λ��
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

    // ������ʼ
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_controller) return false;

        Vec2 touchLocation = touch->getLocation();

        // ������ǰͼ���е����г��Ͽ���
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

    // ��������
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (!_controller || !_clickedCard) return;

        // ��鴥������λ���Ƿ����ڿ�����
        Vec2 endLocation = touch->getLocation();
        Vec2 spriteLocation = _clickedCard->convertToNodeSpace(endLocation);
        Rect rect = Rect(0, 0, _clickedCard->getContentSize().width, _clickedCard->getContentSize().height);

        if (rect.containsPoint(spriteLocation)) {
            // ���ÿ������������¼���������λ��
            _controller->onSpriteClicked(_clickedCard, Vec2(600, 200));
        }

        _clickedCard = nullptr;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}