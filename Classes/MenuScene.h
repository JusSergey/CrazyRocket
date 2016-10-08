#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);

    static void GoToMenuScene(){
        cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, MenuScene::createScene()));
    }

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

private:
    void initDrawBackground();

};

#endif // __MENUSCENE_H__
