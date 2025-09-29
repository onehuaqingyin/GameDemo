#include "HandCardsLayer.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
// ��ʼ����̬��Ա������������Ӵ���
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

// ʵ���ļ� HandCardsLayer.cpp
bool HandCardsLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(148, 33, 146, 255))) // ֱ�����ñ���ɫ��͸����
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
    //��x���С��������
    std::sort(_cardSprites.begin(), _cardSprites.end(), [&](CardSprite* a, CardSprite* b) {
        
        return a->getPosition().x < b->getPosition().x;
        });

    // ����µ�����
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

    // ������ʼ
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // ������ǰͼ���е���������
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

    // ��������
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (_clickedCard) {
            // ��ȡ��������λ��
            Vec2 touchEndPos = touch->getLocation();

            // ������λ���Ƿ�Ҳ�ڿ�����
            Vec2 endSpriteLocation = _clickedCard->convertToNodeSpace(touchEndPos);
            Rect rect = Rect(0, 0, _clickedCard->getContentSize().width, _clickedCard->getContentSize().height);

            // �ҵ����������ľ���
            float maxX = -FLT_MAX;
            for (auto card : _cardSprites) {
                if (card && card->getParent() && card->getPositionX() > maxX) {
                    maxX = card->getPositionX();
                }
            }

            // �ж���������ʼ�ͽ���λ�ö��ڿ����ϣ��ҽ���λ��xС�����x�ľ���
            if (rect.containsPoint(endSpriteLocation) && touchEndPos.x < maxX) {
                _spriteController->onHandCardSpriteClicked();
            }

            // ���õ���Ŀ���
            _clickedCard = nullptr;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
//��Ӿ���
void HandCardsLayer::addCardSprite(CardSprite* card) {
    this->_cardSprites.push_back(card);
}

//�鿴���о���
std::vector<CardSprite*> HandCardsLayer::getCardSprites() {
    return _cardSprites;
}