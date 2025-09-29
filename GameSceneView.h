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

    // 菜单点击回调
    void menuClickCallback(cocos2d::Ref* pSender);

    // 实现CREATE_FUNC宏
    CREATE_FUNC(GameView);
private:
    SpriteController* _controller;
    std::vector<CardSprite*> _allCards;
    Vec2 _handPosition = Vec2(600, 200);//默认手牌位置
    void createBackButton();

    bool _isSpriteMoving = false;    
    //// 图层指针
    FieldCardsLayer* _fieldCardsLayer;
    HandCardsLayer* _handCardsLayer;
    SpriteController* _spriteController;
};

#endif // GAME_VIEW_H
