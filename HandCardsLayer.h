#ifndef HAND_CARDS_LAYER_H
#define HAND_CARDS_LAYER_H

#include "cocos2d.h"
#include "CardSprite.h"
#include <vector>
#include <functional>
#include"../controllers/SpriteController.h"
using CardClickCallback = std::function<void(CardSprite*)>;

class HandCardsLayer : public cocos2d::LayerColor
{
public:
    // 单例模式
    static HandCardsLayer* getInstance();
    // 销毁单例实例
    static void destroyInstance();

    virtual bool init() override;

    // 使用vector更新手牌
    void updateHandCards(const std::vector<CardSprite*>& handCards);

    // 设置点击回调
    void setClickCallback(const CardClickCallback& callback) {
        _clickCallback = callback;
    }
    //查看所有精灵
    std::vector<CardSprite*> getCardSprites();

    //添加精灵
    void HandCardsLayer::addCardSprite(CardSprite* card);

private:
    // 私有构造函数，确保单例
    HandCardsLayer();
    // 禁止拷贝
    HandCardsLayer(const HandCardsLayer&) = delete;

    // 清除现有卡片
    void clearAllCards();

    // 初始化触摸监听
    void initTouchListener();

    // 存储当前显示的手牌
    std::vector<CardSprite*> _currentHandCards;

    // 当前点击的卡片
    CardSprite* _clickedCard;

    // 点击回调
    CardClickCallback _clickCallback;

    // 手牌位置标识
    int _handPosition;

    //控制器类
    SpriteController* _spriteController;

    //存储全部精灵
    std::vector<CardSprite*> _cardSprites;

    // 单例实例
    static HandCardsLayer* _instance;

};

#endif // HAND_CARDS_LAYER_H
