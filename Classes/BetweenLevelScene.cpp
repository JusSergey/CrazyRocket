#include "BetweenLevelScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* BetweenLevelScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object
    auto layer = BetweenLevelScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BetweenLevelScene::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();

    levelTitle = Label::createWithTTF("Level " + StringUtils::toString(GameScene::LevelGame+1), "fonts/Bicubik.ttf", 48);
    levelTitle->setOpacity(0);
    levelTitle->setPosition(visibleSize / 2);

    addChild(levelTitle);

    return true;
}

void BetweenLevelScene::onEnterTransitionDidFinish()
{
    auto action = Sequence::create(FadeIn::create(0.5),
                                   DelayTime::create(1),
                                   FadeOut::create(0.5),
                                   DelayTime::create(0.5),
                                   CallFunc::create([](){ GameScene::GoToGameScene(); }),
                                   nullptr);

    levelTitle->runAction(action);

}
