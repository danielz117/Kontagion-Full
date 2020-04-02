#ifndef STUDENTWORLD_INCLUDED
#define STUDENTWORLD_INCLUDED

#include "GameWorld.h"
#include <string>
#include <vector>
#include "Actor.h"

class Actor;
class Socrates;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addActor(Actor* a);
	bool isBacteriumMovementBlockedAt(Actor* a) const;
	Socrates* getOverlappingSocrates(Actor* a) const;
	Actor* getOverlappingEdible(Actor* a) const;
	bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;
	bool getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const;
	Actor* overlap(Actor* a);
	
private:
	Socrates* player;
	std::vector<Actor*> stuff;
	void validCoord(double& x, double& y, int size);
	bool withinBound(double x, double y) const;
};

#endif // STUDENTWORLD_INCLUDED