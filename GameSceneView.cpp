#include "GameSceneView.h"
#include "ui/CocosGUI.h"
using namespace CocosDenshion;

Scene* GameView::createScene()
{
    return GameView::create();
}

// 加载文件出错时的提示函数
static void problemLoading(const char* filename)
{
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameView.cpp\n");
}

// 初始化场景
bool GameView::init()
{
    if (!Scene::init())
    {
        return false;
    }
    // 创建并添加卡片图层
    _fieldCardsLayer = FieldCardsLayer::create();
    _fieldCardsLayer->setContentSize(Size(1080, 1500));
    _fieldCardsLayer->setPosition(Vec2(0, 580));
    this->addChild(_fieldCardsLayer, 1);  // 场牌图层层级
    
    HandCardsLayer::getInstance()->setContentSize(Size(1080, 580));
    HandCardsLayer::getInstance()->setPosition(Vec2(0, 0));
    this->addChild(HandCardsLayer::getInstance(), 1);  // 手牌图层层级


    // 处理点击事件
    _controller = SpriteController::create(this);
    // 设置展示顺序
    static int _presentationOrder = 0;

    // 初始化配置加载模型
    LoadConfigModel* loadConfigModel = LoadConfigModel::getInstance();

    // 读取JSON配置文件
    if (loadConfigModel->loadConfig("Config.json"))
    {
        // 处理场上的牌
        std::vector<CardSprite*> fieldCards = loadConfigModel->getPlayfieldCards();
        _fieldCardsLayer->updateFieldCards(fieldCards);
        // 处理手牌的牌
        std::vector<CardSprite*> stackCards = loadConfigModel->getStackCards();
        HandCardsLayer::getInstance()->updateHandCards(stackCards);
    }
    //创建按钮
    createBackButton();
    return true;
}
//创建回退按钮
void GameView::createBackButton()
{
    auto closeItem = MenuItemFont::create(
        "back",
        CC_CALLBACK_1(GameView::menuClickCallback, this));

    // 设置按钮大小为200*100
    closeItem->setContentSize(Size(100, 50));
    closeItem->setAnchorPoint(Vec2(0, 0));
    // 获取文字的尺寸
    Size textSize = closeItem->getContentSize();
    // 计算文字在按钮内水平方向的位置
    float x = (200 - textSize.width) / 2;
    // 计算文字在按钮内垂直方向的位置
    float y = (100 - textSize.height) / 2;
    // 创建一个新的位置
    Vec2 textPos = Vec2(x, y);
    // 设置文字的位置
    closeItem->setPosition(textPos);

    // 设置按钮位置为(800, 300)
    closeItem->setPosition(Vec2(800, 200));

    // 创建菜单并添加到当前节点
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);
}

void GameView::menuClickCallback(Ref* pSender)
{
    _spriteController->handReturnButton();
}
