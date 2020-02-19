#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
	: GraphObject(imageID, startX, startY, dir) {
	m_world = world;
	alive = true;
}

bool Actor::isAlive() const{
	return alive;
}

void Actor::setDead() {
	alive = false;
}

bool Actor::overlapOther(Actor* a) const {
	double xx = (a->getX() - this->getX()) * (a->getX() - this->getX());
	double yy = (a->getY() - this->getY()) * (a->getY() - this->getY());
	if (sqrt(xx + yy) < (double)2 * SPRITE_RADIUS) {
		return true;
	}
	else {
		return false;
	}
}

StudentWorld* Actor::getWorld() {
	return m_world;
}

Socrates::Socrates(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
	: Actor(imageID, startX, startY, dir, depth, world) {
	health = 100;
	positionalAngle = 180;
	flameCharges = 5;
	lives = 3;
}

void Socrates::addLife() {
	lives++;
}

void Socrates::setHealth(int num) {
	health = num;
}

void Socrates::addFlame(int num) {
	flameCharges += num;
}

void Socrates::doSomething() {
	if (health <= 0) {
		setDead();
	}
	if (!isAlive()) {
		return;
	}

	int ch;
	if (getWorld()->getKey(ch)) {
		
		switch (ch) {
		case KEY_PRESS_LEFT:
			positionalAngle -= 5;
			moveTo(128 + 128 * cos((3.1415926535897/180)*positionalAngle), 
				128 + 128 * sin((3.1415926535897 / 180) * positionalAngle));
			setDirection(180 + positionalAngle);
			break;

		case KEY_PRESS_RIGHT:
			positionalAngle += 5;
			moveTo(128 + 128 * cos((3.1415926535897 / 180) * positionalAngle),
				128 + 128 * sin((3.1415926535897 / 180) * positionalAngle));
			setDirection(180 + positionalAngle);
			break;
		case KEY_PRESS_SPACE:

			break;
		}
	}
}

Dirt::Dirt(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world) 
	: Actor(imageID, startX, startY, dir, depth, world){

}

void Dirt::doSomething() {

}

Food::Food(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
	: Actor(imageID, startX, startY, dir, depth, world) {

}

void Food::doSomething() {

}

Pit::Pit(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
	: Actor(imageID, startX, startY, dir, depth, world) {

}

void Pit::doSomething() {

}

powerUp::powerUp(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player)
	: Actor(imageID, startX, startY, dir, depth, world) {
	ticksAlive = 0;
	m_player = player;
}

bool powerUp::playerHit() {
	if (overlapOther(m_player)) {
		return true;
	}
	else {
		return false;
	}
}

void powerUp::doSomething() {
	if (!(isAlive())) {
		return;
	}
	else {
		if (playerHit()) {
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setDead();
			effect(m_player);
			return;
		}
		else if (ticksAlive > max(rand()% (300 - 10* level),50)) {
			setDead();
		}
	}
	ticksAlive++;
}

healthGoodie::healthGoodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player)
	: powerUp(imageID, startX, startY, dir, depth, world, level, player) {

}

void healthGoodie::effect(Socrates* player) {
	player->setHealth(100);
	getWorld()->addPoints(250);
}

flameGoodie::flameGoodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player)
	: powerUp(imageID, startX, startY, dir, depth, world, level, player) {

}

void flameGoodie::effect(Socrates* player){
	player->addFlame(5);
	getWorld()->addPoints(300);
}

lifeGoodie::lifeGoodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, int level, Socrates* player)
	: powerUp(imageID, startX, startY, dir, depth, world, level, player) {

}

void lifeGoodie::effect(Socrates* player) {
	player->addLife();
	getWorld()->addPoints(500);
}

template <typename Actor>
bool operator< (const Actor &a, const Actor &b) {
	return (a.getX() > b.getX());
}

