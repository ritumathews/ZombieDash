
#include "Actor.h"
#include <vector>
#include <cmath>
Actor::Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth):GraphObject(imageID, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, dir, depth)
{
    m_studentWorld = w;
    m_dead = false;
}

bool Actor::isDead() const
{
    return m_dead;
}

void Actor::setDead(){
    m_dead = true;
}

StudentWorld* Actor::world() const
{
    return m_studentWorld;
}

void Actor::activateIfAppropriate(Actor *a)
{
    
}

void Actor::useExitIfAppropriate()
{

}

void Actor::dieByFallOrBurnIfAppropriate()
{
    setDead();
}

void Actor::beVomitedOnIfAppropriate()
{
    
}

void Actor::pickUpGoodieIfAppropriate(Goodie* g)
{
    
}

bool Actor::blocksMovement() const
{
    return false;
}

bool Actor::blocksFlame() const
{
    return false;
}

bool Actor::triggersOnlyActiveLandmines() const
{
    return true;
}

bool Actor::triggersZombieVomit() const
{
    return true;
}

bool Actor::threatensCitizens() const
{
    return false;
}

bool Actor::triggersCitizens() const
{
    return false;
}


Wall::Wall(StudentWorld* w, double x, double y):Actor(w, IID_WALL, x, y, right, 0)
{
}
void Wall::doSomething()
{
}
bool Wall::blocksMovement() const
{
    return true;
}
bool Wall::blocksFlame() const
{
    return true;
}


ActivatingObject::ActivatingObject(StudentWorld* w, int imageID, double x, double y, int dir, int depth):Actor(w, imageID, x, y, dir, depth)
{
}

Exit::Exit(StudentWorld* w, double x, double y):ActivatingObject(w, IID_EXIT, x, y, right, 1)
{
}
void Exit::doSomething()
{
    activateIfAppropriate(world()->getPlayer());
    world()->activateOnAppropriateActors(this);
}
void Exit::activateIfAppropriate(Actor* a)
{
    //if bounding boxes overlap, use exit if appropriate
    a->useExitIfAppropriate();
}
bool Exit::blocksFlame() const
{
    return true;
}

Pit::Pit(StudentWorld* w, double x, double y):ActivatingObject(w, IID_PIT, x, y, right, 0)
{
}
void Pit::doSomething()
{
    activateIfAppropriate(world()->getPlayer());
    world()->activateOnAppropriateActors(this);
}
void Pit::activateIfAppropriate(Actor* a)
{
    a->dieByFallOrBurnIfAppropriate();
}

Flame::Flame(StudentWorld* w, double x, double y, int dir):ActivatingObject(w, IID_FLAME, x, y, dir, 0)
{
    
}
void Flame::doSomething()
{
    
}
void Flame::activateIfAppropriate(Actor* a)
{
    
}

Vomit::Vomit(StudentWorld* w, double x, double y, int dir):ActivatingObject(w, IID_VOMIT, x, y, dir, 0)
{
    
}
void Vomit::doSomething()
{
    
}
void Vomit::activateIfAppropriate(Actor* a)
{
    
}

Landmine::Landmine(StudentWorld* w, double x, double y):ActivatingObject(w, IID_LANDMINE, x, y, right, 1)
{
    
}
void Landmine::doSomething()
{
    
}
void Landmine::activateIfAppropriate(Actor* a)
{
    
}
void Landmine::dieByFallOrBurnIfAppropriate()
{
    
}

Goodie::Goodie(StudentWorld* w, int imageID, double x, double y):ActivatingObject(w, imageID, x, y, right, 1)
{
    
}
void Goodie::activateIfAppropriate(Actor* a)
{
    
}
void Goodie::dieByFallOrBurnIfAppropriate()
{
    
}
bool Goodie::blocksMovement() const
{
    return false; 
}

VaccineGoodie::VaccineGoodie(StudentWorld* w, double x, double y):Goodie(w, IID_VACCINE_GOODIE, x, y)
{
    
}
void VaccineGoodie::doSomething()
{
    
}
void VaccineGoodie::pickUp(Penelope* p)
{
    
}

GasCanGoodie::GasCanGoodie(StudentWorld* w, double x, double y):Goodie(w, IID_GAS_CAN_GOODIE, x, y)
{
    
}
void GasCanGoodie::doSomething()
{
    
}
void GasCanGoodie::pickUp(Penelope* p)
{
    
}

