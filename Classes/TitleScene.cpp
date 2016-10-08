#include "TitleScene.h"
#include "MenuScene.h"
#include "DataReader.h"
#include "Audio.h"
#include "Jus.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object
    auto layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    DataReader::FirstInitGame();
    Audio::initPreloads();

    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Sprite *title = Sprite::create("title.jpeg");
    title->setOpacity(0);
    title->setScale(2);
    title->setPosition(Jus::getCenter());
    addChild(title);

    auto action = Sequence::create(FadeIn::create(1),
                                   DelayTime::create(1.5),
                                   FadeOut::create(1),
                                   CallFunc::create([](){ MenuScene::GoToMenuScene(); }),
                                   nullptr);
    title->runAction(action);

    auto *label = Label::createWithTTF("use cocos2d-x\nvk.com/cmd_exe", "fonts/Bicubik.ttf", 16);
    label->setOpacity(0);
    label->setAlignment(TextHAlignment::CENTER);
    label->setPosition(Jus::getDisplayPoint(0.5, 0.05));
    label->runAction(Sequence::create(FadeIn::create(1),
                                      DelayTime::create(1.5),
                                      FadeOut::create(1),
                                      nullptr));
    addChild(label);

    return true;
}
