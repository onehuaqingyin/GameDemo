#ifndef LOAD_CONFIG_MODEL_H
#define LOAD_CONFIG_MODEL_H

#include "cocos2d.h"
#include"../views/CardSprite.h"
#include <vector>
#include"utils/CardEnums.h"
#include "json/document.h"
// ���ý���ģ���ࣨModel�㣩
class LoadConfigModel {
public:
    // ����ģʽ
    static LoadConfigModel* getInstance();
    static void destroyInstance();

    //ת��ö��ֵ
    CardSuitType jsonToCardSuit(int suitValue);
    CardFaceType jsonToCardFace(int faceValue);
    //����JSON����
    CardSprite* parseCardFromJson(const rapidjson::Value& cardJson);
    // ���ز������ؿ�����
    bool loadConfig(const std::string& configPath);

    // ��ȡ�����������
    const std::vector<CardSprite*>& getPlayfieldCards() const { return _playfieldCards; }
    const std::vector<CardSprite*>& getStackCards() const { return _stackCards; }

private:
    LoadConfigModel() = default;
    ~LoadConfigModel() = default;

    std::vector<CardSprite*> _playfieldCards; // ���ϵ���
    std::vector<CardSprite*> _stackCards;     // �ѵ�����

    static LoadConfigModel* _instance;
};

#endif // LOAD_CONFIG_MODEL_H
