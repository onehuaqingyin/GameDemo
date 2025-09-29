#include "LoadConfigModel.h"

#include "cocos2d.h"
using namespace cocos2d;
using namespace rapidjson;
LoadConfigModel* LoadConfigModel::_instance = nullptr;

LoadConfigModel* LoadConfigModel::getInstance() {
    //单例模式
    if (!_instance) {
        _instance = new (std::nothrow) LoadConfigModel();
    }
    return _instance;

}

void LoadConfigModel::destroyInstance() {
    //销毁
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

bool LoadConfigModel::loadConfig(const std::string& configPath) {
    // 清空之前的数据
    _playfieldCards.clear();
    _stackCards.clear();

    // 读取JSON文件内容
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(configPath);
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(fullPath);

    if (jsonContent.empty()) {
        //传入的是空
        return false;
    }

    // 解析JSON
    Document doc;
    doc.Parse<kParseDefaultFlags>(jsonContent.c_str());

    if (doc.HasParseError()) {
        return false;
    }
    // 解析Playfield数组
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        // 先用rapidjson接口获取数组
        const rapidjson::Value& rapidArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < rapidArray.Size(); ++i) {
            CardSprite* newCardSprite = parseCardFromJson(rapidArray[i]);
            newCardSprite->setIsPlayfield(true);
            newCardSprite->setPresentationOrder(rapidArray.Size() - i);
            if (newCardSprite)_playfieldCards.push_back(newCardSprite);
        }
    }
    //解析Stack数组
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        // 先用rapidjson接口获取数组
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

//解析JSON的函数
CardSprite* LoadConfigModel::parseCardFromJson(const rapidjson::Value& cardJson) {
    // 初始化变量，设置默认值
    cocos2d::ValueMap cardValueMap;
    cocos2d::Vec2 tempCardVec(0, 0);
    int faceValue = -1;
    int suitValue = -1;

    // 解析CardFace
    if (cardJson.HasMember("CardFace") && cardJson["CardFace"].IsInt()) {
        faceValue = cardJson["CardFace"].GetInt();
        cardValueMap["CardFace"] = cocos2d::Value(faceValue);
    }

    // 解析CardSuit
    if (cardJson.HasMember("CardSuit") && cardJson["CardSuit"].IsInt()) {
        suitValue = cardJson["CardSuit"].GetInt();
        cardValueMap["CardSuit"] = cocos2d::Value(suitValue);
    }

    // 解析Position
    if (cardJson.HasMember("Position") && cardJson["Position"].IsObject()) {
        const auto& posJson = cardJson["Position"];
        float x = posJson.HasMember("x") && posJson["x"].IsInt() ? posJson["x"].GetInt() : 0;
        float y = posJson.HasMember("y") && posJson["y"].IsInt() ? posJson["y"].GetInt() : 0;
        tempCardVec = cocos2d::Vec2(x, y);
    }

    // 转换为枚举类型并检查有效性
    CardFaceType playfieldCardFace = jsonToCardFace(faceValue);
    CardSuitType playfieldCardSuit = jsonToCardSuit(suitValue);

    if (playfieldCardFace == CFT_NONE || playfieldCardSuit == CST_NONE) {
        CCLOG("Invalid card data: face=%d, suit=%d", faceValue, suitValue);
        return nullptr;
    }

    // 创建卡片并设置属性
    CardSprite* newCardSprite = CardSprite::create(playfieldCardSuit, playfieldCardFace, tempCardVec);
    return newCardSprite;
}
// 将 JSON 中的 CardFace 数值转换为枚举
CardFaceType LoadConfigModel::jsonToCardFace(int faceValue) {
    if (faceValue >= 0 && faceValue < CFT_NUM_CARD_FACE_TYPES) {
        return static_cast<CardFaceType>(faceValue);
    }
    else {
        return CFT_NONE;
    }
}
//用于转换为枚举值
CardSuitType LoadConfigModel::jsonToCardSuit(int suitValue) {
    // 检查数值是否在有效范围内（0 到 枚举数量-1）
    if (suitValue >= 0 && suitValue < CST_NUM_CARD_SUIT_TYPES) {
        return static_cast<CardSuitType>(suitValue); // 合法则强转
    }
    else {
        return CST_NONE;
    }
}
