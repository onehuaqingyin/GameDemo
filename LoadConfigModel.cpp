#include "LoadConfigModel.h"

#include "cocos2d.h"
using namespace cocos2d;
using namespace rapidjson;
LoadConfigModel* LoadConfigModel::_instance = nullptr;

LoadConfigModel* LoadConfigModel::getInstance() {
    //����ģʽ
    if (!_instance) {
        _instance = new (std::nothrow) LoadConfigModel();
    }
    return _instance;

}

void LoadConfigModel::destroyInstance() {
    //����
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

bool LoadConfigModel::loadConfig(const std::string& configPath) {
    // ���֮ǰ������
    _playfieldCards.clear();
    _stackCards.clear();

    // ��ȡJSON�ļ�����
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(configPath);
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(fullPath);

    if (jsonContent.empty()) {
        //������ǿ�
        return false;
    }

    // ����JSON
    Document doc;
    doc.Parse<kParseDefaultFlags>(jsonContent.c_str());

    if (doc.HasParseError()) {
        return false;
    }
    // ����Playfield����
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        // ����rapidjson�ӿڻ�ȡ����
        const rapidjson::Value& rapidArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < rapidArray.Size(); ++i) {
            CardSprite* newCardSprite = parseCardFromJson(rapidArray[i]);
            newCardSprite->setIsPlayfield(true);
            newCardSprite->setPresentationOrder(rapidArray.Size() - i);
            if (newCardSprite)_playfieldCards.push_back(newCardSprite);
        }
    }
    //����Stack����
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        // ����rapidjson�ӿڻ�ȡ����
        const rapidjson::Value& rapidArray = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < rapidArray.Size(); ++i) {
            CardSprite* newCardSprite = parseCardFromJson(rapidArray[i]);
            newCardSprite->setIsPlayfield(false);
            newCardSprite->setPresentationOrder(rapidArray.Size() - i);
            if(newCardSprite)_stackCards.push_back(newCardSprite);
        }
    }
    return true;
}

//����JSON�ĺ���
CardSprite* LoadConfigModel::parseCardFromJson(const rapidjson::Value& cardJson) {
    // ��ʼ������������Ĭ��ֵ
    cocos2d::ValueMap cardValueMap;
    cocos2d::Vec2 tempCardVec(0, 0);
    int faceValue = -1;
    int suitValue = -1;

    // ����CardFace
    if (cardJson.HasMember("CardFace") && cardJson["CardFace"].IsInt()) {
        faceValue = cardJson["CardFace"].GetInt();
        cardValueMap["CardFace"] = cocos2d::Value(faceValue);
    }

    // ����CardSuit
    if (cardJson.HasMember("CardSuit") && cardJson["CardSuit"].IsInt()) {
        suitValue = cardJson["CardSuit"].GetInt();
        cardValueMap["CardSuit"] = cocos2d::Value(suitValue);
    }

    // ����Position
    if (cardJson.HasMember("Position") && cardJson["Position"].IsObject()) {
        const auto& posJson = cardJson["Position"];
        float x = posJson.HasMember("x") && posJson["x"].IsInt() ? posJson["x"].GetInt() : 0;
        float y = posJson.HasMember("y") && posJson["y"].IsInt() ? posJson["y"].GetInt() : 0;
        tempCardVec = cocos2d::Vec2(x, y);
    }

    // ת��Ϊö�����Ͳ������Ч��
    CardFaceType playfieldCardFace = jsonToCardFace(faceValue);
    CardSuitType playfieldCardSuit = jsonToCardSuit(suitValue);

    if (playfieldCardFace == CFT_NONE || playfieldCardSuit == CST_NONE) {
        CCLOG("Invalid card data: face=%d, suit=%d", faceValue, suitValue);
        return nullptr;
    }

    // ������Ƭ����������
    CardSprite* newCardSprite = CardSprite::create(playfieldCardSuit, playfieldCardFace, tempCardVec);
    return newCardSprite;
}
// �� JSON �е� CardFace ��ֵת��Ϊö��
CardFaceType LoadConfigModel::jsonToCardFace(int faceValue) {
    if (faceValue >= 0 && faceValue < CFT_NUM_CARD_FACE_TYPES) {
        return static_cast<CardFaceType>(faceValue);
    }
    else {
        return CFT_NONE;
    }
}
//����ת��Ϊö��ֵ
CardSuitType LoadConfigModel::jsonToCardSuit(int suitValue) {
    // �����ֵ�Ƿ�����Ч��Χ�ڣ�0 �� ö������-1��
    if (suitValue >= 0 && suitValue < CST_NUM_CARD_SUIT_TYPES) {
        return static_cast<CardSuitType>(suitValue); // �Ϸ���ǿת
    }
    else {
        return CST_NONE;
    }
}
