#ifndef CARD_SPRITE_H
#define CARD_SPRITE_H 
#include "cocos2d.h"
#include<string>
#include"../utils/CardEnums.h"
using namespace cocos2d;
class CardSprite : public Sprite
{
public:
    // ��������ʵ��
    static CardSprite* create(CardSuitType suit, CardFaceType face, const Vec2& cardPosition=Vec2(0,0));

    // ��ʼ������
    bool init(CardSuitType suit, CardFaceType face);

    // ���ÿ��ƻ�ɫ������
    void setCard(CardSuitType suit, CardFaceType face);
    // ע�����¼��ص�
    void setOnClick(const std::function<void(CardSprite*)>& callback) {m_clickCallback = callback;}

    // ��ȡ��ǰ������Ϣ
    CardSuitType getSuit() { return m_suit; }
    CardFaceType getFace() { return m_face; }

    // isPlayfield �� get/set ����
    bool getIsPlayfield() const{ return _isPlayfield; }
    void setIsPlayfield(bool value) { _isPlayfield = value; };

    // isOpen �� get/set ����
    bool getIsOpen() const { return _isOpen; }
    void setIsOpen(bool value) { _isOpen = value; };

    // position �� get/set ����
    Vec2 getPositionInScene() const { return _positionInScene; }
    void setPositionInScene(Vec2 value) { _positionInScene = value; };

    // presentationOrder �� get/set ����
    int getPresentationOrder() const { return _presentationOrder; }
    void setPresentationOrder(int value) { _presentationOrder = value; };

    //����¼�
    bool onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event);
private:
    // ���¿�����ʾ
    void updateCardDisplay();

    // ���ػ�ɫͼƬ
    Sprite* loadSuitSprite(CardSuitType suit);

    // ��������ͼƬ
    std::pair<Sprite*, Sprite*>  loadFaceSprite(CardFaceType face, CardSuitType suit);//һ��һС

private:
    CardSuitType m_suit;  // ��ǰ��ɫ
    CardFaceType m_face;  // ��ǰ����
    std::function<void(CardSprite*)> m_clickCallback; // ����ص�����
    bool _isPlayfield = true;//Ĭ�������ƶ�
    bool _isOpen = true;//Ĭ��������
    Vec2 _positionInScene;//λ��
    std::string _faceStr;
    int _presentationOrder;//����չʾ˳��


    Sprite* m_pSuitSprite= nullptr;  // ��ɫ����
    Sprite* m_pBigFaceSprite = nullptr;  // �����־���
    Sprite* m_pSmallFaceSprite = nullptr;  // С���־���
};
#endif 
