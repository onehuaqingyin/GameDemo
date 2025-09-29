#include "SpriteController.h"
#include "../views/GameSceneView.h"
#include "cocos2d.h"
#include"../views/CardSprite.h"
#include"../views/HandCardsLayer.h"
#include"../utils/Tools.h"
USING_NS_CC;

SpriteController* SpriteController::create(GameView* view = nullptr)
{
    SpriteController* controller = new (std::nothrow) SpriteController();
    if (controller && controller->init(view))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool SpriteController::init(GameView* view=nullptr)
{
    this->_isProcessingClick = false;
    if (!Node::init())
    {
        return false;
    }
    std::weak_ptr<GameView> m_gameView;  // 弱引用，不影响生命周期
    return true;
}

void SpriteController::setTargetPosition(Vec2 position)
{
    m_targetPosition = position;
}
//传入待移动的精灵
void SpriteController::onSpriteClicked(CardSprite* sprite, Vec2 position)
{
    std::vector< CardSprite*> sprites = Tools::getInstance().getCollidingSprites(sprite);
    if (sprite || !sprites.empty()) {
        for (CardSprite* card : sprites) {
            if (sprite->getPosition().y > card->getPosition().y) {//因为只能点击Y小的那个
                return;
            }
        }
    }
    if (HandCardsLayer::getInstance())
    {
        CardSprite* highestSprite = Tools::getInstance().findHighestOrderRightmostCard(HandCardsLayer::getInstance()->getCardSprites());
        //两个数字都不等于-1才会执行
        if (static_cast<int>(highestSprite->getFace()) != -1 && static_cast<int>(sprite->getFace()) != -1) {
            //判断原位置的精灵和点击的精灵的点数
            int diff = static_cast<int>(highestSprite->getFace()) -
                static_cast<int>(sprite->getFace());
            // 计算绝对值最小的差异
            int minDiff = abs(diff) % (12 + 1);
            minDiff = minDiff, (12 + 1) - minDiff;
            if (minDiff == 1 || minDiff == -1 ) {
                // 点击的精灵层级设为最高精灵层级+1
                sprite->setPresentationOrder(highestSprite->getPresentationOrder() + 1);

                // 从当前父节点移除
                sprite->retain();
                sprite->removeFromParent();

                // 先添加到目标图层，使层级变化生效
                HandCardsLayer::getInstance()->addChild(sprite);

                // 创建移动动作
                auto moveAction = MoveTo::create(0.2f, position);
                int spriteOrder = sprite->getPresentationOrder();

                // 动作完成后更新层级
                auto finishAction = CallFunc::create([sprite, spriteOrder]() {
                    // 从当前父节点移除
                    sprite->removeFromParent();
                    // 明确添加到正确的图层实例
                    HandCardsLayer::getInstance()->addChild(sprite, spriteOrder);
                    });

                // 添加到图层管理列表
                HandCardsLayer::getInstance()->addCardSprite(sprite);

                // 执行动作序列
                auto sequence = Sequence::create(moveAction, finishAction, nullptr);
                sprite->runAction(sequence);

                sprite->release();
            }
        }
    }
}

//点击手牌区的卡牌，直接将手牌区中presentorder值最大的卡移动到目标位置
void SpriteController::onHandCardSpriteClicked() {
    HandCardsLayer* handCardsLayer = HandCardsLayer::getInstance();
    if (!handCardsLayer) return;

    // 获取所有CardSprite精灵
    std::vector<CardSprite*> cardSprites = handCardsLayer->getCardSprites();
    if (cardSprites.size() < 2) return;
    //筛选最大值的卡
    CardSprite* targetRightmost = Tools::getInstance().findHighestOrderRightmostCard(cardSprites);
    CardSprite* secondRightmost = nullptr;
    float maxX = -1;
// 遍历所有卡片寻找第二靠右的卡片
    for (CardSprite* card : cardSprites) {
        // 跳过目标卡片
        if (card == targetRightmost) {
            continue;
        }

        // 获取当前卡片的x坐标
        float currentX = card->getPositionX();

        // 更新第二靠右的卡片
        if (currentX > maxX && currentX < targetRightmost->getPosition().x) {
            maxX = currentX;
            secondRightmost = card;
        }
    }
    // 异常保护
    if (!targetRightmost || !secondRightmost) return;

    // 目标位置为层级最高的最靠右卡牌的位置
    Vec2 targetPosition = targetRightmost->getPosition();

    // 调整层级
    int newZOrder = targetRightmost->getPresentationOrder() + 1;
    secondRightmost->setPresentationOrder(newZOrder);

    //移动
    auto moveAction = MoveTo::create(0.2f, targetPosition);
    int targetZOrder = secondRightmost->getPresentationOrder();
    // 移动完成后直接调整层级
    auto finishAction = CallFunc::create([secondRightmost, targetZOrder, handCardsLayer]() {
        // 直接在父容器中调整层级
        handCardsLayer->reorderChild(secondRightmost, targetZOrder);
        });
    // 执行序列：先移动，到位后调整层级
    auto sequence = Sequence::create(finishAction, moveAction, nullptr);
    secondRightmost->runAction(sequence);
}
//这个函数用于处理返回按钮
void SpriteController::handReturnButton() {
    // 最上面的一张返回到原来位置
    CardSprite* sprite = Tools::getInstance().findHighestOrderRightmostCard(HandCardsLayer::getInstance()->getCardSprites());
    if (!sprite || sprite->getIsPlayfield())   return; // 主牌区，不可回退

    // 保存原始位置用于后续检查
    Vec2 targetPosition = sprite->getOriginPosition();

    // 移动节点
    sprite->retain();
    //修改层级
    sprite->setPresentationOrder(sprite->getOriginPresentationOrder());
    int spriteOrder = sprite->getPresentationOrder();
    // 创建移动动作，并在移动完成后检查位置
    auto moveAction = MoveTo::create(0.2f, targetPosition);
    auto finishAction = CallFunc::create([sprite, spriteOrder]() {
        // 动作完成后的清理工作
        sprite->removeFromParent();
        HandCardsLayer::getInstance()->addChild(sprite, spriteOrder);
        });

    // 按顺序执行移动和检查动作
    auto sequence = Sequence::create(moveAction, finishAction, nullptr);

    sprite->runAction(sequence);
    sprite->release();
}
