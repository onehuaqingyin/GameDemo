#ifndef SPRITE_CONTROLLER_H
#define SPRITE_CONTROLLER_H

#include "cocos2d.h"
class GameView;//����
class CardSprite;
class SpriteController : public cocos2d::Node
{
public:
    static SpriteController* create(GameView* view);
    virtual bool init(GameView* view);
    void setTargetPosition(cocos2d::Vec2 position);
    void onSpriteClicked(CardSprite* sprite, cocos2d::Vec2 position = cocos2d::Vec2(600, 200));
    void onHandCardSpriteClicked();
    void handReturnButton();//�����ذ�ť
private:
    GameView* m_gameView;
    cocos2d::Vec2 m_targetPosition;
    bool _isProcessingClick = false;
    // ����ƶ���״̬���
    bool _isMoving = false;
};
#endif // SPRITE_CONTROLLER_H