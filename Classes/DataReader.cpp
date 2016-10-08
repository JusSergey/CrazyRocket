#include "DataReader.h"
#include <fstream>

USING_NS_CC;

const string DataReader::FileName = "CrazyRocket.ini";
DataReader::GameInfo DataReader::GameProgressInformation;

DataReader *DataReader::getInstanse()
{
    static DataReader* pReader(DataReader::create());
    return pReader;
}

void DataReader::FirstInitGame()
{
    static bool isAddSearchPath(false);

    if(!isAddSearchPath){

        FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->sharedFileUtils()->getWritablePath());
        isAddSearchPath = true;

        if(!FileUtils::getInstance()->sharedFileUtils()->isFileExist(FileName)){
            FileUtils::getInstance()->sharedFileUtils()->writeStringToFile("0", FileUtils::getInstance()->sharedFileUtils()->getWritablePath() + FileName);
        }
    }
}

DataReader *DataReader::create()
{
    return (new DataReader);
}

int DataReader::getLevelComplete()
{
    string fullpath = FileUtils::getInstance()->sharedFileUtils()->getWritablePath();

    std::ifstream file;
    file.open(fullpath + FileName);
    int level;
    file >> level;
    file.close();
    return level;
}

int DataReader::getCurrentLevel()
{
    string fullpath = FileUtils::getInstance()->sharedFileUtils()->getWritablePath();

    std::ifstream file;
    file.open(fullpath + FileName);
    int level;
    file >> level;
    file >> level;
    file.close();
    return level;
}

void DataReader::saveLevelComplete(int countLevelComplete)
{
    string fullpath = FileUtils::getInstance()->sharedFileUtils()->getWritablePath();

    std::ofstream file;
    file.open(fullpath + FileName);
    file << countLevelComplete;
    file.flush();
    file.close();
}

void DataReader::setLevelComplete(int level, bool flushToSDCard)
{
    GameProgressInformation.levelComplete = level;

    if(flushToSDCard)
        save();
}

void DataReader::setCurrentLevel(int level, bool flushToSDCard)
{
    GameProgressInformation.currentLevel = level;

    if(flushToSDCard)
        save();
}

void DataReader::save()
{
    string fullpath = FileUtils::getInstance()->sharedFileUtils()->getWritablePath();

    std::ofstream file;
    file.open(fullpath + FileName);
    file << GameProgressInformation.levelComplete << '\n'
         << GameProgressInformation.currentLevel;
    file.flush();
    file.close();
}
