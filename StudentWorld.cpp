#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include "Actor.h"
#include "Level.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::~StudentWorld()
{
    cleanUp(); 
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    // Find Penelope's location and create her there
    Level curLevel(assetPath());
    int levelNum = getLevel();
    std::stringstream levelNumStr;
    levelNumStr << levelNum;
    std::string levelFile = "level0" + levelNumStr.str() + ".txt";
    int player_x = 0;
    int player_y = 0;
    if(curLevel.loadLevel(levelFile) == Level::load_success){
        for(int i = 0; i < LEVEL_WIDTH; i++){
            for(int j = 0; j < LEVEL_HEIGHT; j++){
                if(curLevel.getContentsOf(i, j) == Level::player){
                    player_x = i;
                    player_y = j;
                    break;
                }
            }
        }
    }
    m_penelope = new Penelope(this, player_x, player_y);
    
    // Create all the Walls
    int wall_x = 0;
    int wall_y = 0; 
    if(curLevel.loadLevel(levelFile) == Level::load_success){
        for(int i = 0; i < LEVEL_WIDTH; i++){
            for(int j = 0; j < LEVEL_HEIGHT; j++){
                if(curLevel.getContentsOf(i, j) == Level::wall){
                    wall_x = i;
                    wall_y = j;
                    actorsVector.push_back(new Wall(this, wall_x, wall_y));
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_penelope->doSomething();
    vector<Actor*> :: iterator it;
    it = actorsVector.begin();
    while(it != actorsVector.end()){
        (*it)->doSomething();
        it++;
    }
    decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while(!actorsVector.empty()){
        Actor* p = actorsVector.back();
        delete p;
        actorsVector.pop_back();
    }
    delete m_penelope;
}
