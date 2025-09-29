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

    // ���³��Ͽ���
    void updateFieldCards(const std::vector<CardSprite*>& fieldCards);

private:
    // ������п���
    void clearAllCards();
    // ��ʼ������������
    void initTouchListener();

    // ������ʵ��
    SpriteController* _controller;
    // ��ǰ���ϵĿ����б�
    std::vector<CardSprite*> _currentFieldCards;
    // ��ǰ����Ŀ���
    CardSprite* _clickedCard;
};

#endif // FIELD_CARDS_LAYER_H
