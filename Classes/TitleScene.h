#ifndef __TITLESCENE_H__
#define __TITLESCENE_H__

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(TitleScene);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

};

#endif // __TITLESCENE_H__
