#ifndef __BETWEENLEVELSCENE_H__
#define __BETWEENLEVELSCENE_H__

#include "cocos2d.h"

class BetweenLevelScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(BetweenLevelScene);

    static void GoToBetweenScene() {
        cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, BetweenLevelScene::createScene()));
    }

    static void PresentAndLoadNextLevel(){
        GoToBetweenScene();
    }

private:
    cocos2d::Size   visibleSize;
    cocos2d::Label* levelTitle;

protected:
    void onEnterTransitionDidFinish();

};

#endif // __BETWEENLEVELSCENE_H__
