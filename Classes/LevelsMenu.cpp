#include "BetweenLevelScene.h"
#include "Jus.h"
#include "GameScene.h"
#include "LevelsMenu.h"
#include "DataReader.h"
#include "MenuScene.h"
#include "Level.h"

USING_NS_CC;

using StringUtils::toString;

static const short MaxLevelsOnPage = 6;

Scene* LevelsMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'Layer' is an autorelease object

    auto layer = LevelsMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelsMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    indexPage = 0;



    Point points[] = {
        Jus::getDisplayPoint(.15, .70), Jus::getDisplayPoint(.50, .70), Jus::getDisplayPoint(.85, .70),
        Jus::getDisplayPoint(.15, .30), Jus::getDisplayPoint(.50, .30), Jus::getDisplayPoint(.85, .30)
    };

    int i;
    for (i = 0; i < (sizeof(level) / sizeof(level[0])); i++) {

        Sprite *present = Sprite::create("Levels/level_" + toString(i) + "_present.png");
        present->setColor(Color3B(96, 96, 96));

        /* You can start completed levels. Else is lock. */
        if (i <= DataReader::getLevelComplete()) {

            auto callFunc = [i](Ref*) -> void {
                GameScene::LevelGame = i;
                BetweenLevelScene::PresentAndLoadNextLevel();
            };

            MenuItemSprite *item = MenuItemSprite::create(present, present, callFunc);
            item->setPosition(points[i % MaxLevelsOnPage] + Vec2(Jus::getWidth()*(i/MaxLevelsOnPage), 0));
            itemsMenu.pushBack(item);

            auto indexLevelText = Label::createWithTTF(toString(i+1), "fonts/Bicubik.ttf", 58);
            indexLevelText->setPosition(item->getPosition());
            addChild(indexLevelText, LevelTop);

        } else {

            MenuItemSprite *item = MenuItemSprite::create(present, present);
            item->setPosition(points[i % MaxLevelsOnPage] + Vec2(Jus::getWidth()*(i/MaxLevelsOnPage), 0));
            itemsMenu.pushBack(item);

            auto lockLevel = Sprite::create("static/lock.png");
            lockLevel->setScale(0.5);
            lockLevel->setPosition(item->getPosition());
            addChild(lockLevel, LevelTop);

        }

    }

    auto spriteBack = Sprite::create("back.png");
    back = MenuItemSprite::create(spriteBack, spriteBack, [=](Ref*){
        if (indexPage > 0) {
            leftMove();
            indexPage--;
            log("back %d", indexPage);
        }
    });
    back->setPosition(spriteBack->getContentSize()/2 + Size(15, 15));
    itemsMenu.pushBack(back);

    auto spriteNext = Sprite::create("next.png");
    next = MenuItemSprite::create(spriteNext, spriteNext, [=](Ref*){
        if (indexPage+1 < (i % MaxLevelsOnPage)) {
            rightMove();
            indexPage++;
            log("next %d", indexPage);
        }
    }
    );
    next->setPosition(Vec2(Jus::getWidth() - spriteNext->getContentSize().width,
                           spriteNext->getContentSize().height/2 + 15));
    itemsMenu.pushBack(next);

    auto spriteHome = Sprite::create("Home.png");
    spriteHome->setScale(.65);
    spriteHome->setOpacity(0xff * .50);
    spriteHome->setAnchorPoint(Vec2::ZERO);
    home = MenuItemSprite::create(spriteHome, spriteHome, [=](Ref*){ MenuScene::GoToMenuScene(); });
    home->setPosition(Jus::getWidth()/2, spriteHome->getContentSize().height/2 + 15);
    itemsMenu.pushBack(home);

    Menu *menu = Menu::createWithArray(itemsMenu);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    return true;
}

void LevelsMenu::leftMove () {
    this->runAction(MoveBy::create(0.3, Vec2( 1024, 0)));
    auto action = MoveBy::create(0.3, Vec2(-1024, 0));
    next->runAction(action);
    back->runAction(action->clone());
    home->runAction(action->clone());
}

void LevelsMenu::rightMove () {
    this->runAction(MoveBy::create(0.3, Vec2(-1024, 0)));
    auto action = MoveBy::create(0.3, Vec2( 1024, 0));
    next->runAction(action);
    back->runAction(action->clone());
    home->runAction(action->clone());
}
