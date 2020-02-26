#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	//bool blocked(Actor* a, Actor* b) const;
	void addSpray(double startX, double startY, Direction dir);
	void addFlame(double startX, double startY, Direction dir);
	void addRSalmonella(double startX, double startY);
	Actor* hitSomething(Projectile* p);

private:
	int points;
	int level;
	Socrates* player;
	vector<Actor*> stuff;
	bool withinBound(double x, double y) const;
	void validCoord(double& x, double& y, int size);
};

#endif // STUDENTWORLD_H_
