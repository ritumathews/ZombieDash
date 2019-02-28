
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
    return false;
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

bool Actor:: launchFlame(int x, int y, int dir)
{
    if(!(world()->isFlameBlockedAt(x, y))){
        Actor* a = new Flame(world(), x, y, dir);
        world()->addActor(a);
        a->moveTo(x, y);
        return true;
    }
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
    m_ticks = 0;
}

Exit::Exit(StudentWorld* w, double x, double y):ActivatingObject(w, IID_EXIT, x, y, right, 1)
{
}
void Exit::doSomething()
{
    if(world()->overlap(world()->getPlayer(), this)){
        activateIfAppropriate(world()->getPlayer());
    }
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
    if(world()->overlap(world()->getPlayer(), this)){
        activateIfAppropriate(world()->getPlayer());
    }
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
    if(isDead()){
        return;
    }
    countTick();
    if(getTicks() == 2){
        setDead();
        return; 
    }
    if(world()->overlap(world()->getPlayer(), this)){
        activateIfAppropriate(world()->getPlayer());
    }
    world()->activateOnAppropriateActors(this);
}
void Flame::activateIfAppropriate(Actor* a)
{
    a->dieByFallOrBurnIfAppropriate();
}

Vomit::Vomit(StudentWorld* w, double x, double y, int dir):ActivatingObject(w, IID_VOMIT, x, y, dir, 0)
{
    
}
void Vomit::doSomething()
{
    if(isDead()){
        return;
    }
    countTick();
    if(getTicks() == 2){
        setDead();
        return;
    }
    if(world()->overlap(world()->getPlayer(), this)){
        activateIfAppropriate(world()->getPlayer());
    }
    world()->activateOnAppropriateActors(this);
}
void Vomit::activateIfAppropriate(Actor* a)
{
    a->beVomitedOnIfAppropriate(); 
}

Landmine::Landmine(StudentWorld* w, double x, double y):ActivatingObject(w, IID_LANDMINE, x, y, right, 1)
{
    m_safetyTicks = 30;
    m_active = false;
}
void Landmine::doSomething()
{
    
    if(isDead()){
        return;
    }
    if(!m_active){
        m_safetyTicks--;
        if(m_safetyTicks == 0){
            m_active = true;
            return;
        }
    }else{
        blowUp(); 
    
    if(world()->overlap(world()->getPlayer(), this)){
        activateIfAppropriate(world()->getPlayer());
    }
        world()->activateOnAppropriateActors(this);
}
}
void Landmine::activateIfAppropriate(Actor* a)
{
    if(a->triggersOnlyActiveLandmines()){
        blowUp(); 
    }
}

void Landmine::blowUp()
{
    setDead();
    world()->playSound(SOUND_LANDMINE_EXPLODE);
    int dir = up;
    launchFlame(getX(), getY(), dir);
    launchFlame(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT, dir);
    launchFlame(getX()-SPRITE_WIDTH, getY(), dir);
    launchFlame(getX(), getY()+SPRITE_HEIGHT, dir);
    launchFlame(getX(), getY()-SPRITE_HEIGHT, dir);
    launchFlame(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT, dir);
    launchFlame(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT, dir);
    launchFlame(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT, dir);
    launchFlame(getX()+SPRITE_WIDTH, getY(), dir);
    
    Actor* b = new Pit(world(), getX(), getY());
    world()->addActor(b);
    b->moveTo(getX(), getY());
    
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
    m_active = true;
    activateIfAppropriate(this);
}

Goodie::Goodie(StudentWorld* w, int imageID, double x, double y):ActivatingObject(w, imageID, x, y, right, 1)
{
    
}
void Goodie::activateIfAppropriate(Actor* a)
{
    if(isDead()){
        return;
    }
    if(world()->overlap(a, this)){
        world()->increaseScore(50);
        setDead();
        world()->playSound(SOUND_GOT_GOODIE);
        pickUp(world()->getPlayer());
    }
}
void Goodie::dieByFallOrBurnIfAppropriate()
{
    setDead();
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
    activateIfAppropriate(world()->getPlayer());
}
void VaccineGoodie::pickUp(Penelope* p)
{
    p->increaseVaccines();
}