LandmineGoodie::LandmineGoodie(StudentWorld* w, double x, double y):Goodie(w, IID_LANDMINE_GOODIE, x, y)
{
    
}

void LandmineGoodie::doSomething()
{
}

void LandmineGoodie::pickUp(Penelope* p)
{
}

Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir):Actor(w, imageID, x, y, dir, 0)
{
    
}
bool Agent::blocksMovement() const
{
    return true;
}
bool Agent::triggersOnlyActiveLandmines() const
{
    return true;
}

void Agent::getCloser(Actor* a, Actor* target, int& dirX, int& dirY)
{
    if(target->getX() < a->getX()){
        dirX = right;
    }else{
        dirX = left;
    }
    if(target->getY() < a->getY()){
        dirY = down;
    }else{
        dirY = up;
    }
}

Human::Human(StudentWorld* w, int imageID, double x, double y):Agent(w, imageID, x, y, right)
{
    m_infectionLevel = 0;
}
void Human::beVomitedOnIfAppropriate()
{
    
}
bool Human::triggersZombieVomit() const
{
    return false; //INCORRECT BUT WILL COMPILE
}

// Make this human uninfected by vomit.
void Human::clearInfection()
{
    m_infectionLevel = 0;
}

// How many ticks since this human was infected by vomit?
int Human::getInfectionDuration() const
{
    return m_infectionLevel;
}

void Human::move(int dir, int steps, int x, int y)
{
    int dest_x, dest_y;
    switch(dir){
        case left:
            dest_x = x - steps;
            dest_y = y;
            if(!(world()->isAgentMovementBlockedAt(dest_x, dest_y))){
                setDirection(left);
                moveTo(dest_x, dest_y);
            }
            break;
        case right:
            dest_x = x + steps;
            dest_y = y;
            if(!(world()->isAgentMovementBlockedAt(dest_x+SPRITE_WIDTH-1, dest_y))){
                setDirection(right);
                moveTo(dest_x, dest_y);
            }
            break;
        case up:
            dest_x = x;
            dest_y = y + steps;
            if(!(world()->isAgentMovementBlockedAt(dest_x+SPRITE_WIDTH-1, dest_y+SPRITE_HEIGHT-1))){
                setDirection(up);
                moveTo(dest_x, dest_y);
            }
            break;
        case down:
            dest_x = x;
            dest_y = y - steps;
            if(!(world()->isAgentMovementBlockedAt(dest_x, dest_y))){
                setDirection(down);
                moveTo(dest_x, dest_y);
            }
            break;
    }
}

Penelope::Penelope(StudentWorld* w, double x, double y):Human(w, IID_PLAYER, x, y)
{
    m_vaccines = 0;
    m_landmines = 0;
    m_flameCharges = 0;
}
void Penelope::doSomething()
{
    if(isDead()){
        return; 
    }
    int ch;
    if(world()->getKey(ch)){
        //user hit a key during this tick
        switch(ch){
            case KEY_PRESS_LEFT:
                //move Penelope left
                move(left, 4, getX(), getY());
                break;
            case KEY_PRESS_RIGHT:
                //move Penelope right
                move(right, 4, getX(), getY());
                break;
            case KEY_PRESS_UP:
                //move Penelope up
                move(up, 4, getX(), getY());
                break;
            case KEY_PRESS_DOWN:
                //move Penelope down
                move(down, 4, getX(), getY());
                break;
        }
    }
}
void Penelope::useExitIfAppropriate()
{
    world()->recordLevelFinishedIfAllCitizensGone(); 
}
void Penelope::dieByFallOrBurnIfAppropriate()
{
    setDead();
    world()->playSound(SOUND_PLAYER_DIE);
    
}
void Penelope::pickUpGoodieIfAppropriate(Goodie* g)
{
    
}

// Increase the number of vaccines the object has.
void Penelope::increaseVaccines()
{
    m_vaccines++;
}

// Increase the number of flame charges the object has.
void Penelope::increaseFlameCharges()
{
    m_flameCharges++;
}

// Increase the number of landmines the object has.
void Penelope::increaseLandmines()
{
    m_landmines++;
}

