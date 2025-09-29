#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
#include "../utils/CardEnums.h"
#include "../models/LoadConfigModel.h"
#include"../controllers/SpriteController.h"
#include"FieldCardsLayer.h"
#include"HandCardsLayer.h"
#include"../utils/Tools.h"
USING_NS_CC;
class GameView : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // �˵�����ص�
    void menuClickCallback(cocos2d::Ref* pSender);

    // ʵ��CREATE_FUNC��
    CREATE_FUNC(GameView);
private:
    SpriteController* _controller;
    std::vector<CardSprite*> _allCards;
    Vec2 _handPosition = Vec2(600, 200);//Ĭ������λ��
    void createBackButton();

    bool _isSpriteMoving = false;    
    //// ͼ��ָ��
    FieldCardsLayer* _fieldCardsLayer;
    HandCardsLayer* _handCardsLayer;
    SpriteController* _spriteController;
};

#endif // GAME_VIEW_H
