#include"Tools.h"
//筛选最大值的卡
CardSprite* Tools::findHighestOrderRightmostCard(const std::vector<CardSprite*>& cardSprites)const
{
    if (cardSprites.empty()) {
        return nullptr; // 空列表直接返回
    }

    // 步骤1：找出最大X坐标（最靠右的X值）
    float maxX = -FLT_MAX;
    for (auto card : cardSprites) {
        if (card && card->getParent()) {
            float cardX = card->getPositionX();
            if (cardX > maxX) {
                maxX = cardX;
            }
        }
    }

    // 步骤2：筛选出所有最靠右的卡牌（X等于maxX）
    std::vector<CardSprite*> rightmostCards;
    for (auto card : cardSprites) {
        if (card && card->getParent() && card->getPositionX() == maxX) {
            rightmostCards.push_back(card);
        }
    }

    if (rightmostCards.empty()) {
        return nullptr; // 理论上不会走到这里，做个保护
    }

    // 步骤3：从最靠右的卡牌中，选择getPresentationOrder()最大的那张
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

    // 获取目标精灵的父节点
    auto parent = targetSprite->getParent();
    if (!parent) return collidingSprites;

    // 获取目标精灵的包围盒
    Rect targetRect = targetSprite->getBoundingBox();

    // 遍历父节点下的所有子精灵
    for (auto child : parent->getChildren()) {
        // 排除自身
        if (child == targetSprite) continue;

        // 检查是否是CardSprite类型
        auto otherSprite = dynamic_cast<CardSprite*>(child);
        if (otherSprite) {
            // 获取其他精灵的包围盒
            Rect otherRect = otherSprite->getBoundingBox();

            // 检查两个矩形是否重叠（发生碰撞）
            if (targetRect.intersectsRect(otherRect)) {
                collidingSprites.push_back(otherSprite);
            }
        }
    }

    return collidingSprites;
}
//判断两个精灵是否碰撞
bool Tools::checkCardCollision(const CardSprite* cardA, const CardSprite* cardB)
{
    // 空指针安全检查
    if (!cardA || !cardB)
        return false;

    // 获取两张卡牌在世界坐标系中的边界矩形
    cocos2d::Rect rectA = cardA->getBoundingBox();
    cocos2d::Rect rectB = cardB->getBoundingBox();

    // 检查矩形是否相交（碰撞）
    return rectA.intersectsRect(rectB);
}
