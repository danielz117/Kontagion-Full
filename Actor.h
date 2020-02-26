#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Actor: public GraphObject {
public:
	Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	virtual ~Actor();
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	bool isAlive() const;
	void setDead();
	bool overlapOther(Actor* a) const;
	bool isDamageable() const;
	virtual bool takeDamage(int damage);
private:
	StudentWorld* m_world;
	bool alive;
	bool m_damageable;
	bool hitPoints;
};

class Socrates : public Actor
{
public:
	Socrates(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	void doSomething();
	void setHealth(int num);
	int getHealth() const;
	void addFlame(int num);
	int getSprays() const;
	int getFlames() const;

private:
	int health;
	int positionalAngle;
	int flameCharges;
	int sprayCharges;
	int waitSpray;
};

class Dirt : public Actor {
public:
	Dirt(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	void doSomething();
};

class Food : public Actor {
public:
	Food(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	void doSomething();
};

class Projectile : public Actor {
public:
	Projectile(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	void doSomething();
	virtual bool maxDistance(int num) = 0;
private:
	int travelDistance;
};

class Spray : public Projectile {
public:
	Spray(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	bool maxDistance(int num);
};

class Flame :public Projectile {
public:
	Flame(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	bool maxDistance(int num);
};

class Pit : public Actor {
public:
	Pit(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable);
	void doSomething();
};

class powerUp : public Actor {
public:
	powerUp(int imageID, double startX, double startY, Direction dir, int depth, 
		StudentWorld* world, Socrates* player, bool damageable);
	void doSomething();
	bool playerHit();
	virtual void effect(Socrates* player) = 0;
private:
	Socrates* m_player;
	int ticksAlive;
	int maxTicks;
};

class healthGoodie : public powerUp {
public:
	healthGoodie(int imageID, double startX, double startY, Direction dir, int depth,
		StudentWorld* world, Socrates* player, bool damageable);
	void effect(Socrates* player);

};

class flameGoodie : public powerUp {
public:
	flameGoodie(int imageID, double startX, double startY, Direction dir, int depth, 
		StudentWorld* world, Socrates* player, bool damageable);
	void effect(Socrates* player);
};

class lifeGoodie : public powerUp {
public:
	lifeGoodie(int imageID, double startX, double startY, Direction dir, int depth,
		StudentWorld* world, Socrates* player, bool damageable);
	void effect(Socrates* player);
};

class Fungus : public powerUp {
public:
	Fungus(int imageID, double startX, double startY, Direction dir, int depth,
		StudentWorld* world, Socrates* player, bool damageable);
	void effect(Socrates* player);
};

class Enemy : public Actor {
public:
	Enemy(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world,
		bool damageable, Socrates* player, int damage);
	void doSomething();
	virtual void duplicate(double x, double y) = 0;
private:
	int movePlanDistance;
	int foodEaten;
	Socrates* m_player;
	int m_damage;
};

class RSalmonella : public Enemy {
public:
	RSalmonella(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world,
		bool damageable, Socrates* player, int damage);
	void duplicate(double x, double y);
};

#endif // ACTOR_H_