GasCanGoodie::GasCanGoodie(StudentWorld* w, double x, double y):Goodie(w, IID_GAS_CAN_GOODIE, x, y)
{
    
}
void GasCanGoodie::doSomething()
{
    activateIfAppropriate(world()->getPlayer());
}
void GasCanGoodie::pickUp(Penelope* p)
{
    p->increaseFlameCharges();
}

LandmineGoodie::LandmineGoodie(StudentWorld* w, double x, double y):Goodie(w, IID_LANDMINE_GOODIE, x, y)
{
    
}

void LandmineGoodie::doSomething()
{
    activateIfAppropriate(world()->getPlayer());
}

void LandmineGoodie::pickUp(Penelope* p)
{
    p->increaseLandmines();
}

Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir):Actor(w, imageID, x, y, dir, 0)
{
    m_paralysis = false;
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
        dirX = left;
    }else{
        dirX = right;
    }
    if(target->getY() < a->getY()){
        dirY = down;
    }else{
        dirY = up;
    }
}

void Agent::becomeParalyzed()
{
    if(isParalyzed()){
        m_paralysis = false;
    }else{
        m_paralysis = true;
    }
}


Human::Human(StudentWorld* w, int imageID, double x, double y):Agent(w, imageID, x, y, right)
{
    m_infectionLevel = 0;
}
void Human::beVomitedOnIfAppropriate()
{
    if(getInfectionDuration() == 0){
        world()->playSound(SOUND_CITIZEN_INFECTED);
    }
    increaseInfection();
    
}
bool Human::triggersZombieVomit() const
{
    return true; //INCORRECT BUT WILL COMPILE
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

bool Agent::canMove(int dir, int steps, int x, int y, int& dest_x, int& dest_y)
{
    
    switch(dir){
        case left:
            dest_x = x - steps;
            dest_y = y;
            if(!(world()->isAgentMovementBlockedAt(dest_x, dest_y)) &&
               !(world()->isAgentMovementBlockedAt(dest_x, dest_y+SPRITE_HEIGHT-1))){
                return true;
            }
            break;
        case right:
            dest_x = x + steps;
            dest_y = y;
            if(!(world()->isAgentMovementBlockedAt(dest_x+SPRITE_WIDTH-1, dest_y)) &&
               !(world()->isAgentMovementBlockedAt(dest_x+SPRITE_WIDTH-1, dest_y+SPRITE_HEIGHT-1))){
                return true;
            }
            break;
        case up:
            dest_x = x;
            dest_y = y + steps;
            if(!(world()->isAgentMovementBlockedAt(dest_x+SPRITE_WIDTH-1, dest_y+SPRITE_HEIGHT-1)) &&
               !(world()->isAgentMovementBlockedAt(dest_x, dest_y+SPRITE_HEIGHT-1))){
                return true;
            }
            break;
        case down:
            dest_x = x;
            dest_y = y - steps;
            if(!(world()->isAgentMovementBlockedAt(dest_x + SPRITE_WIDTH-1, dest_y)) &&
               !(world()->isAgentMovementBlockedAt(dest_x, dest_y))){
                return true;
            }
            break;
    }
    return false;
}

void Agent::move(int dir, int steps, int x, int y)
{
    int dest_x = 0;
    int dest_y = 0;
    if(canMove(dir, steps, x, y, dest_x, dest_y)){
        setDirection(dir);
        moveTo(dest_x, dest_y);
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
    if(getInfectionDuration() > 0){
        increaseInfection();
    }
    if(getInfectionDuration() == 500){
        setDead();
        world()->playSound(SOUND_PLAYER_DIE);
        world()->addActor(new DumbZombie(world(), getX(), getY()));
        world()->playSound(SOUND_ZOMBIE_BORN);
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
            case KEY_PRESS_TAB:
                if(getNumLandmines() < 1){
                    return;
                }else{
                    int x = getX();
                    int y = getY();
                    Actor* b = new Landmine(world(), x, y);
                    world()->addActor(b);
                    b->moveTo(x, y);
                    m_landmines--;
                }
                break;
            case KEY_PRESS_ENTER:
                if(getNumVaccines() < 1){
                    return;
                }
                clearInfection();
                m_vaccines--;
                break;
            case KEY_PRESS_SPACE:
                // attempt to fire flame thrower
                if(getNumFlameCharges() < 1){
                    return;
                }
                int dir = getDirection();
                switch(dir){
                    case up:
                        for(int i = 1; i < 4; i++){
                            if(!launchFlame(getX(), getY() + i * SPRITE_HEIGHT, dir)){
                                break;
                            }
                        }
                        break;
                    case down:
                        for(int i = 1; i < 4; i++){
                            if(!launchFlame(getX(), getY() - i * SPRITE_HEIGHT, dir)){
                                break;
                            }
                        }
                        break;
                    case left:
                        for(int i = 1; i < 4; i++){
                            if(!launchFlame(getX() - i * SPRITE_WIDTH, getY(), dir)){
                                break;
                            }
                        }
                        break;
                    case right:
                        for(int i = 1; i < 4; i++){
                            if(!launchFlame(getX() + i * SPRITE_WIDTH, getY(), dir)){
                                break;
                            }
                        }
                        break;
                }
                m_flameCharges--; 
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
    m_flameCharges+=5;
}

// Increase the number of landmines the object has.
void Penelope::increaseLandmines()
{
    m_landmines+=2;
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
}

void Citizen::getFurther(int xCit, int yCit, int xThreat, int yThreat, int& dirX, int& dirY)
{
    if(xThreat < xCit){
        dirX = right;
    }else{
        dirX = left;
    }
    if(yThreat < yCit){
        dirY = up;
    }else{
        dirY = down;
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
            world()->recordCitizenGone(); 
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
                    if(getX()/16 == triggerX/16){ //Same row
                        
                        if(getY() < triggerY){ // find out if above or below zombie
                            //MOVE DOWN
                            move(down, 2, getX(), getY());
                        }else{
                            //MOVE UP
                            move(up, 2, getX(), getY());
                        }
                    }else if(getY()/16 == triggerY/16){ //Same column
                        if(getX() < triggerX){ // find out if left or right of zombie
                            //MOVE LEFT
                            move(left, 2, getX(), getY());
                        }else{
                            //MOVE RIGHT
                            move(right, 2, getX(), getY());
                        }
                    }else {
                        //determine which direction vertical and horizontal would get citizen further to zombie
                        //chose vert or horizontal at random
                        int dirX, dirY;
                        int dest_x = 0;
                        int dest_y = 0;
                        //getCloser(this, world()->getPlayer(), dirX, dirY);
                        getFurther(getX(), getY(), triggerX, triggerY, dirX, dirY);
                        int prob = randInt(0, 1);
                        if(prob == 0){
                            if(!canMove(dirX, 2, getX(), getY(), dest_x, dest_y)){
                                move(dirY, 2, getX(), getY());
                            }else{
                                move(dirX, 2, getX(), getY());
                            }
                        }else{
                            if(!canMove(dirY, 2, getX(), getY(), dest_x, dest_y)){
                                move(dirX, 2, getX(), getY());
                            }else{
                                move(dirY, 2, getX(), getY());
                            }
                        }
                    }
                    
                }else{
                    //if citizen is in same row or column, move two pixels towards Penelope, if able
                    if(getX()/16 == triggerX/16){ //Same row
                        
                        if(getY() < triggerY){ // find out if above or below Penelope
                            //MOVE UP
                            move(up, 2, getX(), getY());
                        }else{
                            //MOVE DOWN
                            move(down, 2, getX(), getY());
                        }
                    }else if(getY()/16 == triggerY/16){ //Same column
                        if(getX() < triggerX){ // find out if left or right of Penelope
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
                        int dest_x = 0;
                        int dest_y = 0;
                        getCloser(this, world()->getPlayer(), dirX, dirY);
                        int prob = randInt(0, 1);
                        if(prob == 0){
                            if(!canMove(dirX, 2, getX(), getY(), dest_x, dest_y)){
                                move(dirY, 2, getX(), getY());
                            }else{
                                move(dirX, 2, getX(), getY());
                            }
                        }else{
                            if(!canMove(dirY, 2, getX(), getY(), dest_x, dest_y)){
                                move(dirX, 2, getX(), getY());
                            }else{
                                move(dirY, 2, getX(), getY());
                            }
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
    world()->recordCitizenGone(); 
    world()->playSound(SOUND_CITIZEN_DIE);
    world()->increaseScore(-1000);
}

Zombie::Zombie(StudentWorld* w, double x, double y):Agent(w, IID_ZOMBIE, x, y, right)
{
    m_movementPlan = 0;
}

bool Zombie::triggersCitizens() const
{
    return true;
}

bool Zombie::threatensCitizens() const
{
    return true;
}

void Zombie::dieByFallOrBurnIfAppropriate()
{
    setDead();
    world()->playSound(SOUND_ZOMBIE_DIE);
    increaseScore();
}

void Zombie::computeVomit(int& x, int& y)
{
    int dir = getDirection();
    switch(dir)
    {
        case left:
            x = getX() - SPRITE_WIDTH;
            y = getY();
            break;
        case right:
            x = getX() + SPRITE_WIDTH;
            y = getY();
            break;
        case up:
            x = getX();
            y = getY() + SPRITE_HEIGHT;
            break;
        case down:
            x = getX();
            y = getY() - SPRITE_HEIGHT;
            
    }
}

void Zombie::doSomething()
{
    if(isDead()){
        return;
    }
    if(!isParalyzed()){
        double humanX, humanY, distance;
        if(world()->locateNearestVomitTrigger(getX(), getY(), humanX, humanY, distance)){
            int x, y;
            computeVomit(x, y);
            if(world()->isZombieVomitTriggerAt(x, y)){
                int prob = randInt(0, 2);
                if(prob == 0){
                    Actor* a = new Vomit(world(), x, y, getDirection());
                    world()->addActor(a);
                    world()->playSound(SOUND_ZOMBIE_VOMIT);
                    a->moveTo(x, y);
                    becomeParalyzed();
                    return;
                }
            }
        }
        if(m_movementPlan == 0){
            m_movementPlan = randInt(3, 10);
            int dir = randInt(0, 3);
            switch(dir){
                case 0:
                    setDirection(up);
                    break;
                case 1:
                    setDirection(down);
                    break;
                case 2:
                    setDirection(left);
                    break;
                case 3:
                    setDirection(right);
                    break;
            }
        }
        int x, y;
        if(!canMove(getDirection(), 1, getX(), getY(), x, y)){
            m_movementPlan = 0;
            becomeParalyzed();
            return; 
        }
        move(getDirection(), 1, getX(), getY());
        m_movementPlan--;
        
    }
    becomeParalyzed();
    
}

DumbZombie::DumbZombie(StudentWorld* w,  double x, double y):Zombie(w, x, y)
{
    
}

void DumbZombie::increaseScore()
{
    world()->increaseScore(1000);
    int prob = randInt(0, 9);
    if(prob == 6){
        int randDir = randInt(0, 3);
        int x, y;
        switch(randDir){
            case 0:
                x = getX() - SPRITE_WIDTH;
                y = getY();
                break;
            case 1:
                x = getX() + SPRITE_WIDTH;
                y = getY();
                break;
            case 2:
                x = getX();
                y = getY() - SPRITE_HEIGHT;
                break;
            case 3:
                x = getX();
                y = getY() + SPRITE_HEIGHT;
                break;
            default://will never run
                x = getX();
                y = getY();
        }
        //supposed to check if something there...
        
        Actor* a = new VaccineGoodie(world(), x, y);
        world()->addActor(a);
        a->moveTo(getX(), getY());
    }
}

SmartZombie::SmartZombie(StudentWorld* w,  double x, double y):Zombie(w, x, y)
{
    
}

void SmartZombie::increaseScore()
{
    world()->increaseScore(2000);
}

