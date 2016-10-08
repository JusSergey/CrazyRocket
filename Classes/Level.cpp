#include "Level.h"
#include "Jus.h"
#include "MyBodyParser.h"

USING_NS_CC;
using std::string;

const string Level::PathBomba         = "static/bomba.png"  ;
const string Level::PathPlayer        = "static/player.png" ;
const string Level::PathLock          = "static/lock.png"   ;
const string Level::PathUnlock        = "static/unlock.png" ;
const string Level::PathRedWall       = "static/do.png"     ;
const string Level::PathGruz          = "static/gruz.png"   ;
const string Level::PathReleaseGruz   = "static/release.png";

const string Level::PathLevelsDir = "Levels";
const string Level::PathLevelTMX = "level.tmx";
const string Level::PathLevelBodyes[] = {"body_wall.json", "body_war.json"};

ParticleFire* Level::reactive = nullptr;
Sprite* Level::player = nullptr;
Sprite* Level::gruz = nullptr;
Level::ArrsSprites_t Level::RedWalls;
Sprite* Level::lockGruz = nullptr;
Sprite* Level::unlockGruz = nullptr;
Sprite* Level::releaseGruz = nullptr;
Sprite* Level::bomba = nullptr;

PhysicsJointLimit* Level::limit = nullptr;
ParticleFire* Level::fire = nullptr;
TMXTiledMap*  Level::tiled = nullptr;
Node*         Level::miniMap = nullptr;

void cocos2d::Level::init(cocos2d::Layer *layer)
{
    // Init tiled map
    MyBodyParser::getInstance()->parseJsonFile(getLevelBody());
    tiled = TMXTiledMap::create(getLevelPath());

    tiled->setAnchorPoint(Vec2(0.5, 0.5));
    tiled->setPhysicsBody(MyBodyParser::getInstance()->bodyFormJson(tiled, "body", PhysicsMaterial(0.5, 0.8, 0.5)));
    tiled->getPhysicsBody()->setDynamic(false);
    tiled->getPhysicsBody()->setContactTestBitmask(0b1);
    tiled->setPosition(Jus::getSize() / 2);
    layer->addChild(tiled, 0);
    MyBodyParser::getInstance()->clearCache();

    auto setAbsolutePosition = [tiled](Node *node, const Vec2 &pos) -> void {
        Vec2 _nn = tiled->getPosition() - Vec2(tiled->getContentSize().width  * tiled->getAnchorPoint().x,
                                               tiled->getContentSize().height * tiled->getAnchorPoint().y);
        node->setPosition(_nn + Jus::getPointNode(tiled, pos));

    };

    MyBodyParser::getInstance()->parseJsonFile("static/bodyes.json");


    player = Sprite::create(PathPlayer);
    setAbsolutePosition(player, absoluteCoordinatePlayer);
    //player->setPosition(Jus::getPointNode(tiled, absoluteCoordinatePlayer));
    auto physicsBodyPlayer = MyBodyParser::getInstance()->bodyFormJson(player, "player", PhysicsMaterial(0.4, 0.5, 0.5));
    physicsBodyPlayer->setContactTestBitmask(0b1);
    player->setPhysicsBody(physicsBodyPlayer);
    layer->addChild(player, LevelPlayer);

    gruz = Sprite::create(PathGruz);
    auto physicsBodyGruz = PhysicsBody::createCircle(gruz->getContentSize().width/4);
    setAbsolutePosition(gruz, absoluteCoordinateGruz);
    gruz->setPhysicsBody(physicsBodyGruz);
    physicsBodyGruz->setContactTestBitmask(0b1);
    physicsBodyGruz->setMass(40);
    layer->addChild(gruz, LevelPlayer-1);

    MyBodyParser::getInstance()->clearCache();

    miniMap = Sprite::create();
    miniMap->setOpacity(0xff*0.666);
    miniMap->setPosition(miniMap->getContentSize().width/2 + 15,
                         Jus::getHeight() - miniMap->getContentSize().height/2 - 15);

//    DrawNode *draw = DrawNode::create(3);
//    miniMap->addChild(draw);
//    layer->addChild(miniMap, LevelTop);

    fire = ParticleFire::create();
    fire->setPhysicsBody(PhysicsBody::createBox(Size(30, 30)));
    fire->getPhysicsBody()->setContactTestBitmask(0b1);
    fire->getPhysicsBody()->setRotationEnable(false);
    setAbsolutePosition(fire, absoluteCoordinateFire);
    layer->addChild(fire, LevelPlayer);

    auto draw = DrawNode::create(1);
    draw->setTag(Level::Tags::DrawJointLine);
    draw->setCameraMask((unsigned short)CameraFlag::USER1);
    layer->addChild(draw, LevelTop);

    layer->schedule([draw](float){
        if(GameScene::isJoint){
            draw->clear();
            draw->drawLine(Level::gruz->getPosition(), Level::player->getPosition(), Color4F::RED);
        }
    }, 1.0f/50, "key_draw");

    RedWalls.clear();

    for(auto &i : absoluteCoordinateRedWalls){

        Sprite *tmpRedWall = Sprite::create(PathRedWall);
        tmpRedWall->setPhysicsBody(PhysicsBody::createBox(tmpRedWall->getContentSize()));
        tmpRedWall->getPhysicsBody()->setDynamic(i.dynamic);
        tmpRedWall->getPhysicsBody()->setMass(512);
        tmpRedWall->getPhysicsBody()->setContactTestBitmask(0b1);
        setAbsolutePosition(tmpRedWall, i.position);
        tmpRedWall->setRotation(i.angel);
        layer->addChild(tmpRedWall, LevelPlayer);

        if(!i.dynamic){
            tmpRedWall->setOpacity(0xff / 2);
        }

        RedWalls.push_back(tmpRedWall);

    }

    reactive = ParticleFire::create();
    reactive->setPhysicsBody(PhysicsBody::createBox(Size(1, 1)));
    reactive->getPhysicsBody()->setAngularVelocityLimit(0.00001);
    reactive->getPhysicsBody()->setContactTestBitmask(false);
    reactive->getPhysicsBody()->setCollisionBitmask(false);
    reactive->getPhysicsBody()->setCategoryBitmask(false);
    reactive->setEmitterMode(ParticleSystem::Mode::RADIUS);
    reactive->setLife(0.3);
    reactive->setStartRadius(3);
    reactive->setEndColor(Color4F::BLUE);
    reactive->setStartColor(Color4F::RED);
    layer->addChild(reactive, LevelPlayer-1);

}
