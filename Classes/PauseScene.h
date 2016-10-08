#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"

NS_CC_BEGIN

class PauseLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    static PauseLayer* create(const std::function<void()> &_onResume = [](){},
                              const std::function<void()> &_onHome   = [](){},
                              const std::function<void()> &_onReload = [](){})
    {
        PauseLayer *pRet = new(std::nothrow) PauseLayer();
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            pRet->onResume = _onResume;
            pRet->onHome   = _onHome;
            pRet->onReload = _onReload;
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }

public:
    void show(float time);
    void hide(float time);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

private:
    DrawNode *pausing;
    Menu *menuPausing;
    MenuItemSprite *ButtonResume;
    MenuItemSprite *ButtonHome;
    MenuItemSprite *ButtonReload;

public:
    std::function<void()> onResume;
    std::function<void()> onHome;
    std::function<void()> onReload;

};

NS_CC_END

#endif // __PAUSE_SCENE_H__
