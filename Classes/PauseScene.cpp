#include "PauseScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "Jus.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool PauseLayer::init()
{
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();


    Sprite *_resume= Sprite::create("ButtonResume.png");
    Sprite *_home  = Sprite::create("Home.png");
    Sprite *_reload= Sprite::create("Reload.png");

    ButtonResume = MenuItemSprite::create(_resume, _resume, [this](Ref*){ onResume(); });
    ButtonHome   = MenuItemSprite::create(_home,   _home,   [this](Ref*){ onHome();   });
    ButtonReload = MenuItemSprite::create(_reload, _reload, [this](Ref*){ onReload(); });

    ButtonResume->setPosition(Jus::getCenter());
    ButtonHome  ->setPosition(Jus::getCenter() + Vec2(ButtonResume->getContentSize().width, 0));
    ButtonReload->setPosition(Jus::getCenter() - Vec2(ButtonResume->getContentSize().width, 0));


    Menu *m = Menu::create(ButtonHome, ButtonReload, ButtonResume, nullptr);
    m->setPosition(Vec2::ZERO);
    addChild(m);

    DrawNode* DN = DrawNode::create();
    addChild(DN, -1);
    DN->drawSolidRect(Vec2::ZERO, Vec2(visibleSize.width, visibleSize.height), Color4F(0, 0, 0, 0.5));

    hide(0);

    return true;
}

void PauseLayer::show(float time)
{
    log("pause show");
    setVisible(true);
}

void PauseLayer::hide(float time)
{
    log("pause hide");
    setVisible(false);
}
