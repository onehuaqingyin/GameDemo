#include"Tools.h"
//ɸѡ���ֵ�Ŀ�
CardSprite* Tools::findHighestOrderRightmostCard(const std::vector<CardSprite*>& cardSprites)const
{
    if (cardSprites.empty()) {
        return nullptr; // ���б�ֱ�ӷ���
    }

    // ����1���ҳ����X���꣨��ҵ�Xֵ��
    float maxX = -FLT_MAX;
    for (auto card : cardSprites) {
        if (card && card->getParent()) {
            float cardX = card->getPositionX();
            if (cardX > maxX) {
                maxX = cardX;
            }
        }
    }

    // ����2��ɸѡ��������ҵĿ��ƣ�X����maxX��
    std::vector<CardSprite*> rightmostCards;
    for (auto card : cardSprites) {
        if (card && card->getParent() && card->getPositionX() == maxX) {
            rightmostCards.push_back(card);
        }
    }

    if (rightmostCards.empty()) {
        return nullptr; // �����ϲ����ߵ������������
    }

    // ����3������ҵĿ����У�ѡ��getPresentationOrder()��������
    CardSprite* highestOrderCard = nullptr;
    int maxPresentationOrder = -1;
    for (auto card : rightmostCards) {
        int order = card->getPresentationOrder();
        if (order > maxPresentationOrder) {
            maxPresentationOrder = order;
            highestOrderCard = card;
        }
    }

    return highestOrderCard;
}

std::vector<CardSprite*> Tools::getCollidingSprites(CardSprite* targetSprite) {
    std::vector<CardSprite*> collidingSprites;
    if (!targetSprite) return collidingSprites;

    // ��ȡĿ�꾫��ĸ��ڵ�
    auto parent = targetSprite->getParent();
    if (!parent) return collidingSprites;

    // ��ȡĿ�꾫��İ�Χ��
    Rect targetRect = targetSprite->getBoundingBox();

    // �������ڵ��µ������Ӿ���
    for (auto child : parent->getChildren()) {
        // �ų�����
        if (child == targetSprite) continue;

        // ����Ƿ���CardSprite����
        auto otherSprite = dynamic_cast<CardSprite*>(child);
        if (otherSprite) {
            // ��ȡ��������İ�Χ��
            Rect otherRect = otherSprite->getBoundingBox();

            // ������������Ƿ��ص���������ײ��
            if (targetRect.intersectsRect(otherRect)) {
                collidingSprites.push_back(otherSprite);
            }
        }
    }

    return collidingSprites;
}
//�ж����������Ƿ���ײ
bool Tools::checkCardCollision(const CardSprite* cardA, const CardSprite* cardB)
{
    // ��ָ�밲ȫ���
    if (!cardA || !cardB)
        return false;

    // ��ȡ���ſ�������������ϵ�еı߽����
    cocos2d::Rect rectA = cardA->getBoundingBox();
    cocos2d::Rect rectB = cardB->getBoundingBox();

    // �������Ƿ��ཻ����ײ��
    return rectA.intersectsRect(rectB);
}
