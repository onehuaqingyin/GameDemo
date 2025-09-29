#ifndef CARD_SPRITE_H
#define CARD_SPRITE_H 
#include "cocos2d.h"
#include<string>
#include"../utils/CardEnums.h"
using namespace cocos2d;
class CardSprite : public Sprite
{
public:
    virtual void release(); // ��д release ����

    // ��������ʵ��
    static CardSprite* create(CardSuitType suit, CardFaceType face, const Vec2& cardPosition=Vec2(0,0));

    // ��ʼ������
    bool init(CardSuitType suit, CardFaceType face);

    // ���ÿ��ƻ�ɫ������
    void setCard(CardSuitType suit, CardFaceType face);

    // ��ȡ��ǰ������Ϣ
    CardSuitType getSuit();
    CardFaceType getFace();

    // isPlayfield �� get/set 
    bool getIsPlayfield()const;
    void setIsPlayfield(bool value);

    // isOpen �� get/set 
    bool getIsOpen() const;
    void setIsOpen(bool value);

    // position �� get/set 
    Vec2 getPositionInScene() const;
    void setPositionInScene(Vec2 value);

    // presentationOrder �� get/set 
    int getPresentationOrder() const;
    void setPresentationOrder(int value);

    Vec2 getOriginPosition();
    void setOriginPosition(Vec2 value);
    int getOriginPresentationOrder();
    void setOriginPresentationOrder(int value);

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
    Vec2 _originPosition;//��¼ԭʼλ�ã����ڷ���
    int _originPresentationOrder;//��¼ԭʼչʾ˳��

    Sprite* m_pSuitSprite= nullptr;  // ��ɫ����
    Sprite* m_pBigFaceSprite = nullptr;  // �����־���
    Sprite* m_pSmallFaceSprite = nullptr;  // С���־���
};
#endif 
