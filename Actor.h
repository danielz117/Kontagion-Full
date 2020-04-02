#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

class StudentWorld;
class Socrates;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);
	virtual void doSomething() = 0;
	bool isAlive() const;
	void setDead();
	StudentWorld* getWorld() const;
	virtual bool takeDamage(int damage);
	bool overlapOther(Actor* a) const;
	bool movementOverlap(Actor* a) const;
	virtual bool hasHitPoints() const;
	virtual bool blocksBacteriumMovement() const;
	virtual bool isEdible() const;
	virtual bool preventsLevelCompleting() const;
private:
	bool alive;
	StudentWorld* m_world;
};

class Dirt : public Actor
{
public:
	Dirt(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual bool takeDamage(int);
	virtual bool blocksBacteriumMovement() const;
};


class Food : public Actor
{
public:
	Food(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual bool isEdible() const;
};

class Pit : public Actor
{
public:
	Pit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual bool preventsLevelCompleting() const;
private:
	int numRSal;
	int numASal;
	int numEColi;
};

class Projectile : public Actor
{
public:
	Projectile(StudentWorld* w, int imageID, double x, double y, int dir);
	virtual void doSomething();
	virtual int getMaxDist() = 0;
	virtual int getDamage() = 0;
private:
	int travelDistance;
};

class Spray : public Projectile
{
public:
	Spray(StudentWorld* w, double x, double y, int dir);
	virtual int getMaxDist();
	virtual int getDamage();
};

class Flame : public Projectile
{
public:
	Flame(StudentWorld* w, double x, double y, int dir);
	virtual int getMaxDist();
	virtual int getDamage();
};

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* w, int imageID, double x, double y);
	virtual bool takeDamage(int damage);
	virtual void doSomething();
	virtual bool notFungus() const;
	virtual void pickUp(Socrates* s) = 0;
private:
	int ticksAlive;
	int maxTicks;
};

class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(StudentWorld* w, double x, double y);
	virtual void pickUp(Socrates* s);

};

class FlamethrowerGoodie : public Goodie
{
public:
	FlamethrowerGoodie(StudentWorld* w, double x, double y);
	virtual void pickUp(Socrates* s);
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* w, double x, double y);
	virtual void pickUp(Socrates* s);
};

class Fungus : public Goodie
{
public:
	Fungus(StudentWorld* w, double x, double y);
	virtual void pickUp(Socrates* s);
	virtual bool notFungus() const;
};

class Agent : public Actor
{
public:
	Agent(StudentWorld* w, int imageID, double x, double y, int dir, int hitPoints);
	virtual bool takeDamage(int damage);
	virtual bool hasHitPoints() const;
	int numHitPoints() const;
	virtual void restoreHealth();
	virtual int soundWhenHurt() const = 0;
	virtual int soundWhenDie() const = 0;
private:
	int m_hitPoints; 
	int maxPoints;
};

class Socrates : public Agent
{
public:
	Socrates(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual int soundWhenHurt() const;
	virtual int soundWhenDie() const;
	void addFlames();
	int numFlames() const;
	int numSprays() const;
private:
	int positionalAngle;
	int flameCharges;
	int sprayCharges;
	int waitSpray;
};

class Bacterium : public Agent
{
public:
	Bacterium(StudentWorld* w, int imageID, double x, double y, int hitPoints);
	virtual bool takeDamage(int damage);
	virtual bool preventsLevelCompleting() const;
	int getFood() const;
	void changeFood(int num);
	void duplicate();
	virtual int getDamage() const = 0;
	virtual Actor* newNew(double x, double y) = 0;
private:
	int foodEaten;
};

class EColi : public Bacterium
{
public:
	EColi(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual int soundWhenHurt() const;
	virtual int soundWhenDie() const;
	Actor* newNew(double x, double y);
	virtual int getDamage() const;
}; 

class Salmonella : public Bacterium
{
public:
	Salmonella(StudentWorld* w, double x, double y, int hitPoints);
	virtual void doSomething();
	virtual int soundWhenHurt() const;
	virtual int soundWhenDie() const;
	int getPlanDist() const;
	void changePlanDist(int num);
	virtual bool aggression() const;
	virtual void aggressiveMoves();
	bool changeAggression(bool change);
private:
	int movementPlanDist;
	bool beingAggressive;
};

class RegularSalmonella : public Salmonella
{
public:
	RegularSalmonella(StudentWorld* w, double x, double y);
	virtual int getDamage() const;
	virtual Actor* newNew(double x, double y);
};

class AggressiveSalmonella : public Salmonella
{
public:
	AggressiveSalmonella(StudentWorld* w, double x, double y);
	virtual int getDamage() const;
	virtual Actor* newNew(double x, double y);
	virtual bool aggression() const;
	virtual void aggressiveMoves();
}; 

#endif // ACTOR_INCLUDED