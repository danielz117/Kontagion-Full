#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include<iostream>
#include <sstream>
#include <iomanip> 
using namespace std;
const double PI = 4 * atan(1);
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	player = nullptr;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

bool StudentWorld::getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const
{
	Actor* potential = nullptr;
	int min = 10000;
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i]->isEdible() && stuff[i]->isAlive()) {
			double xx = (a->getX() - stuff[i]->getX()) * (a->getX() - stuff[i]->getX());
			double yy = (a->getY() - stuff[i]->getY()) * (a->getY() - stuff[i]->getY());
			if (sqrt(xx + yy) <= (double)dist) {
				if (sqrt(xx + yy) < min) {
					potential = stuff[i];
					min = sqrt(xx + yy);
				}
			}
		}
	}
	if (potential != nullptr && min != 10000) {
		angle = (180/PI)*atan2(potential->getY() - a->getY(), potential->getX()- a->getX());
		return true;
	}
	return false;
}

Actor* StudentWorld::overlap(Actor* a)
{
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i]->isAlive()) {
			if (stuff[i]->overlapOther(a)) {
				return stuff[i];
			}
		}
	}
	return nullptr;
}

bool StudentWorld::withinBound(double x, double y) const {
	if (sqrt((x - VIEW_WIDTH / 2) * (x - VIEW_WIDTH / 2) + (y - VIEW_HEIGHT / 2) * (y - VIEW_HEIGHT / 2)) > 120) {
		return false;
	}
	else {
		return true;
	}
}

void StudentWorld::validCoord(double& x, double& y, int size) {
	bool Blocked = false;
	do {
		x = randInt(VIEW_WIDTH / 2 - 120, VIEW_WIDTH / 2 + 120);
		y = randInt(VIEW_HEIGHT / 2 - 120, VIEW_HEIGHT / 2 + 120);
		Blocked = false;

		Dirt* test = new Dirt(this, x, y);
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
	player = new Socrates(this, 0, VIEW_HEIGHT / 2);

	for (int i = 0; i < getLevel(); i++) {
		double x;
		double y;
		validCoord(x, y, stuff.size());
		Pit* pit = new Pit(this, x, y);
		stuff.push_back(pit);
	}

	for (int i = 0; i < min(5 * getLevel(), 25); i++) {
		double x;
		double y;
		validCoord(x, y, stuff.size());
		Food* food = new Food(this, x, y);
		stuff.push_back(food);
	}
	for (int i = 0; i < max(180 - 20 * getLevel(), 20); i++) {
		double x;
		double y;
		validCoord(x, y, getLevel() + min(5 * getLevel(), 25));
		Dirt* dirt = new Dirt(this, x, y);
		stuff.push_back(dirt);
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	if (player->isAlive()) {
		player->doSomething();
	}
	/*else {
		delete player;
		player = nullptr;
	}*/

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

	int ChangeFungus = max(500 - getLevel() * 10, 200);
	if (randInt(0, ChangeFungus) == 0) {
		int randCoord = 5 * randInt(0, 72);
		double x = 128 + 128 * cos((3.1415926535897 / 180) * randCoord);
		double y = 128 + 128 * sin((3.1415926535897 / 180) * randCoord);
		Fungus* fungus = new Fungus(this, x, y);
		stuff.push_back(fungus);
	}

	int ChanceGoodie = max(510 - getLevel()* 10, 250);
	if (randInt(0, ChanceGoodie) == 0) {
		int randCoord = 5 * randInt(0, 72);
		double x = 128 + 128 * cos((3.1415926535897 / 180) * randCoord);
		double y = 128 + 128 * sin((3.1415926535897 / 180) * randCoord);
		int rand = randInt(0, 9);
		if (rand >= 0 && rand <= 5) {
			RestoreHealthGoodie* hgood = new RestoreHealthGoodie(this, x, y);
			stuff.push_back(hgood);
		}
		else if (rand >= 6 && rand <= 8) {
			FlamethrowerGoodie* fgood = new FlamethrowerGoodie(this, x, y);
			stuff.push_back(fgood);
		}
		else if (rand == 9) {
			ExtraLifeGoodie* lgood = new ExtraLifeGoodie(this, x, y);
			stuff.push_back(lgood);
		}
	}

	if (player != nullptr) {
		if (!(player->isAlive())) {
			delete player;
			player = nullptr;
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}
	for (int i = 0; i < stuff.size();) {
		if (stuff[i] != nullptr) {
			if (!(stuff[i]->isAlive())) {
				delete stuff[i];
				stuff[i] = nullptr;
				stuff.erase(stuff.begin() + i);
				continue;
			}
		}
		i++;
	}

	ostringstream oss;
	oss.setf(ios::fixed);
	oss.precision(0);

	oss << "Score: ";
	oss.fill('0');
	if (getScore() < 0) {
		oss << "-" << setw(5) << -getScore();
	}
	else {
		oss << setw(6) << getScore();
	}
	oss << "  Level: ";
	oss.fill(' ');
	oss << setw(2) << getLevel();
	if (player != nullptr) {
		oss << "  Lives: " << getLives();
		oss << "  Health: ";
		oss.fill(' ');
		oss << setw(3) << player->numHitPoints();
		oss << "  Sprays: ";
		oss.fill(' ');
		oss << setw(2) << player->numSprays();
		oss << "  Flames: ";
		oss.fill(' ');
		oss << setw(2) << player->numFlames();
	}

	string text = oss.str();
	this->setGameStatText(text);

	bool completeLevel = true;
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i] != nullptr) {
			if (stuff[i]->isAlive()) {
				if (stuff[i]->preventsLevelCompleting()) {
					completeLevel = false;
				}
			}
		}
	}
	if (completeLevel) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	else {
		return GWSTATUS_CONTINUE_GAME;
	}
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
	for (int i = 0; i < stuff.size();) {
		stuff.erase(stuff.begin());
		continue;
	}
}

void StudentWorld::addActor(Actor* a)
{
	stuff.push_back(a);
}

bool StudentWorld::isBacteriumMovementBlockedAt(Actor* a) const
{
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i]->blocksBacteriumMovement()) {
			if (stuff[i]->isAlive()) {
				if (stuff[i]->movementOverlap(a)) {
					return true;
				}
			}
		}
	}
	return false;
}

Socrates* StudentWorld::getOverlappingSocrates(Actor* a) const
{
	if (player->isAlive()) {
		if (a->overlapOther(player)) {
			return player;
		}
	}
	return nullptr;
}

Actor* StudentWorld::getOverlappingEdible(Actor* a) const
{
	for (int i = 0; i < stuff.size(); i++) {
		if (stuff[i]->isEdible()) {
			if (stuff[i]->isAlive()) {
				if (stuff[i]->overlapOther(a)) {
					return stuff[i];
				}
			}
		}
	}
	return nullptr;
}

bool StudentWorld::getAngleToNearbySocrates(Actor* a, int dist, int& angle) const
{
	double xx = (a->getX() - player->getX()) * (a->getX() - player->getX());
	double yy = (a->getY() - player->getY()) * (a->getY() - player->getY());
	if (sqrt(xx + yy) <= (double)dist) {
		angle = (180 / PI) * atan2(player->getY() - a->getY(), player->getX() - a->getX());
		return true;
	}
	return false;
}
