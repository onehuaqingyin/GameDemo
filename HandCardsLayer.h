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
    // ����ģʽ
    static HandCardsLayer* getInstance();
    // ���ٵ���ʵ��
    static void destroyInstance();

    virtual bool init() override;

    // ʹ��vector��������
    void updateHandCards(const std::vector<CardSprite*>& handCards);

    // ���õ���ص�
    void setClickCallback(const CardClickCallback& callback) {
        _clickCallback = callback;
    }
    //�鿴���о���
    std::vector<CardSprite*> getCardSprites();

    //��Ӿ���
    void HandCardsLayer::addCardSprite(CardSprite* card);

private:
    // ˽�й��캯����ȷ������
    HandCardsLayer();
    // ��ֹ����
    HandCardsLayer(const HandCardsLayer&) = delete;

    // ������п�Ƭ
    void clearAllCards();

    // ��ʼ����������
    void initTouchListener();

    // �洢��ǰ��ʾ������
    std::vector<CardSprite*> _currentHandCards;

    // ��ǰ����Ŀ�Ƭ
    CardSprite* _clickedCard;

    // ����ص�
    CardClickCallback _clickCallback;

    // ����λ�ñ�ʶ
    int _handPosition;

    //��������
    SpriteController* _spriteController;

    //�洢ȫ������
    std::vector<CardSprite*> _cardSprites;

    // ����ʵ��
    static HandCardsLayer* _instance;

};

#endif // HAND_CARDS_LAYER_H
