

#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include "Actor.h"
#include "Level.h"
#include <vector>
using namespace std;

GameWorld* createStudentWorld(std::string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(std::string assetDir):GameWorld(assetDir)
{
    
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    m_levelFinished = false;
    m_nCitizens = 0; 
    Level curLevel(assetPath());
    /*
    int levelNum = getLevel();
    std::stringstream levelNumStr;
    levelNumStr << levelNum;
    std::string levelFile = "level0" + levelNumStr.str() + ".txt";
     */
    std::string levelFile = "level04.txt";
    //std::cerr << "level0" + levelNumStr.str() + ".txt" << std:: endl;
    if(curLevel.loadLevel(levelFile) == Level::load_success){
        for(int i = 0; i < LEVEL_WIDTH; i++){
            for(int j = 0; j < LEVEL_HEIGHT; j++){
                Level::MazeEntry ch = curLevel.getContentsOf(i, j);
                //CREATE PLAYER
                if(ch == Level::player){
                    m_penelope = new Penelope(this, i, j);
                }
                //CREATE WALLS
                if(ch == Level::wall){
                    addActor(new Wall(this, i, j));
                }
                //CREATE EXIT
                if(ch == Level::exit){
                    addActor(new Exit(this, i, j));
                }
                //CREATE PITS
                if(ch == Level::pit){
                    addActor(new Pit(this, i, j));
                }
                //CREATE CITIZENS
                if(ch == Level::citizen){
                    addActor(new Citizen(this, i, j));
                    m_nCitizens++;
                }
                //CREATE DUMB ZOMBIES
                if(ch == Level::dumb_zombie){
                    addActor(new DumbZombie(this, i, j));
                }
                //CREATE SMART ZOMBIES
                if(ch == Level::smart_zombie){
                    addActor(new SmartZombie(this, i, j));
                }
                //CREATE GOODIES
                if(ch == Level::vaccine_goodie){
                    addActor(new VaccineGoodie(this, i, j));
                }
                if(ch == Level::gas_can_goodie){
                    addActor(new GasCanGoodie(this, i, j));
                }
                if(ch == Level::landmine_goodie){
                    addActor(new LandmineGoodie(this, i, j));
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDead()
{
    for(int i = 0; i != m_actors.size(); i++){
        if(m_actors[i]->isDead()){
            delete m_actors[i];
            m_actors.erase(m_actors.begin()+i);
            i--;
        }
    }
}

int StudentWorld::move()
{
    if(m_levelFinished){
        return GWSTATUS_FINISHED_LEVEL; 
    }
    //if penelope is dead, dec lives, if penelope has no lives, game over
    if(m_penelope->isDead()){
        //decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    m_penelope->doSomething();
    vector<Actor*> :: iterator it;
    it = m_actors.begin();
    
    removeDead();
    
    while(it != m_actors.end()){
        (*it)->doSomething();
        it++;
    }
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
    while(!m_actors.empty()){
        Actor* p = m_actors.back();
        delete p;
        m_actors.pop_back();
    }
    delete m_penelope;
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

// Record that one more citizen on the current level is gone (exited,
// died, or turned into a zombie).
void StudentWorld::recordCitizenGone()
{
    m_nCitizens--;
}

// Indicate that the player has finished the level if all citizens
// are gone.
void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
    if(m_nCitizens == 0){
        m_levelFinished = true;
    }
}

// For each actor overlapping a, activate a if appropriate.
void StudentWorld::activateOnAppropriateActors(Actor* a)
{
    vector<Actor*> :: iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if(overlap((*it), a)){
            a->activateIfAppropriate((*it));
        }
        it++;
    }
}

// Is an agent blocked from moving to the indicated location?
bool StudentWorld::isAgentMovementBlockedAt(double x, double y)
{
    vector<Actor*> :: iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
    
        if(((*it)->getX() <= x && x <= (*it)->getX() + SPRITE_WIDTH - 1) &&
           ((*it)->getY() <= y && y <= (*it)->getY() + SPRITE_HEIGHT - 1) && (*it)->blocksMovement()){
            return true;
        }
        it++;
    }
    return false;
}

// Is creation of a flame blocked at the indicated location?
bool StudentWorld::isFlameBlockedAt(double x, double y) const
{
    return false;
}

// Is there something at the indicated location that might cause a
// zombie to vomit (i.e., a human)?
bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const
{
    return true;
}

// Return true if there is a living human, otherwise false.  If true,
// otherX, otherY, and distance will be set to the location and distance
// of the human nearest to (x,y).
bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance)
{
    return true;
}

// Return true if there is a living zombie or Penelope, otherwise false.
// If true, otherX, otherY, and distance will be set to the location and
// distance of the one nearest to (x,y), and isThreat will be set to true
// if it's a zombie, false if a Penelope.
bool StudentWorld::locateNearestCitizenTrigger(Citizen* a, double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const
{
    vector<Actor*> :: const_iterator it;
    it = m_actors.begin();
    double distanceToPlayer = calcDistance(a, getPlayer());
    double closestZombie = sqrt(256*256 + 256*256);
    while(it != m_actors.end()){
        if((*it)->triggersCitizens() && (*it)->threatensCitizens()){
            double distanceToZombie = calcDistance(a, (*it));
            if (distanceToZombie <= closestZombie){
                closestZombie = distanceToZombie;
                otherX = (*it)->getX();
                otherY = (*it)->getY();
                distance = distanceToZombie;
            }
        }
        it++;
    }
    if(distanceToPlayer < closestZombie){
        otherX = getPlayer()->getX();
        otherY = getPlayer()->getY();
        distance = distanceToPlayer;
    }else{
        isThreat = true;
    }
    isThreat = false;
    if(distance <= 80){
        return true;
    }else{
        return false;
    }
    
}

// Return true if there is a living zombie, false otherwise.  If true,
// otherX, otherY and distance will be set to the location and distance
// of the one nearest to (x,y).
bool StudentWorld::locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const
{
    return true;
}

double StudentWorld::calcDistance(Actor* a, Actor* b) const
{
    double aX = ((a->getX() + SPRITE_WIDTH - 1)/2);
    double aY = ((a->getY() + SPRITE_HEIGHT - 1)/2);
    double bX = ((b->getX() + SPRITE_WIDTH -1)/2);
    double bY = ((b->getY() + SPRITE_HEIGHT - 1)/2);
    return sqrt((bX - aX)*(bX - aX) + (bY - aY)*(bY - aY));
}

bool StudentWorld::overlap(Actor* a, Actor* b) const
{
    if((a->getX() <= b->getX() && b->getX() <= a->getX() + SPRITE_WIDTH - 1) && (a->getY() <= b->getY() && b->getY() <= a->getY() + SPRITE_HEIGHT - 1)){
        return true;
    }
    return false;
}
