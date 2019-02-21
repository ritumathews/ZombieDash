#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld(); 
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    std::vector <Actor*> getActorPtr() {return actorsVector;}

private:
    Penelope* m_penelope;
    Wall* m_wall; 
    std::vector <Actor*> actorsVector;
};

#endif // STUDENTWORLD_H_
