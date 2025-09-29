#include "GameSceneView.h"
#include "ui/CocosGUI.h"
using namespace CocosDenshion;

Scene* GameView::createScene()
{
    return GameView::create();
}

// �����ļ�����ʱ����ʾ����
static void problemLoading(const char* filename)
{
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameView.cpp\n");
}

// ��ʼ������
bool GameView::init()
{
    if (!Scene::init())
    {
        return false;
    }
    // ��������ӿ�Ƭͼ��
    _fieldCardsLayer = FieldCardsLayer::create();
    _fieldCardsLayer->setContentSize(Size(1080, 1500));
    _fieldCardsLayer->setPosition(Vec2(0, 580));
    this->addChild(_fieldCardsLayer, 1);  // ����ͼ��㼶
    
    HandCardsLayer::getInstance()->setContentSize(Size(1080, 580));
    HandCardsLayer::getInstance()->setPosition(Vec2(0, 0));
    this->addChild(HandCardsLayer::getInstance(), 1);  // ����ͼ��㼶


    // �������¼�
    _controller = SpriteController::create(this);
    // ����չʾ˳��
    static int _presentationOrder = 0;

    // ��ʼ�����ü���ģ��
    LoadConfigModel* loadConfigModel = LoadConfigModel::getInstance();

    // ��ȡJSON�����ļ�
    if (loadConfigModel->loadConfig("Config.json"))
    {
        // �����ϵ���
        std::vector<CardSprite*> fieldCards = loadConfigModel->getPlayfieldCards();
        _fieldCardsLayer->updateFieldCards(fieldCards);
        // �������Ƶ���
        std::vector<CardSprite*> stackCards = loadConfigModel->getStackCards();
        HandCardsLayer::getInstance()->updateHandCards(stackCards);
    }
    //������ť
    createBackButton();
    return true;
}
//�������˰�ť
void GameView::createBackButton()
{
    auto closeItem = MenuItemFont::create(
        "back",
        CC_CALLBACK_1(GameView::menuClickCallback, this));

    // ���ð�ť��СΪ200*100
    closeItem->setContentSize(Size(100, 50));
    closeItem->setAnchorPoint(Vec2(0, 0));
    // ��ȡ���ֵĳߴ�
    Size textSize = closeItem->getContentSize();
    // ���������ڰ�ť��ˮƽ�����λ��
    float x = (200 - textSize.width) / 2;
    // ���������ڰ�ť�ڴ�ֱ�����λ��
    float y = (100 - textSize.height) / 2;
    // ����һ���µ�λ��
    Vec2 textPos = Vec2(x, y);
    // �������ֵ�λ��
    closeItem->setPosition(textPos);

    // ���ð�ťλ��Ϊ(800, 300)
    closeItem->setPosition(Vec2(800, 200));

    // �����˵�����ӵ���ǰ�ڵ�
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);
}

void GameView::menuClickCallback(Ref* pSender)
{
    _spriteController->handReturnButton();
}
