#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#define _updateAccel (1.0f / 20)
#define LevelPlayer 2
#define LevelTop 4
#define LevelPauseMenu 8

#define g_wall      0xf0
#define g_player    0xf1
#define g_dead      0xf3

NS_CC_BEGIN
using std::vector;
class PauseLayer;
class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    static void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    static void GoToGameScene(){
        Director::getInstance()->replaceScene(TransitionFade::create(1, GameScene::createScene()));
    }

    static void StartGame(int start_level = 0);
    static void StartNewGame();

private:
    PhysicsWorld      *world;
    PhysicsJointLimit *joint;

    Size visibleSize;
    Vec2 origin;
    Size sizeTiled;

public:
    static int LevelGame;
    static bool isJoint;
    static bool isLockGruz;
    bool isContactFire;

private:
    Camera *camera;

private:
    Menu *m_Pausing;
    MenuItemSprite *s_ButtonPause;
    PauseLayer *pauseMenu;
    Sprite *backgroud;

private:
    Menu   *navigatorMenu;
    Sprite *ButtonLock;
    Sprite *ButtonUnlock;
    Sprite *ButtonRelease;
    MenuItemSprite *itemLock;
    MenuItemSprite *itemUnlock;
    MenuItemSprite *itemRelease;

private:
    void onPause();
    void saveGameProgressToFile();
    void initBackgroundSprite();
    void initContentScaleFactor();
    void initCamera();
    void initPauseLayer();
    void initPauseButton();
    void initListeners();
    void initListenerTouches();
    void initNavigation();
    void initJoystick();
    void timerSetCorrectlyPositionCamera(float);
    void finish();

private: // player joystick
    inline bool isTouchJoystick(float joyX, float width) { return (joyX > width*0.3f); }

private:
    Sprite *joystickBegin;
    Sprite *joystickMoved;

};

NS_CC_END

#endif // __GAME_SCENE_H__
