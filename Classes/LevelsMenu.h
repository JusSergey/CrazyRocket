#ifndef __LEVELSMENU_H__
#define __LEVELSMENU_H__

#include "cocos2d.h"

class LevelsMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(LevelsMenu);

    static void GoToMenuLevelScene(){
        cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, LevelsMenu::createScene()));
    }

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    short indexPage;
    cocos2d::Vector<cocos2d::MenuItem*> itemsMenu;

    cocos2d::MenuItemSprite *back;
    cocos2d::MenuItemSprite *next;
    cocos2d::MenuItemSprite *home;

private:
    void leftMove();
    void rightMove();

};

#endif // __LEVELSMENU_H__