// How many vaccines does the object have?
int Penelope::getNumVaccines() const
{
    return m_vaccines;
}

// How many flame charges does the object have?
int Penelope::getNumFlameCharges() const
{
    return m_flameCharges;
}

// How many landmines does the object have?
int Penelope::getNumLandmines() const
{
    return m_landmines;
}

bool Penelope::triggersCitizens() const
{
    return true; 
}

Citizen::Citizen(StudentWorld* w,  double x, double y):Human(w, IID_CITIZEN, x, y)
{
    m_paralysis = false;
}
double Citizen::dist_p()
{
    return world()->calcDistance(this, world()->getPlayer());
}
void Citizen::becomeParalyzed()
{
    if(isParalyzed()){
        m_paralysis = false;
    }else{
        m_paralysis = true;
    }
}
void Citizen::doSomething()
{
    if(isDead()){
        return;
    }else{
        if(getInfectionDuration()>0){
            increaseInfection();
        }
        if(getInfectionDuration()>= 500){
            setDead();
            world()->playSound(SOUND_ZOMBIE_BORN);
            world()->increaseScore(-1000);
            int prob = randInt(0, 9);
            if(prob < 7){
                world()->addActor(new DumbZombie(world(), getX(), getY()));
            }else{
                world()->addActor(new SmartZombie(world(), getX(), getY()));
            }
            return;
        }
        if(!isParalyzed()){
            double triggerX, triggerY, triggerDistance;
            bool isThreat = false;
            if(world()->locateNearestCitizenTrigger(this, getX(), getY(), triggerX, triggerY, triggerDistance, isThreat)){
                if(isThreat){
                    //Fill with code of what to do if zombie is closer than Penelope
                    
                    
                }else{
                    //if citizen is in same row or column, move two pixels towards Penelope, if able
                    if(getX()/16 == world()->getPlayer()->getX()/16){ //Same row
                        
                        if(getY() < world()->getPlayer()->getY()){ // find out if above or below Penelope
                            //MOVE UP
                            move(up, 2, getX(), getY());
                        }else{
                            //MOVE DOWN
                            move(down, 2, getX(), getY());
                        }
                    }else if(getY()/16 == world()->getPlayer()->getY()/16){ //Same column
                        if(getX() < world()->getPlayer()->getX()){ // find out if left or right of Penelope
                            //MOVE RIGHT
                            move(right, 2, getX(), getY());
                        }else{
                            //MOVE LEFT
                            move(left, 2, getX(), getY());
                        }
                    }else {
                        //determine which direction vertical and horizontal would get citizen closer to Pen
                        //chose vert or horizontal at random
                        int dirX, dirY;
                        getCloser(this, world()->getPlayer(), dirX, dirY);
                        int prob = randInt(0, 1);
                        if(prob == 0){
                            move(dirX, 2, getX(), getY());
                        }else{
                            move(dirY, 2, getX(), getY());
                        }
                    }
                    
                }
            }
        }else{
            becomeParalyzed();
            return;
        }
        becomeParalyzed();
    }
    
}
void Citizen::useExitIfAppropriate()
{
    world()->increaseScore(500);
    setDead();
    world()->playSound(SOUND_CITIZEN_SAVED);
    world()->recordCitizenGone(); 
}
void Citizen::dieByFallOrBurnIfAppropriate()
{
    setDead();
    world()->playSound(SOUND_CITIZEN_DIE);
    world()->increaseScore(-1000);
}

Zombie::Zombie(StudentWorld* w, double x, double y):Agent(w, IID_ZOMBIE, x, y, right)
{
    
}

bool Zombie::triggersCitizens() const
{
    return true;
}

bool Zombie::threatensCitizens() const
{
    return true;
}

DumbZombie::DumbZombie(StudentWorld* w,  double x, double y):Zombie(w, x, y)
{
    
}
void DumbZombie::doSomething()
{
    
}
void DumbZombie::dieByFallOrBurnIfAppropriate()
{
    setDead(); 
}

SmartZombie::SmartZombie(StudentWorld* w,  double x, double y):Zombie(w, x, y)
{
    
}
void SmartZombie::doSomething()
{
    
}
void SmartZombie::dieByFallOrBurnIfAppropriate()
{
    
}

