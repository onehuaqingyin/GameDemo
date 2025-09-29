#include "SpriteController.h"
#include "../views/GameSceneView.h"
#include "cocos2d.h"
#include"../views/CardSprite.h"
#include"../views/HandCardsLayer.h"
#include"../utils/Tools.h"
USING_NS_CC;

SpriteController* SpriteController::create(GameView* view = nullptr)
{
    SpriteController* controller = new (std::nothrow) SpriteController();
    if (controller && controller->init(view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool SpriteController::init(GameView* view=nullptr)
{
    this->_isProcessingClick = false;
    if (!Node::init())
    {
        return false;
    }
    std::weak_ptr<GameView> m_gameView;  // �����ã���Ӱ����������
    return true;
}

void SpriteController::setTargetPosition(Vec2 position)
{
    m_targetPosition = position;
}
//������ƶ��ľ���
void SpriteController::onSpriteClicked(CardSprite* sprite, Vec2 position)
{
    std::vector< CardSprite*> sprites = Tools::getInstance().getCollidingSprites(sprite);
    if (sprite || !sprites.empty()) {
        for (CardSprite* card : sprites) {
            if (sprite->getPosition().y > card->getPosition().y) {//��Ϊֻ�ܵ��YС���Ǹ�
                return;
            }
        }
    }
    if (HandCardsLayer::getInstance())
    {
        CardSprite* highestSprite = Tools::getInstance().findHighestOrderRightmostCard(HandCardsLayer::getInstance()->getCardSprites());
        //�������ֶ�������-1�Ż�ִ��
        if (static_cast<int>(highestSprite->getFace()) != -1 && static_cast<int>(sprite->getFace()) != -1) {
            //�ж�ԭλ�õľ���͵���ľ���ĵ���
            int diff = static_cast<int>(highestSprite->getFace()) -
                static_cast<int>(sprite->getFace());
            // �������ֵ��С�Ĳ���
            int minDiff = abs(diff) % (12 + 1);
            minDiff = minDiff, (12 + 1) - minDiff;
            if (minDiff == 1 || minDiff == -1 ) {
                // ����ľ���㼶��Ϊ��߾���㼶+1
                sprite->setPresentationOrder(highestSprite->getPresentationOrder() + 1);

                // �ӵ�ǰ���ڵ��Ƴ�
                sprite->retain();
                sprite->removeFromParent();

                // ����ӵ�Ŀ��ͼ�㣬ʹ�㼶�仯��Ч
                HandCardsLayer::getInstance()->addChild(sprite);

                // �����ƶ�����
                auto moveAction = MoveTo::create(0.2f, position);
                int spriteOrder = sprite->getPresentationOrder();

                // ������ɺ���²㼶
                auto finishAction = CallFunc::create([sprite, spriteOrder]() {
                    // �ӵ�ǰ���ڵ��Ƴ�
                    sprite->removeFromParent();
                    // ��ȷ��ӵ���ȷ��ͼ��ʵ��
                    HandCardsLayer::getInstance()->addChild(sprite, spriteOrder);
                    });

                // ��ӵ�ͼ������б�
                HandCardsLayer::getInstance()->addCardSprite(sprite);

                // ִ�ж�������
                auto sequence = Sequence::create(moveAction, finishAction, nullptr);
                sprite->runAction(sequence);

                sprite->release();
            }
        }
    }
}

//����������Ŀ��ƣ�ֱ�ӽ���������presentorderֵ���Ŀ��ƶ���Ŀ��λ��
void SpriteController::onHandCardSpriteClicked() {
    HandCardsLayer* handCardsLayer = HandCardsLayer::getInstance();
    if (!handCardsLayer) return;

    // ��ȡ����CardSprite����
    std::vector<CardSprite*> cardSprites = handCardsLayer->getCardSprites();
    if (cardSprites.size() < 2) return;
    //ɸѡ���ֵ�Ŀ�
    CardSprite* targetRightmost = Tools::getInstance().findHighestOrderRightmostCard(cardSprites);
    CardSprite* secondRightmost = nullptr;
    float maxX = -1;
// �������п�ƬѰ�ҵڶ����ҵĿ�Ƭ
    for (CardSprite* card : cardSprites) {
        // ����Ŀ�꿨Ƭ
        if (card == targetRightmost) {
            continue;
        }

        // ��ȡ��ǰ��Ƭ��x����
        float currentX = card->getPositionX();

        // ���µڶ����ҵĿ�Ƭ
        if (currentX > maxX && currentX < targetRightmost->getPosition().x) {
            maxX = currentX;
            secondRightmost = card;
        }
    }
    // �쳣����
    if (!targetRightmost || !secondRightmost) return;

    // Ŀ��λ��Ϊ�㼶��ߵ���ҿ��Ƶ�λ��
    Vec2 targetPosition = targetRightmost->getPosition();

    // �����㼶
    int newZOrder = targetRightmost->getPresentationOrder() + 1;
    secondRightmost->setPresentationOrder(newZOrder);

    //�ƶ�
    auto moveAction = MoveTo::create(0.2f, targetPosition);
    int targetZOrder = secondRightmost->getPresentationOrder();
    // �ƶ���ɺ�ֱ�ӵ����㼶
    auto finishAction = CallFunc::create([secondRightmost, targetZOrder, handCardsLayer]() {
        // ֱ���ڸ������е����㼶
        handCardsLayer->reorderChild(secondRightmost, targetZOrder);
        });
    // ִ�����У����ƶ�����λ������㼶
    auto sequence = Sequence::create(finishAction, moveAction, nullptr);
    secondRightmost->runAction(sequence);
}
//����������ڴ����ذ�ť
void SpriteController::handReturnButton() {
    // �������һ�ŷ��ص�ԭ��λ��
    CardSprite* sprite = Tools::getInstance().findHighestOrderRightmostCard(HandCardsLayer::getInstance()->getCardSprites());
    if (!sprite || sprite->getIsPlayfield())   return; // �����������ɻ���

    // ����ԭʼλ�����ں������
    Vec2 targetPosition = sprite->getOriginPosition();

    // �ƶ��ڵ�
    sprite->retain();
    //�޸Ĳ㼶
    sprite->setPresentationOrder(sprite->getOriginPresentationOrder());
    int spriteOrder = sprite->getPresentationOrder();
    // �����ƶ������������ƶ���ɺ���λ��
    auto moveAction = MoveTo::create(0.2f, targetPosition);
    auto finishAction = CallFunc::create([sprite, spriteOrder]() {
        // ������ɺ��������
        sprite->removeFromParent();
        HandCardsLayer::getInstance()->addChild(sprite, spriteOrder);
        });

    // ��˳��ִ���ƶ��ͼ�鶯��
    auto sequence = Sequence::create(moveAction, finishAction, nullptr);

    sprite->runAction(sequence);
    sprite->release();
}
