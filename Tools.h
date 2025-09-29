#ifndef TOOLS_H
#define TOOLS_H

#include"../views/CardSprite.h"

// 假设卡片的阶数类型定义
using CardOrder = int;

class Tools {
private:
    // 私有构造函数，防止外部实例化
    Tools() = default;

    // 禁用拷贝构造和赋值操作
    Tools(const Tools&) = delete;
    Tools& operator=(const Tools&) = delete;

public:
    // 获取单例实例的静态方法
    static Tools& getInstance() {
        static Tools instance; // 局部静态变量，保证只初始化一次
        return instance;
    }
    //找手牌
    CardSprite* findHighestOrderRightmostCard(const std::vector<CardSprite*>& cardSprites) const;
    //判断精灵上是否有别的精灵
    std::vector<CardSprite*> getCollidingSprites(CardSprite* targetSprite);
    //判断两个精灵是否碰撞
    bool checkCardCollision(const CardSprite* cardA, const CardSprite* cardB);
};

#endif // TOOLS_H
