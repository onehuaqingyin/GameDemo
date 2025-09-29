#ifndef CARD_SPRITE_H
#define CARD_SPRITE_H 
#include "cocos2d.h"
#include<string>
#include"../utils/CardEnums.h"
using namespace cocos2d;
class CardSprite : public Sprite
{
public:
    // 创建卡牌实例
    static CardSprite* create(CardSuitType suit, CardFaceType face, const Vec2& cardPosition=Vec2(0,0));

    // 初始化方法
    bool init(CardSuitType suit, CardFaceType face);

    // 设置卡牌花色和数字
    void setCard(CardSuitType suit, CardFaceType face);
    // 注册点击事件回调
    void setOnClick(const std::function<void(CardSprite*)>& callback) {m_clickCallback = callback;}

    // 获取当前卡牌信息
    CardSuitType getSuit() { return m_suit; }
    CardFaceType getFace() { return m_face; }

    // isPlayfield 的 get/set 方法
    bool getIsPlayfield() const{ return _isPlayfield; }
    void setIsPlayfield(bool value) { _isPlayfield = value; };

    // isOpen 的 get/set 方法
    bool getIsOpen() const { return _isOpen; }
    void setIsOpen(bool value) { _isOpen = value; };

    // position 的 get/set 方法
    Vec2 getPositionInScene() const { return _positionInScene; }
    void setPositionInScene(Vec2 value) { _positionInScene = value; };

    // presentationOrder 的 get/set 方法
    int getPresentationOrder() const { return _presentationOrder; }
    void setPresentationOrder(int value) { _presentationOrder = value; };

    //点击事件
    bool onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event);
private:
    // 更新卡牌显示
    void updateCardDisplay();

    // 加载花色图片
    Sprite* loadSuitSprite(CardSuitType suit);

    // 加载数字图片
    std::pair<Sprite*, Sprite*>  loadFaceSprite(CardFaceType face, CardSuitType suit);//一大一小

private:
    CardSuitType m_suit;  // 当前花色
    CardFaceType m_face;  // 当前数字
    std::function<void(CardSprite*)> m_clickCallback; // 点击回调函数
    bool _isPlayfield = true;//默认在主牌堆
    bool _isOpen = true;//默认是正面
    Vec2 _positionInScene;//位置
    std::string _faceStr;
    int _presentationOrder;//设置展示顺序


    Sprite* m_pSuitSprite= nullptr;  // 花色精灵
    Sprite* m_pBigFaceSprite = nullptr;  // 大数字精灵
    Sprite* m_pSmallFaceSprite = nullptr;  // 小数字精灵
};
#endif 
