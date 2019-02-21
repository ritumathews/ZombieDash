#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* swp, int imageID, int level_x, int level_y, Direction startDirection, int depth);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    void setDirection(Direction newDirection);
    bool checkBoundingBox(int dest_x, int dest_y);
private:
    StudentWorld* m_studentWorld;
    Direction m_direction;
    int m_upperX;
    int m_upperY; 
    
};

class Wall: public Actor
{
public:
    Wall(StudentWorld* swp, int level_x, int level_y);
    virtual void doSomething(); 
private:
};

class Penelope: public Actor
{
public:
    Penelope(StudentWorld* swp, int level_x, int level_y);
    virtual void doSomething(); //I THOUGHT INHERITANCE WAS SUPPOSED TO LET ME LEAVE THIS OUT???
private:
};

#endif // ACTOR_H_
