#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Actor: public GraphObject {
public:
	Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
	virtual void doSomething() = 0;
	StudentWorld* getWorld();
	bool isAlive() const;
	void setDead();
	bool overlapOther(Actor* a) const;
private:
	StudentWorld* m_world;
	bool alive;
};

class Socrates : public Actor
{
public:
	Socrates(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
	void doSomething();
	void setHealth(int num);
	void addFlame(int num);
	void addLife();
private:
	int health;
	int positionalAngle;
	int flameCharges;
	int lives;
};

class Dirt : public Actor {
public:
	Dirt(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
	void doSomething();
};

class Food : public Actor {
public:
	Food(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
	void doSomething();
};

class Pit : public Actor {
public:
	Pit(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
	void doSomething();
};

class powerUp : public Actor {
public:
	powerUp(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player);
	void doSomething();
	bool playerHit();
	virtual void effect(Socrates* player) = 0;
private:
	Socrates* m_player;
	int ticksAlive;
	int level;
};

class healthGoodie : public powerUp {
public:
	healthGoodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player);
	void effect(Socrates* player);

};

class flameGoodie : public powerUp {
public:
	flameGoodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player);
	void effect(Socrates* player);
};

class lifeGoodie : public powerUp {
public:
	lifeGoodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player);
	void effect(Socrates* player);
};

#endif // ACTOR_H_