
/*
class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    bool checkBoundingBox(int dest_x, int dest_y);
    virtual std::string type() = 0;
private:
    StudentWorld* m_studentWorld;
    int m_upperX;
    int m_upperY; 
    
};

class Wall: public Actor
{
public:
    Wall(StudentWorld* swp, int level_x, int level_y);
    virtual void doSomething();
    virtual std::string type(){return "wall";}
private:
};

class Penelope: public Actor
{
public:
    Penelope(StudentWorld* swp, int level_x, int level_y);
    virtual void doSomething(); //I THOUGHT INHERITANCE WAS SUPPOSED TO LET ME LEAVE THIS OUT???
    virtual std::string type(){return "Penelope";}
private:
};

class Exit:public Actor
{
public:
    Exit(StudentWorld* swp, int level_x, int level_y);
    virtual void doSomething();
    virtual std::string type(){return "exit";}
private:
};

#endif // ACTOR_H_
*/

#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"
#include "StudentWorld.h"
#include "Level.h"
class StudentWorld;
class Goodie;
class Penelope;
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);
    
    // Action to perform for each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    bool isDead() const;
    
    // Mark this actor as dead.
    void setDead();
    
    // Get this actor's world
    StudentWorld* world() const;
    
    // If this is an activated object, perform its effect on a (e.g., for an
    // Exit have a use the exit).
    virtual void activateIfAppropriate(Actor* a);
    
    // If this object uses exits, use the exit.
    virtual void useExitIfAppropriate();
    
    // If this object can die by falling into a pit or burning, die.
    virtual void dieByFallOrBurnIfAppropriate();
    
    // If this object can be infected by vomit, get infected.
    virtual void beVomitedOnIfAppropriate();
    
    // If this object can pick up goodies, pick up g
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    // Does this object block agent movement?
    virtual bool blocksMovement() const;
    
    // Does this object block flames?
    virtual bool blocksFlame() const;
    
    // Does this object trigger landmines only when they're active?
    virtual bool triggersOnlyActiveLandmines() const;
    
    // Can this object cause a zombie to vomit?
    virtual bool triggersZombieVomit() const;
    
    // Is this object a threat to citizens?
    virtual bool threatensCitizens() const;
    
    // Does this object trigger citizens to follow it or flee it?
    virtual bool triggersCitizens() const;
    
private:
    StudentWorld* m_studentWorld;
    bool m_health;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool blocksFlame() const;
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* w, int imageID, double x, double y, int dir, int depth);
};

class Exit : public ActivatingObject
{
public:
    Exit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
    Pit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Flame : public ActivatingObject
{
public:
    Flame(StudentWorld* w, double x, double y, int dir);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Vomit : public ActivatingObject
{
public:
    Vomit(StudentWorld* w, double x, double y, int dir);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Landmine : public ActivatingObject
{
public:
    Landmine(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
};

class Goodie : public ActivatingObject
{
public:
    Goodie(StudentWorld* w, int imageID, double x, double y);
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    virtual bool blocksMovement() const; 
    // Have p pick up this goodie.
    virtual void pickUp(Penelope* p) = 0;
};

class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void pickUp(Penelope* p);
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* w, int imageID, double x, double y, int dir);
    virtual bool blocksMovement() const;
    virtual bool triggersOnlyActiveLandmines() const;
    
};

class Human : public Agent
{
public:
    Human(StudentWorld* w, int imageID, double x, double y);
    virtual void beVomitedOnIfAppropriate();
    virtual bool triggersZombieVomit() const;
    
    // Make this human uninfected by vomit.
    void clearInfection();
    
    // How many ticks since this human was infected by vomit?
    int getInfectionDuration() const;
    
    //increase infection level
    void increaseInfection(){m_infectionLevel++;}
    
private:
    int m_infectionLevel;
};

class Penelope : public Human
{
public:
    Penelope(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    // Increase the number of vaccines the object has.
    void increaseVaccines();
    
    // Increase the number of flame charges the object has.
    void increaseFlameCharges();
    
    // Increase the number of landmines the object has.
    void increaseLandmines();
    
    // How many vaccines does the object have?
    int getNumVaccines() const;
    
    // How many flame charges does the object have?
    int getNumFlameCharges() const;
    
    // How many landmines does the object have?
    int getNumLandmines() const;
    
    virtual bool triggersCitizens() const;
    
private:
    int m_vaccines, m_flameCharges, m_landmines; 
};

class Citizen : public Human
{
public:
    Citizen(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    double dist_p();
    double dist_z();
    bool isParalyzed(){return m_paralysis;}
    void becomeParalyzed();

private:
    bool m_paralysis;
};

class Zombie : public Agent
{
public:
    Zombie(StudentWorld* w, double x, double y);
    virtual bool triggersCitizens() const;
    virtual bool threatensCitizens() const;
};

class DumbZombie : public Zombie
{
public:
    DumbZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
};

class SmartZombie : public Zombie
{
public:
    SmartZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
};

#endif // ACTOR_INCLUDED

