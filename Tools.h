#ifndef TOOLS_H
#define TOOLS_H

#include"../views/CardSprite.h"

// ���迨Ƭ�Ľ������Ͷ���
using CardOrder = int;

class Tools {
private:
    // ˽�й��캯������ֹ�ⲿʵ����
    Tools() = default;

    // ���ÿ�������͸�ֵ����
    Tools(const Tools&) = delete;
    Tools& operator=(const Tools&) = delete;

public:
    // ��ȡ����ʵ���ľ�̬����
    static Tools& getInstance() {
        static Tools instance; // �ֲ���̬��������ֻ֤��ʼ��һ��
        return instance;
    }
    //������
    CardSprite* findHighestOrderRightmostCard(const std::vector<CardSprite*>& cardSprites) const;
    //�жϾ������Ƿ��б�ľ���
    std::vector<CardSprite*> getCollidingSprites(CardSprite* targetSprite);
    //�ж����������Ƿ���ײ
    bool checkCardCollision(const CardSprite* cardA, const CardSprite* cardB);
};

#endif // TOOLS_H
