#include "Actor.h"
#include "StudentWorld.h"
#include <vector>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* swp, int imageID, int startX, int startY, Direction startDirection, int depth):GraphObject(imageID, startX, startY, startDirection, depth)
{
    m_studentWorld = swp;
    m_direction = startDirection;
    m_upperX = startX + SPRITE_WIDTH - 1;
    m_upperY = startX + SPRITE_HEIGHT - 1;
}

void Actor:: setDirection(Direction newDirection)
{
    m_direction = newDirection; 
}

bool Actor:: checkBoundingBox(int dest_x, int dest_y)
{
    std::vector<Actor*> :: iterator it;
    it = getWorld()->getActorPtr().begin();
    while(it != getWorld()->getActorPtr().end() && !(getWorld()->getActorPtr().empty())){
        if((dest_x <= (*it)->getX()+SPRITE_WIDTH-1 && dest_x >= (*it)->getX()) ||
           (dest_y <= (*it)->getY()+SPRITE_WIDTH-1 && dest_y >= (*it)->getY()))
            return false;
        it++;
    }
    return true;
}

StudentWorld* Actor:: getWorld()
{
    return m_studentWorld;
}


Penelope::Penelope(StudentWorld* swp, int level_x, int level_y):Actor(swp, IID_PLAYER, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, right, 0)
{
}

void Penelope:: doSomething() // NOT DONE NEED TO CHECK IF BOUNDING BOX OVERLAPS
{
    int ch, dest_x, dest_y;
    if(getWorld()->getKey(ch)){
        //user hit a key during this tick
        switch(ch){
            case KEY_PRESS_LEFT:
                //move Penelope left
                setDirection(ch);
                dest_x = getX()-4;
                dest_y = getY();
                moveTo(dest_x, dest_y);
                break;
            case KEY_PRESS_RIGHT:
                //move Penelope right
                setDirection(ch);
                dest_x = getX()+4;
                dest_y = getY();
                std::cerr << getX() << std::endl << dest_x << std::endl; 
                moveTo(dest_x, dest_y);
                break;
            case KEY_PRESS_UP:
                //move Penelope up
                setDirection(ch);
                dest_x = getX();
                dest_y = getY()+4;
                moveTo(dest_x, dest_y);
                break;
            case KEY_PRESS_DOWN:
                //move Penelope down
                setDirection(ch);
                dest_x = getX();
                dest_y = getY()-4;
                moveTo(dest_x, dest_y);
                break;
        }
    }
}

Wall:: Wall(StudentWorld* swp, int level_x, int level_y):Actor(swp, IID_WALL, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, right, 0)
{
}

void Wall:: doSomething()
{
    
}


