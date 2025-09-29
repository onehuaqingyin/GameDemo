#ifndef FIELD_CARDS_LAYER_H
#define FIELD_CARDS_LAYER_H

#include "cocos2d.h"
#include "CardSprite.h"
#include "../controllers/SpriteController.h"
#include <vector>

class FieldCardsLayer : public cocos2d::LayerColor
{
public:
    static FieldCardsLayer* create();
    virtual bool init() override;

    // 更新场上卡牌
    void updateFieldCards(const std::vector<CardSprite*>& fieldCards);

private:
    // 清除所有卡牌
    void clearAllCards();
    // 初始化触摸监听器
    void initTouchListener();

    // 控制器实例
    SpriteController* _controller;
    // 当前场上的卡牌列表
    std::vector<CardSprite*> _currentFieldCards;
    // 当前点击的卡牌
    CardSprite* _clickedCard;
};

#endif // FIELD_CARDS_LAYER_H
