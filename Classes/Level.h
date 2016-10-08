#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "GameScene.h"

NS_CC_BEGIN

using std::string;
using StringUtils::toString;

struct Level {

    typedef std::vector<Sprite*> ArrsSprites_t;
    typedef std::vector<Vec2>    ArrsCoord_t;
    typedef std::vector<string>  ArrsPathBodyes_t;
    typedef struct s{
        Vec2 position;
        float angel;
        bool dynamic;
        s(const Vec2 &v, const float &f, const bool &b):
        position(v), angel(f), dynamic(b){}
    } _RedWall_t;

    typedef std::pair<Vec2, float> RedWall_t;
    typedef std::vector<_RedWall_t> RedWalls_t;

    enum Tags {Player, RedWall, Fire, Gruz, Tiled, Lock, Unlock, DrawJointLine};
    enum BitmaskStatusBodyes {Wars = 0b01, Walls = 0b10};
    int bitmaskBodyes;

    Vec2 absoluteCoordinatePlayer;
    Vec2 absoluteCoordinateFire;
    Vec2 absoluteCoordinateGruz;
    Vec2 absoluteCoordinateBomba;

    RedWalls_t absoluteCoordinateRedWalls;

    int indexLevel;

    static const string PathLevelsDir;
    static const string PathBomba;
    static const string PathPlayer;
    static const string PathLock;
    static const string PathUnlock;
    static const string PathRedWall;
    static const string PathGruz;
    static const string PathReleaseGruz;
    static const string PathStaticBodyes;
    static const string PathLevelBodyes[2/*path walls of wars*/];
    static const string PathLevelTMX;
    static ArrsSprites_t  RedWalls;

    static Sprite *bomba;
    static Sprite *player;
    static Sprite *gruz;
    static Sprite *lockGruz;
    static Sprite *unlockGruz;
    static Sprite *releaseGruz;
    static PhysicsJointLimit *limit;

    static ParticleFire *reactive;
    static ParticleFire *fire;
    static TMXTiledMap  *tiled;
    static Node         *miniMap;


    //inline string getPrefix() { return string("level_") + toString(indexLevel+1) + "/"; }
    inline string getLevelPath() { return "Levels/level_" + StringUtils::toString(indexLevel) + ".tmx"; }
    inline string getLevelBody() { return "Levels/body_wall_" + StringUtils::toString(indexLevel) + ".json"; }

    Level(const int          &_indexLevel,
          const Vec2         &_absoluteCoordinateFire,
          const Vec2         &_absoluteCoordinateGruz,
          const RedWalls_t   &_absoluteCoordinateRedWalls,
          const Vec2         &_absoluteCooridnatePlayer,
          const Vec2         &_absoluteCoordinateBomba)
        :
          indexLevel                 (_indexLevel),
          absoluteCoordinateFire     (_absoluteCoordinateFire),
          absoluteCoordinateGruz     (_absoluteCoordinateGruz),
          absoluteCoordinateRedWalls (_absoluteCoordinateRedWalls),
          absoluteCoordinatePlayer   (_absoluteCooridnatePlayer),
          absoluteCoordinateBomba    (_absoluteCoordinateBomba)
    {}

    void init(Layer *layer);

}
/* Init Levels */
static level[] = {
    Level(0,              // index level
          Vec2(.25, .10), // absolute coord fire
          Vec2(.80, .10), // absolute cooed gruz
          Level::RedWalls_t{/*Level::RedWall_t(Vec2(.50, .50), 0.0f),
                            Level::RedWall_t(Vec2(.50, .60), 20.0f),
                            Level::RedWall_t(Vec2(.50, .80), -5.0f)*/},
          Vec2(.10, .30),  // absolute position player
          Vec2(.80, .20)), // absolute position bomba

    Level(1,
          Vec2(.95, .95),
          Vec2(.50, .10),
          Level::RedWalls_t{},
          Vec2(.10, .20),
          Vec2::ZERO),

    Level(2,
          Vec2(.95, .70),
          Vec2(.10, .65),
          Level::RedWalls_t{Level::_RedWall_t(Vec2(.25, .60), 0.f, true)},
          Vec2(.10, .15),
          Vec2::ZERO),

    Level(3,
          Vec2(.80, .10),
          Vec2(.05, .50),
          Level::RedWalls_t{Level::_RedWall_t(Vec2(.18, .95),   0.f, true),
                            Level::_RedWall_t(Vec2(.35, .80),  40.f, true),
                            Level::_RedWall_t(Vec2(.862, .80),-15.f, true)},
          Vec2(.05, .10),
          Vec2::ZERO),

    Level(4,
          Vec2(.72, .75),
          Vec2(.10, .90),
          Level::RedWalls_t{Level::_RedWall_t(Vec2(.15, .70),   0.f, true),
                            Level::_RedWall_t(Vec2(.395,.65),   0.f, true),
                            Level::_RedWall_t(Vec2(.775, .415), 0.f, false)},
          Vec2(.12, .20),
          Vec2::ZERO),

    Level(5,
          Vec2(.97, .60),
          Vec2(.26, .40),
          Level::RedWalls_t{Level::_RedWall_t(Vec2(.65, .35),  0.f, true),
                            Level::_RedWall_t(Vec2(.70, .55),-9.3f, false),
                            Level::_RedWall_t(Vec2(.37, .58), 15.f, true),
                            Level::_RedWall_t(Vec2(.45, .75), 45.f, true),
                            Level::_RedWall_t(Vec2(.13, .65), 45.f, true)},
          Vec2(.95, .15),
          Vec2::ZERO),

    Level(6,
          Vec2(.90, .20),
          Vec2(.20, .20),
          Level::RedWalls_t{},
          Vec2(.10, .20),
          Vec2::ZERO),

    Level(7,
          Vec2(.90, .20),
          Vec2(.20, .20),
          Level::RedWalls_t{},
          Vec2(.10, .20),
          Vec2::ZERO),

};

NS_CC_END

#endif // __LEVEL_H__




