#include "GameScene.h"
#include "MenuScene.h"
#include "BetweenLevelScene.h"
#include "SimpleAudioEngine.h"
#include "MyBodyParser.h"
#include "DataReader.h"
#include "PauseScene.h"
#include "TheEndScene.h"
#include "Level.h"
#include "Audio.h"
#include "Jus.h"

USING_NS_CC;

#define _1gradus (0.00872638888889*2)
#define angular 0.30f
#define scaledAngular 30.0f
#define updatesFps (1.0f / 10.f)
#define tag_draw 1


bool GameScene::isJoint    = false;
bool GameScene::isLockGruz = true;
int  GameScene::LevelGame  = 0;


using std::string;
using StringUtils::toString;

Scene* GameScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->world = scene->getPhysicsWorld();
    layer->world->setGravity(Vec2(0, -90));

    // add layer as a child to scene
    scene->addChild(layer);
    auto f = PhysicsJointPin::construct(Level::player->getPhysicsBody(),
                               Level::reactive->getPhysicsBody(),
                               Vec2(0, -Level::player->getContentSize().height/2+10),
                               Vec2::ZERO);

    f->createConstraints();
    f->setCollisionEnable(false);

    Level::reactive->getPhysicsBody()->setMass(0.00000000001);

    layer->world->addJoint(f);
    //Level::player->setPosition(Jus::getCenter());

    // return the scene
    return scene;
}

