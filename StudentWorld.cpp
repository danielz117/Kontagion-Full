#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::~StudentWorld() {
	cleanUp();
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	player = nullptr;
}

void StudentWorld::addPoints(int num) {
	points += num;
}

bool StudentWorld::withinBound(double x, double y) const{
	if (sqrt((x - VIEW_WIDTH / 2) * (x - VIEW_WIDTH / 2) + (y - VIEW_HEIGHT / 2) * (y - VIEW_HEIGHT / 2)) > 120) {
		return false;
	}
	else {
		return true;
	}
}

/*bool StudentWorld::blocked(Actor* a, Actor* b) const{
	int xx = (a->getX() - b->getX()) * (a->getX() - b->getX());
	int yy = (a->getY() - b->getY()) * (a->getY() - b->getY());
	if (sqrt( xx+ yy)< 2*SPRITE_RADIUS) {
		return true;
	}
	else {
		return false;
	}
} */

void StudentWorld::validCoord(double &x, double &y, int size) {
	bool Blocked = false;
	do {
		x = randInt(VIEW_WIDTH / 2 - 120, VIEW_WIDTH / 2 + 120);
		y = randInt(VIEW_HEIGHT / 2 - 120, VIEW_HEIGHT / 2 + 120);
		Blocked = false;

		Dirt* test = new Dirt(IID_DIRT, x, y, 0, 1, this);
		for (int j = 0; j < size; j++) {
			if (test->overlapOther(stuff[j])) {
				Blocked = true;
				break;
			}
		}
		delete test;
	} while (!withinBound(x, y) || Blocked);
}

int StudentWorld::init()
{
	level = 1;
	player = new Socrates(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, this);

	for (int i = 0; i < level; i++) {
		double x;
		double y;
		validCoord(x, y, stuff.size());
		Pit* food = new Pit(IID_PIT, x, y, 0, 1, this);
		stuff.push_back(food);
	}

	for (int i = 0; i < min(5 * level, 25); i++) {
		double x;
		double y;
		validCoord(x, y, stuff.size());
		Food* food = new Food(IID_FOOD, x, y, 90, 1, this);
		stuff.push_back(food);
	}

	for (int i = 0; i < max(180 - 20*level, 20); i++) {
		double x;
		double y;
		validCoord(x, y, level + min(5 * level, 25));
		Dirt* dirt = new Dirt(IID_DIRT, x, y, 0, 1, this);
		stuff.push_back(dirt);
	} 
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	if (player->isAlive()) {
		player->doSomething();
	}
	else {
		delete player;
		player = nullptr;
	}
	for (int i = 0; i < stuff.size(); ) {
		if (stuff[i]->isAlive()) {
			stuff[i]->doSomething();
		}
		else {
			delete stuff[i];
			stuff[i] = nullptr;
			stuff.erase(stuff.begin() + i);
			continue;
		}
		i++;
	}

	int ChanceGoodie = min(510 - level * 10, 250);
	if (randInt(0,ChanceGoodie) == 0) {
		int randCoord = 5 * randInt(0, 72);
		double x = 128 + 128 * cos((3.1415926535897 / 180) * randCoord);
		double y = 128 + 128 * sin((3.1415926535897 / 180) * randCoord);
		int rand = randInt(0, 10);
		if (rand >= 0 && rand <= 5) {
			healthGoodie* hgood = new healthGoodie(IID_RESTORE_HEALTH_GOODIE, x, y, 0, 1, this, level, player);
			stuff.push_back(hgood);
		}
		else if (rand >= 6 && rand <= 8) {
			flameGoodie* fgood = new flameGoodie(IID_FLAME_THROWER_GOODIE, x, y, 0, 1, this, level, player);
			stuff.push_back(fgood);
		}
		else if (rand == 9) {
			lifeGoodie* lgood = new lifeGoodie(IID_EXTRA_LIFE_GOODIE, x, y, 0, 1, this, level, player);
			stuff.push_back(lgood);
		}
		
	}

    decLives();
    //return GWSTATUS_PLAYER_DIED;
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (player != nullptr) {
		delete player;
	}
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i] != nullptr) {
			delete stuff[i];
		}	
	}
}
