#ifndef LOAD_CONFIG_MODEL_H
#define LOAD_CONFIG_MODEL_H

#include "cocos2d.h"
#include"../views/CardSprite.h"
#include <vector>
#include"utils/CardEnums.h"
#include "json/document.h"
// 配置解析模型类（Model层）
class LoadConfigModel {
public:
    // 单例模式
    static LoadConfigModel* getInstance();
    static void destroyInstance();

    //转换枚举值
    CardSuitType jsonToCardSuit(int suitValue);
    CardFaceType jsonToCardFace(int faceValue);
    //解析JSON函数
    CardSprite* parseCardFromJson(const rapidjson::Value& cardJson);
    // 加载并解析关卡配置
    bool loadConfig(const std::string& configPath);

    // 获取解析后的数据
    const std::vector<CardSprite*>& getPlayfieldCards() const { return _playfieldCards; }
    const std::vector<CardSprite*>& getStackCards() const { return _stackCards; }

private:
    LoadConfigModel() = default;
    ~LoadConfigModel() = default;

    std::vector<CardSprite*> _playfieldCards; // 场上的牌
    std::vector<CardSprite*> _stackCards;     // 堆叠的牌

    static LoadConfigModel* _instance;
};

#endif // LOAD_CONFIG_MODEL_H