template <typename T>
inline constexpr T ABS (const T &t) {
    return (t > 0 ? t : -t);
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if (!Layer::init())
        return false;

    initContentScaleFactor();

    saveGameProgressToFile();

    isJoint       = false;
    isContactFire = false;
    joint         = nullptr;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin      = Director::getInstance()->getVisibleOrigin();

    initCamera();

    initBackgroundSprite();

    level[LevelGame].init(this);

    setCameraMask((unsigned short)CameraFlag::USER1);

    addChild(camera);

    initJoystick();

    schedule(schedule_selector(GameScene::timerSetCorrectlyPositionCamera), 1.f / 60.f);

    for(auto &wall : Level::RedWalls)
        wall->setCameraMask((unsigned short)CameraFlag::USER1);

    sizeTiled = Level::tiled->getContentSize();

    initPauseButton();
    initPauseLayer();
    initNavigation();
    initListeners();

    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::StartGame(int start_level)
{
    LevelGame = start_level;
    BetweenLevelScene::PresentAndLoadNextLevel();
}

void GameScene::StartNewGame()
{
    DataReader::setCurrentLevel(0);
    DataReader::setLevelComplete(0);
    DataReader::save();
    StartGame(0);
}


void GameScene::onPause()
{
    m_Pausing->setVisible(false);
    pauseMenu->show(1);
    Director::getInstance()->sharedDirector()->pause();
    world->setSpeed(0);

}

void GameScene::saveGameProgressToFile()
{
    DataReader::setCurrentLevel(LevelGame);
    DataReader::setLevelComplete(std::max(DataReader::getLevelComplete(), LevelGame));
    DataReader::save();
}

void GameScene::initBackgroundSprite()
{
    backgroud = Sprite::create("Levels/background.png");
    backgroud->setPosition(Jus::getCenter());
    backgroud->setCameraMask((unsigned short)CameraFlag::USER1);
    addChild(backgroud, 0);
}

void GameScene::initContentScaleFactor()
{
    /* 1024x600 - my resolution size display */
    float ScaleFactorX = Jus::getWidth() / 1024;
    float ScaleFactorY = Jus::getHeight()/ 600;

    if(ScaleFactorX != 1 || ScaleFactorY != 1)
        Director::getInstance()->setContentScaleFactor(std::max(ScaleFactorX, ScaleFactorY));
}

void GameScene::initCamera()
{
    camera = Camera::createOrthographic(visibleSize.width, visibleSize.height, 1, 1000);
    camera->setContentSize(Jus::getSize());
    camera->setAnchorPoint(Vec2(0.5, 0.5));
    camera->setCameraFlag(CameraFlag::USER1);
    camera->setPosition3D(Vec3(Jus::getWidth() / 2, Jus::getHeight() / 2, 100));
}

void GameScene::initPauseLayer()
{

    pauseMenu = PauseLayer::create();

    pauseMenu->onResume = [this]() -> void {
        Director::getInstance()->sharedDirector()->resume();
        world->setSpeed(1);
        pauseMenu->hide(1);
        m_Pausing->setVisible(true);
    };

    pauseMenu->onHome = [this]() -> void {
        Director::getInstance()->sharedDirector()->resume();
        MenuScene::GoToMenuScene();
    };

    pauseMenu->onReload = [this]() -> void {
        Director::getInstance()->sharedDirector()->resume();
        GameScene::StartGame(LevelGame);
    };

    addChild(pauseMenu, LevelTop);
}

void GameScene::initPauseButton()
{
    Sprite *pauseSprite = Sprite::create("ButtonPause.png");

    s_ButtonPause = MenuItemSprite::create(pauseSprite, pauseSprite, [this](Ref*){ onPause(); });

    Size sizePauseButton = pauseSprite->getContentSize();
    s_ButtonPause->setPosition(Jus::getWidth() - sizePauseButton.width/2 - 10, sizePauseButton.height/2 + 10);
    s_ButtonPause->setOpacity(0xff*0.75);


    m_Pausing = Menu::create(s_ButtonPause, nullptr);
    m_Pausing->setPosition(Vec2::ZERO);
    addChild(m_Pausing, LevelTop + 2);
}

void GameScene::initListeners()
{

    initListenerTouches();

    Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(_updateAccel);

    auto listenerAccel = EventListenerAcceleration::create([this](Acceleration *accel, Event *){

         if(ABS(accel->x) > 0.1)
            Jus::addAngular(Level::player->getPhysicsBody(), -accel->x / 5);
         else
            Jus::addAngular(Level::player->getPhysicsBody(), Level::player->getPhysicsBody()->getAngularVelocity() / -8);

    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerAccel, this);


    auto listenerKeys = EventListenerKeyboard::create();
    listenerKeys->onKeyPressed = [this](EventKeyboard::KeyCode code, Event *){

        switch (code) {

        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            Jus::addAngular(Level::player->getPhysicsBody(),  0.5); break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            Jus::addAngular(Level::player->getPhysicsBody(), -0.5); break;

        }

        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeys, this);


    /* CONTACTS */

    for(auto currentRedWall : Level::RedWalls){
        auto listenerContactPD = EventListenerPhysicsContactWithBodies::create(Level::player->getPhysicsBody(),
                                                                               currentRedWall->getPhysicsBody());
        listenerContactPD->onContactBegin = [this](PhysicsContact &){
            BetweenLevelScene::PresentAndLoadNextLevel();
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerContactPD, this);
    }


    auto listenerContactPG = EventListenerPhysicsContactWithBodies::create(Level::player->getPhysicsBody(),
                                                                           Level::gruz->getPhysicsBody());
    listenerContactPG->onContactBegin = [this](PhysicsContact &){

        if(isJoint || !isLockGruz)
            return true;

        joint = PhysicsJointLimit::construct(Level::player->getPhysicsBody(),
                                             Level::gruz->getPhysicsBody(),
                                             Vec2(0, 0),
                                             Vec2(0, Level::gruz->getContentSize().height/2));
        joint->createConstraints();
        joint->setCollisionEnable(false);
        joint->setMax(200);

        Level::gruz->getPhysicsBody()->setVelocityLimit(110);
        Level::gruz->getPhysicsBody()->setAngularVelocityLimit(1);

        world->addJoint(joint);
        isJoint = true;

        itemRelease->setVisible(true);

        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerContactPG, this);

    auto listenFire = EventListenerPhysicsContactWithBodies::create(Level::gruz->getPhysicsBody(),
                                                                    Level::fire->getPhysicsBody());
    listenFire->onContactBegin = [this](PhysicsContact &){
        if(!isContactFire)
            Level::fire->runAction(Sequence::create(ScaleBy::create(2.0, 5),
                                                    DelayTime::create(3),
                                                    CallFunc::create([this](){ finish(); }),
                                                    nullptr));
        isContactFire = true;
        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenFire, this);

}

void GameScene::initListenerTouches()
{
    auto touch = EventListenerTouchOneByOne::create();

    touch->onTouchBegan = [this](Touch *touch, Event*){

        if (isTouchJoystick(touch->getLocation().x, Jus::getWidth())) {

            schedule([=](float){

                static const float maxSpeed = 1024.f;

                auto  body = Level::player->getPhysicsBody();

                float forceY = (touch->getLocation().y - touch->getStartLocation().y) / (visibleSize.height/64);
                float forceX = (touch->getLocation().x - touch->getStartLocation().x) / (visibleSize.height/32);

                auto  vel = body->getVelocity();

                Vec2 vec(forceX * (maxSpeed - ABS(vel.x)) / (maxSpeed/2),
                         forceY * (maxSpeed - ABS(vel.y)) / (maxSpeed/2));

                Jus::addVelocity(body, vec);

            }, (1.0f / 20), "KeyUp");

        } else {

            Audio::PlayMusicEffect();

            float rotate     = Level::player->getRotation()*_1gradus;
            Size  playerSize = Level::player->getContentSize();
            Vec2  vec(sin(rotate), cos(rotate));

            Sprite *bullet = Sprite::create("static/bullet.png");
            bullet->setPhysicsBody(PhysicsBody::createCircle(bullet->getContentSize().width/2));
            bullet->setPosition(Level::player->getPosition() +
                                Vec2(vec.x*playerSize.width /2,
                                     vec.y*playerSize.height/2));

            Jus::addVelocity(bullet->getPhysicsBody(),
                             Vec2(vec.x*600, vec.y*600));

            addChild(bullet, LevelPlayer);
            bullet->setCameraMask((unsigned short)CameraFlag::USER1);

            auto action = Sequence::create(DelayTime::create(3),
                                           FadeOut::create(1),
                                           CallFunc::create([=](){ removeChild(bullet); }),
                                           nullptr);

            bullet->runAction(action);


        }

        return true;
    };

    touch->onTouchEnded = [this](Touch *touch, Event*){
        unschedule("KeyUp");
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

}

void GameScene::initNavigation()
{

    ButtonLock    = Sprite::create("static/lock.png");
    ButtonUnlock  = Sprite::create("static/unlock.png");
    ButtonRelease = Sprite::create("static/release.png");

    ButtonLock   ->setOpacity(0xff * .75);
    ButtonRelease->setOpacity(0xff * .75);
    ButtonUnlock ->setOpacity(0xff * .75);


    itemLock    = MenuItemSprite::create(ButtonLock,    ButtonLock);
    itemUnlock  = MenuItemSprite::create(ButtonUnlock,  ButtonUnlock);
    itemRelease = MenuItemSprite::create(ButtonRelease, ButtonRelease);

    auto callButtonLock = [=](Ref*){
        log("log lock");
        isLockGruz = true;
        itemLock->setVisible(false);
        itemUnlock->setVisible(true);
    };

    auto callButtonUnlock = [=](Ref*){

        log("log unlock");
        isLockGruz = false;
        itemLock->setVisible(true);
        itemUnlock->setVisible(false);
    };

    auto callButtonRelease = [=](Ref*){
        log("log release");
        itemRelease->setVisible(false);

        if(joint){
            world->removeJoint(joint);
            isJoint = false;
            ((DrawNode*)getChildByTag(Level::Tags::DrawJointLine))->clear();
        }
    };

    itemLock->setCallback(callButtonLock);
    itemUnlock->setCallback(callButtonUnlock);
    itemRelease->setCallback(callButtonRelease);

    float scale(0.5);

    for(auto &i : {itemLock, itemUnlock, itemRelease})
        i->setScale(scale);

    itemLock->setPosition((Vec2(15, 15) + (itemLock->getContentSize()/2))*scale);
    itemUnlock->setPosition(itemLock->getPosition());
    itemRelease->setPosition((itemLock->getPosition()/scale + Vec2(itemRelease->getContentSize().width, 0))*scale);

    Menu *menu = Menu::create(itemLock, itemUnlock, itemRelease, nullptr);
    menu->setPosition(Vec2::ZERO);

    addChild(menu, LevelTop+1);

    callButtonLock(nullptr);
    callButtonRelease(nullptr);
}

void GameScene::initJoystick()
{
    joystickBegin = Sprite::create("static/joystick.png");
    joystickMoved = Sprite::createWithTexture(joystickBegin->getTexture());

    joystickBegin->setOpacity(0);
    joystickMoved->setOpacity(0);

    addChild(joystickBegin, LevelTop + 1);
    addChild(joystickMoved, LevelTop + 1);

    auto touches = EventListenerTouchOneByOne::create();

    touches->onTouchBegan = [=](Touch *touch, Event*){

        if (isTouchJoystick(touch->getLocation().x, Jus::getWidth())) {
            joystickBegin->stopAllActions();
            joystickMoved->stopAllActions();
            joystickBegin->setOpacity(0xff*0.35);
            joystickMoved->setOpacity(0xff*0.35);
            joystickBegin->setPosition(touch->getLocation());
            joystickMoved->setPosition(touch->getLocation());
        }
        return true;
    };

    touches->onTouchMoved = [=](Touch *touch, Event*){
        joystickMoved->setPosition(touch->getLocation());
    };

    touches->onTouchEnded = [=](Touch *touch, Event*){
        if (isTouchJoystick(touch->getStartLocation().x, Jus::getWidth())) {
            joystickBegin->runAction(FadeOut::create(0.3));
            joystickMoved->runAction(FadeOut::create(0.3));
        }
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touches, this);
}

void GameScene::timerSetCorrectlyPositionCamera(float)
{
    Vec2 cameraPos = camera->getPosition();
    Vec2 playerPos = Level::player->getPosition();
    Vec2 offsetPos((playerPos.x - cameraPos.x)/32, (playerPos.y - cameraPos.y)/32);

    if((Level::tiled->getPositionX() - sizeTiled.width/2) > ((cameraPos.x - Jus::getWidth()/2) + offsetPos.x) ||
       (Level::tiled->getPositionX() + sizeTiled.width/2) < ((cameraPos.x + Jus::getWidth()/2) + offsetPos.x) ||
       (ABS(offsetPos.x) < 1.f))
        offsetPos.x = 0;

    if((Level::tiled->getPositionY() - sizeTiled.height/2) > ((cameraPos.y - Jus::getHeight()/2) + offsetPos.y) ||
       (Level::tiled->getPositionY() + sizeTiled.height/2) < ((cameraPos.y + Jus::getHeight()/2) + offsetPos.y) ||
       (ABS(offsetPos.y) < 1.f))
        offsetPos.y = 0;

    if(offsetPos.x || offsetPos.y){
        camera->setPosition(cameraPos + offsetPos);
        backgroud->setPosition(backgroud->getPosition() + offsetPos);
    }
}

void GameScene::finish()
{
    LevelGame++;
    if (LevelGame < (sizeof(level) / sizeof(level[0]))) {
        BetweenLevelScene::PresentAndLoadNextLevel();
    } else {
        TheEndScene::GoToTheEndScene();

    }
}
