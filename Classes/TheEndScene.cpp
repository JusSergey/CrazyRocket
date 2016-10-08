#include "TheEndScene.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* TheEndScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object

    auto layer = TheEndScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TheEndScene::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Label *label = Label::createWithTTF("THE END", "fonts/Bicubik.ttf", 96);
    label->setPosition(visibleSize / 2);
    addChild(label);

    label->runAction(Sequence::create(DelayTime::create(2), FadeOut::create(5), CallFunc::create([](){ MenuScene::GoToMenuScene(); }), nullptr));

    return true;
}
