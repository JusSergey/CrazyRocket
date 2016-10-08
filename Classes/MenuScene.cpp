#include "LevelsMenu.h"
#include "MenuScene.h"
#include "DataReader.h"
#include "GameScene.h"
#include "Audio.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    DrawNode *backRect = DrawNode::create();
    addChild(backRect, 0);
    backRect->drawSolidRect(Vec2(visibleSize.width*0.25, 0), Vec2(visibleSize.width*0.75, visibleSize.height), Color4F(0.3, 0.2, 0, 0.8));

    MenuItemLabel *menuItems[] = {
        MenuItemLabel::create(Label::createWithTTF("New Game", "fonts/Bicubik.ttf", 48), [](Ref*){ GameScene::StartNewGame(); }),
        MenuItemLabel::create(Label::createWithTTF("Continue", "fonts/Bicubik.ttf", 48), [](Ref*){ GameScene::StartGame(DataReader::getCurrentLevel()); }),
        MenuItemLabel::create(Label::createWithTTF("Levels",   "fonts/Bicubik.ttf", 48), [](Ref*){ LevelsMenu::GoToMenuLevelScene(); }),
        MenuItemLabel::create(Label::createWithTTF("Options",  "fonts/Bicubik.ttf", 48)),
        MenuItemLabel::create(Label::createWithTTF("Exit",     "fonts/Bicubik.ttf", 48), [](Ref*){ GameScene::menuCloseCallback(0); })
    };

    Menu *menu = Menu::create(menuItems[0],
                              menuItems[1],
                              menuItems[2],
                              menuItems[3],
                              menuItems[4],
                              nullptr);

    menu->alignItemsVerticallyWithPadding(menuItems[0]->getContentSize().height / 2);

    addChild(menu, 1);

    initDrawBackground();

    return true;

}

void MenuScene::initDrawBackground()
{
    DrawNode *draw = DrawNode::create(3);
    addChild(draw, -1);
    srand(time(0));

    static const Color4F Colors[] = {
        Color4F(1, 0, 0, 0.5),
        Color4F(1, 0, 0, 0.25),
        Color4F(1, 0, 0, 0.1),

        Color4F(1, 1, 0, 0.5),
        Color4F(1, 1, 0, 0.25),
        Color4F(1, 1, 0, 0.1),

        Color4F::BLACK,

        Color4F(1, 0, 1, 0.5),
        Color4F(1, 0, 1, 0.25),
        Color4F(1, 0, 1, 0.1),

        Color4F(1, 1, 1, 0.5),
        Color4F(1, 1, 1, 0.25),
        Color4F(1, 1, 1, 0.1),

        Color4F(0, 1, 0, 0.5),
        Color4F(0, 1, 0, 0.25),
        Color4F(0, 1, 0, 0.1),

        Color4F::BLACK,

        Color4F(0, 1, 1, 0.5),
        Color4F(0, 1, 1, 0.25),
        Color4F(0, 1, 1, 0.1),

        Color4F(0, 0, 1, 0.5),
        Color4F(0, 0, 1, 0.25),
        Color4F(0, 0, 1, 0.1),

        Color4F::BLACK,

        Color4F(1,1,1,0.5)
    };

    int countColor(sizeof(Colors) / sizeof(decltype(Colors[0])));

    schedule([=, &Colors](float){

        static char value(1);
        if(!(value++ % 16))
            draw->clear();

        draw->drawDot(Vec2(rand_0_1()*visibleSize.width, rand_0_1()*visibleSize.height),
                      rand_0_1()*visibleSize.height,
                      Colors[rand() % countColor]);

    }, 0.5, "TimerDraw");


}
