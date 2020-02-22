#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
const double PI = 4 * atan(1);
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: GraphObject(imageID, startX, startY, dir, depth) {
	m_world = world;
	alive = true;
	m_damageable = damageable;
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
	if (sqrt(xx + yy) <= (double)SPRITE_WIDTH) {
		return true;
	}
	return false;
}

bool Actor::isDamageable() const {
	return m_damageable;
}

StudentWorld* Actor::getWorld() const{
	return m_world;
}

Socrates::Socrates(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Actor(imageID, startX, startY, dir, depth, world, damageable) {
	health = 100;
	positionalAngle = 180;
	flameCharges = 5;
	sprayCharges = 20;
	waitSpray = 0;
}

void Socrates::setHealth(int num) {
	health = num;
}

int Socrates::getHealth() const {
	return health;
}

void Socrates::addFlame(int num) {
	flameCharges += num;
}

int Socrates::getSprays() const {
	return sprayCharges;
}

int Socrates::getFlames() const {
	return flameCharges;
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
			positionalAngle += 5;
			moveTo(VIEW_RADIUS + VIEW_RADIUS * cos((PI/180)*positionalAngle), 
				VIEW_RADIUS + VIEW_RADIUS * sin((PI/ 180) * positionalAngle));
			setDirection(180 + positionalAngle);
			break;
		case KEY_PRESS_RIGHT:
			positionalAngle -= 5;
			moveTo(VIEW_RADIUS + VIEW_RADIUS * cos((PI/ 180) * positionalAngle),
				VIEW_RADIUS + VIEW_RADIUS * sin((PI/ 180) * positionalAngle));
			setDirection(180 + positionalAngle);
			break;
		case KEY_PRESS_SPACE:
			if (sprayCharges > 0) {
				getWorld()->addSpray(this->getX() + SPRITE_WIDTH * cos(this->getDirection() * 1.0 / 360 * 2 * PI),
					this->getY() + SPRITE_WIDTH * sin(this->getDirection() * 1.0 / 360 * 2 * PI), this->getDirection());
				sprayCharges--;
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				waitSpray = 0;
				break;
			}
			else {
				if (waitSpray % 7 == 6) {
					sprayCharges++;
				}
				waitSpray++;
				waitSpray %= 7;
				break;
			}
		case KEY_PRESS_ENTER:
			if (flameCharges > 0) {
				for (int i = 0; i < 16; i++) {
					getWorld()->addFlame(this->getX() + SPRITE_WIDTH * cos((this->getDirection() + (double) i*22) * 1.0 / 360 * 2 * PI),
						this->getY() + SPRITE_WIDTH * sin((this->getDirection()+(double)i*22) * 1.0 / 360 * 2 * PI), this->getDirection() + i*22);
				}
				flameCharges--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		}
	}
	else {
		if (sprayCharges < 20 && waitSpray %7 == 6) {
			sprayCharges++;
		}
		else if (sprayCharges < 20) {
			waitSpray++;
			waitSpray %= 7;
		}
	}
}

