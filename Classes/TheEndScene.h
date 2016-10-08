#ifndef __THEENDSCENE_H__
#define __THEENDSCENE_H__

#include "cocos2d.h"

class TheEndScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(TheEndScene);

    static void GoToTheEndScene(){
        CCLOG("repl");
        cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, TheEndScene::createScene()));
        CCLOG("OUTREPL");
    }

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

};

#endif // __THEENDSCENE_H__
