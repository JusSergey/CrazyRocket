#ifndef __DATAREADER_H__
#define __DATAREADER_H__

#include "cocos2d.h"
using std::string;

class DataReader
{
public:
    static DataReader *getInstanse();
    struct GameInfo {
        int levelComplete;
        int currentLevel;
    } static GameProgressInformation;

    static void FirstInitGame();
private:
    static DataReader *create();

private:
    static const string FileName;

public:
    static int  getLevelComplete();
    static int  getCurrentLevel();
    static void saveLevelComplete(int countLevelComplete);
    static void setLevelComplete(int level, bool flushToSDCard = false);
    static void setCurrentLevel(int level, bool flushToSDCard = false);
    static void save();
};

#endif // __DATAREADER_H__