Dirt::Dirt(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Actor(imageID, startX, startY, dir, depth, world, damageable){

}

void Dirt::doSomething() {

}

Food::Food(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Actor(imageID, startX, startY, dir, depth, world, damageable) {

}

void Food::doSomething() {

}

Projectile::Projectile(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Actor(imageID, startX, startY, dir, depth, world, damageable) {
	travelDistance = 0;
}

void Projectile::doSomething() {
	if (!(isAlive())) {
		return;
	}
	else {
		if (getWorld()->hitSomething(this) != nullptr) {
			if (getWorld()->hitSomething(this)->isDamageable()) {
				getWorld()->hitSomething(this)->setDead();
				setDead();
				return;
			}
		}
		moveForward(SPRITE_WIDTH);
		travelDistance += SPRITE_WIDTH;
		if (maxDistance(travelDistance)) {
			setDead();
		}
	}
}

Spray::Spray(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Projectile(imageID, startX, startY, dir, depth, world, damageable) {

}

bool Spray::maxDistance(int num){
	if (num >= 112) {
		return true;
	}
	return false;
}

Flame::Flame(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Projectile(imageID, startX, startY, dir, depth, world, damageable) {

}

bool Flame::maxDistance(int num) {
	if (num >= 32) {
		return true;
	}
	return false;
}

Pit::Pit(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world, bool damageable)
	: Actor(imageID, startX, startY, dir, depth, world, damageable) {

}

void Pit::doSomething() {

}

powerUp::powerUp(int imageID, double startX, double startY, Direction dir, int depth, 
	StudentWorld* world, Socrates* player, bool damageable)
	: Actor(imageID, startX, startY, dir, depth, world, damageable) {
	ticksAlive = 0;
	m_player = player;
	maxTicks = max(randInt(0, 300 - 10 * getWorld()->getLevel() - 1), 50);
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
		else if (ticksAlive > maxTicks) {
			setDead();
		}
	}
	ticksAlive++;
}

healthGoodie::healthGoodie(int imageID, double startX, double startY, Direction dir, int depth,
	StudentWorld* world, Socrates* player, bool damageable)
	: powerUp(imageID, startX, startY, dir, depth, world, player, damageable) {

}

void healthGoodie::effect(Socrates* player) {
	player->setHealth(100);
	getWorld()->increaseScore(250);
}

flameGoodie::flameGoodie(int imageID, double startX, double startY, Direction dir, int depth,
	StudentWorld* world, Socrates* player, bool damageable)
	: powerUp(imageID, startX, startY, dir, depth, world, player, damageable) {

}

void flameGoodie::effect(Socrates* player){
	player->addFlame(5);
	getWorld()->increaseScore(300);
}

lifeGoodie::lifeGoodie(int imageID, double startX, double startY, Direction dir, int depth,
	StudentWorld* world, Socrates* player, bool damageable)
	: powerUp(imageID, startX, startY, dir, depth, world, player, damageable) {

}

void lifeGoodie::effect(Socrates* player) {
	getWorld()->incLives();
	getWorld()->increaseScore(500);
}

Fungus::Fungus(int imageID, double startX, double startY, Direction dir, int depth,
	StudentWorld* world, Socrates* player, bool damageable)
	: powerUp(imageID, startX, startY, dir, depth, world, player, damageable) {

}

void Fungus::effect(Socrates* player) {
	player->setHealth(player->getHealth() - 20);
	getWorld()->increaseScore(-50);
}

Enemy::Enemy(int imageID, double startX, double startY, Direction dir, int depth,
	StudentWorld* world, bool damageable, Socrates* player, int damage)
	:Actor(imageID, startX, startY, dir, depth, world, damageable) {
	movePlanDistance = 0;
	foodEaten = 0;
	m_player = player;
	m_damage = damage;
};

void Enemy::doSomething() {
	if (!(isAlive())) {
		return;
	}

	if (overlapOther(m_player)) {
		m_player->setHealth(m_player->getHealth() - m_damage);
	}
	else if (foodEaten >= 3) {
		double newX;
		if (getX() < VIEW_WIDTH / 2) {
			newX = getX() + SPRITE_WIDTH / 2;
		}
		else if (getX() > VIEW_WIDTH/2){
			newX = getX() - SPRITE_WIDTH / 2; 
		}
		else {
			newX = VIEW_WIDTH / 2;
		}
		double newY;
		if (getY() < VIEW_HEIGHT / 2) {
			newY = getY() + SPRITE_WIDTH / 2;
		}
		else if (getY() > VIEW_HEIGHT / 2) {
			newY = getY() - SPRITE_WIDTH / 2;
		}
		else {
			newY = VIEW_HEIGHT / 2;
		}
		duplicate(newX, newY);
	}
}

RSalmonella::RSalmonella(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world,
	bool damageable, Socrates* player, int damage)
	:Enemy(imageID, startX, startY, dir, depth, world, damageable, player, damage) {

}

void RSalmonella::duplicate(double x, double y) {
	getWorld()->addRSalmonella(x, y);
}

template <typename Actor>
bool operator< (const Actor &a, const Actor &b) {
	return (a.getX() > b.getX());
}

