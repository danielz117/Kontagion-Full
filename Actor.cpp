#include "Actor.h"
#include "StudentWorld.h"

using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
const double PI = 4 * atan(1);
Actor::Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	: GraphObject(imageID, x, y, dir, depth)
{
	alive = true;
	m_world = w;
}

bool Actor::isAlive() const
{
	return alive;
}

void Actor::setDead()
{
	alive = false;
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}

bool Actor::takeDamage(int)
{
	return false;
}

bool Actor::overlapOther(Actor* a) const
{
	double xx = (a->getX() - this->getX()) * (a->getX() - this->getX());
	double yy = (a->getY() - this->getY()) * (a->getY() - this->getY());
	if (sqrt(xx + yy) <= (double)SPRITE_WIDTH) {
		return true;
	}
	return false;
}

bool Actor::movementOverlap(Actor* a) const
{
	double xx = (a->getX() - this->getX()) * (a->getX() - this->getX());
	double yy = (a->getY() - this->getY()) * (a->getY() - this->getY());
	if (sqrt(xx + yy) <= (double)SPRITE_WIDTH/2) {
		return true;
	}
	return false;
}

bool Actor::hasHitPoints() const
{
	return false;
}

bool Actor::blocksBacteriumMovement() const
{
	return false;
}

bool Actor::isEdible() const
{
	return false;
}

bool Actor::preventsLevelCompleting() const
{
	return false;
}

Dirt::Dirt(StudentWorld* w, double x, double y)
	:Actor(w, IID_DIRT, x, y, 0, 1)
{
}

void Dirt::doSomething()
{

}

bool Dirt::takeDamage(int)
{
	return true;
}

bool Dirt::blocksBacteriumMovement() const
{
	return true;
}

Food::Food(StudentWorld* w, double x, double y)
	:Actor(w, IID_FOOD, x, y, 90, 1)
{
}

void Food::doSomething()
{

}

bool Food::isEdible() const
{
	return true;
}

Pit::Pit(StudentWorld* w, double x, double y)
	:Actor(w, IID_PIT, x, y, 0, 1)
{
	numRSal = 5;
	numASal = 3;
	numEColi = 2;
}

void Pit::doSomething()
{
	if (numRSal == 0 && numASal == 0 && numEColi == 0) {
		setDead();
	}
	else {
		if (randInt(0, 49) == 0) {
			if (numRSal != 0 && numASal != 0 && numEColi != 0) {
				int rand = randInt(0, 2);
				if (rand == 0) {
					RegularSalmonella* renemy = new RegularSalmonella(getWorld(), getX(), getY());
					getWorld()->addActor(renemy);
					numRSal--;
				}
				else if (rand == 1) {
					AggressiveSalmonella* aenemy = new AggressiveSalmonella(getWorld(), getX(), getY());
					getWorld()->addActor(aenemy);
					numASal--;
				}
				else if (rand == 2) {
					EColi* eenemy = new EColi(getWorld(), getX(), getY());
					getWorld()->addActor(eenemy);
					numEColi--;
				}
			}
			else if (numRSal != 0 && numASal != 0) {
				int rand = randInt(0, 1);
				if (rand == 0) {
					RegularSalmonella* renemy = new RegularSalmonella(getWorld(), getX(), getY());
					getWorld()->addActor(renemy);
					numRSal--;
				}
				else if (rand == 1) {
					AggressiveSalmonella* aenemy = new AggressiveSalmonella(getWorld(), getX(), getY());
					getWorld()->addActor(aenemy);
					numASal--;
				}
			}
			else if (numRSal != 0 && numEColi != 0) {
				int rand = randInt(0, 1);
				if (rand == 0) {
					RegularSalmonella* renemy = new RegularSalmonella(getWorld(), getX(), getY());
					getWorld()->addActor(renemy);
					numRSal--;
				}
				else if (rand == 1) {
					EColi* eenemy = new EColi(getWorld(), getX(), getY());
					getWorld()->addActor(eenemy);
					numEColi--;
				}
			}
			else if (numASal != 0 && numEColi != 0) {
				int rand = randInt(0, 1);
				if (rand == 0) {
					AggressiveSalmonella* aenemy = new AggressiveSalmonella(getWorld(), getX(), getY());
					getWorld()->addActor(aenemy);
					numASal--;
				}
				else if (rand == 1) {
					EColi* eenemy = new EColi(getWorld(), getX(), getY());
					getWorld()->addActor(eenemy);
					numEColi--;
				}
			}
			else if (numRSal != 0) {
				RegularSalmonella* renemy = new RegularSalmonella(getWorld(), getX(), getY());
				getWorld()->addActor(renemy);
				numRSal--;
			}
			else if (numASal != 0) {
				AggressiveSalmonella* aenemy = new AggressiveSalmonella(getWorld(), getX(), getY());
				getWorld()->addActor(aenemy);
				numASal--;
			}
			else if (numEColi != 0) {
				EColi* eenemy = new EColi(getWorld(), getX(), getY());
				getWorld()->addActor(eenemy);
				numEColi--;
			}
			getWorld()->playSound(SOUND_BACTERIUM_BORN);
		}
		
	}
}

bool Pit::preventsLevelCompleting() const
{
	return true;
}

Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir, int hitPoints)
	:Actor(w, imageID, x ,y, dir, 0)
{
	m_hitPoints = hitPoints;
	maxPoints = hitPoints;
}

bool Agent::takeDamage(int damage)
{
	m_hitPoints -= damage;
	if (m_hitPoints <= 0) {
		setDead();
		getWorld()->playSound(soundWhenDie());
	}
	getWorld()->playSound(soundWhenHurt());
	return true;
}

bool Agent::hasHitPoints() const
{
	return true;
}

int Agent::numHitPoints() const
{
	return m_hitPoints;
}

void Agent::restoreHealth()
{
	m_hitPoints = maxPoints;
}

Socrates::Socrates(StudentWorld* w, double x, double y)
	:Agent(w, IID_PLAYER, x, y, 0, 100)
{
	positionalAngle = 180;
	flameCharges = 5;
	sprayCharges = 20;
	waitSpray = 0;
}

void Socrates::doSomething()
{
	if (numHitPoints() <= 0) {
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
			moveTo(VIEW_RADIUS + VIEW_RADIUS * cos((PI / 180) * positionalAngle),
				VIEW_RADIUS + VIEW_RADIUS * sin((PI / 180) * positionalAngle));
			setDirection(180 + positionalAngle);
			break;
		case KEY_PRESS_RIGHT:
			positionalAngle -= 5;
			moveTo(VIEW_RADIUS + VIEW_RADIUS * cos((PI / 180) * positionalAngle),
				VIEW_RADIUS + VIEW_RADIUS * sin((PI / 180) * positionalAngle));
			setDirection(180 + positionalAngle);
			break;
		case KEY_PRESS_SPACE:
			if (sprayCharges > 0) {
				Spray* spray = new Spray(getWorld(), this->getX() + SPRITE_WIDTH * cos(this->getDirection() * 1.0 / 360 * 2 * PI),
					this->getY() + SPRITE_WIDTH * sin(this->getDirection() * 1.0 / 360 * 2 * PI), this->getDirection());
				getWorld()->addActor(spray);
				sprayCharges--;
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				waitSpray = 0;
				break;
			}
			else {
				if (waitSpray % 2 == 1) {
					sprayCharges++;
				}
				waitSpray++;
				waitSpray %= 2;
				break;
			}
		case KEY_PRESS_ENTER:
			if (flameCharges > 0) {
				for (int i = 0; i < 16; i++) {
					Flame* flame = new Flame(getWorld(), this->getX() + SPRITE_WIDTH * cos((this->getDirection() 
						+ (double)i * 22) * 1.0 / 360 * 2 * PI),this->getY()+ SPRITE_WIDTH * sin((this->getDirection() 
							+ (double)i * 22) * 1.0 / 360 * 2 * PI), this->getDirection() + i * 22);
					getWorld()->addActor(flame);
				}
				flameCharges--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		}
	}
	else {
		if (sprayCharges < 20 && waitSpray % 2 == 1) {
			sprayCharges++;
		}
		else if (sprayCharges < 20) {
			waitSpray++;
			waitSpray %= 2;
		} 
	}
}

int Socrates::soundWhenHurt() const
{
	return SOUND_PLAYER_HURT;
}

int Socrates::soundWhenDie() const
{
	return SOUND_PLAYER_DIE;
}

void Socrates::addFlames()
{
	flameCharges += 5;
}

int Socrates::numFlames() const
{
	return flameCharges;
}

int Socrates::numSprays() const
{
	return sprayCharges;
}

Goodie::Goodie(StudentWorld* w, int imageID, double x, double y)
	:Actor(w, imageID, x, y, 0, 1)
{
	ticksAlive = 0;
	maxTicks = max(randInt(0, 300 - 10 * getWorld()->getLevel() - 1), 50);
}

bool Goodie::takeDamage(int)
{
	return true;
}

void Goodie::doSomething()
{
	if (!(isAlive())) {
		return;
	}
	else {
		if (getWorld()->getOverlappingSocrates(this) != nullptr) {
			if (notFungus()) {
				getWorld()->playSound(SOUND_GOT_GOODIE);
			}
			setDead();
			pickUp(getWorld()->getOverlappingSocrates(this));
			return;
		}
		else if (ticksAlive > maxTicks) {
			setDead();
		}
	}
	ticksAlive++;
}

bool Goodie::notFungus() const
{
	return true;
}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* w, double x, double y)
	:Goodie(w, IID_RESTORE_HEALTH_GOODIE, x, y)
{
}

void RestoreHealthGoodie::pickUp(Socrates* s)
{
	s->restoreHealth();
	getWorld()->increaseScore(250);
}

FlamethrowerGoodie::FlamethrowerGoodie(StudentWorld* w, double x, double y)
	:Goodie(w, IID_FLAME_THROWER_GOODIE, x, y)
{
}

void FlamethrowerGoodie::pickUp(Socrates* s)
{
	s->addFlames();
	getWorld()->increaseScore(300);
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* w, double x, double y)
	:Goodie(w, IID_EXTRA_LIFE_GOODIE, x, y)
{
}

void ExtraLifeGoodie::pickUp(Socrates* s)
{
	s->getWorld()->incLives();
	s->getWorld()->increaseScore(500);
}

Fungus::Fungus(StudentWorld* w, double x, double y)
	: Goodie(w, IID_FUNGUS, x, y)
{
}

void Fungus::pickUp(Socrates* s)
{
	s->takeDamage(20);
	getWorld()->increaseScore(-50);
}

bool Fungus::notFungus() const
{
	return false;
}

Projectile::Projectile(StudentWorld* w, int imageID, double x, double y, int dir)
	:Actor(w, imageID, x, y, dir, 1)
{
	travelDistance = 0;
}

void Projectile::doSomething()
{
	if (!(isAlive())) {
		return;
	}
	else {
		Actor* hit = getWorld()->overlap(this);
		if (hit != nullptr) {
			if (hit->takeDamage(0)) {
				if (!(hit->hasHitPoints())) {
					hit->setDead();
				}
				else {
					hit->takeDamage(getDamage());
				}
				setDead();
				return;
			}
		}
		moveForward(SPRITE_WIDTH);
		travelDistance += SPRITE_WIDTH;
		if (travelDistance >= getMaxDist()) {
			setDead();
		}
	}
}

Spray::Spray(StudentWorld* w, double x, double y, int dir)
	:Projectile(w, IID_SPRAY, x, y, dir)
{
}

int Spray::getMaxDist()
{
	return 112;
}

int Spray::getDamage()
{
	return 2;
}

Flame::Flame(StudentWorld* w, double x, double y, int dir)
	:Projectile(w, IID_FLAME, x, y, dir)
{
}

int Flame::getMaxDist()
{
	return 32;
}

int Flame::getDamage()
{
	return 5;
}

Bacterium::Bacterium(StudentWorld* w, int imageID, double x, double y, int hitPoints)
	:Agent(w, imageID, x, y, 90, hitPoints)
{
	foodEaten = 0;
}

bool Bacterium::takeDamage(int damage)
{
	Agent::takeDamage(damage);
	if (numHitPoints() <= 0) {
		setDead();
		getWorld()->playSound(soundWhenDie());
		getWorld()->increaseScore(100);
		if (randInt(0, 1) == 0) {
			Food* foo = new Food(getWorld(), getX(), getY());
			getWorld()->addActor(foo);
		}
	}
	else {
		getWorld()->playSound(soundWhenHurt());
	}
	return true;
}

int Bacterium::getFood() const
{
	return foodEaten;
}

void Bacterium::changeFood(int num)
{
	foodEaten += num;
}

void Bacterium::duplicate()
{
	double newX;
	if (getX() < VIEW_WIDTH / 2) {
		newX = getX() + SPRITE_WIDTH / 2;
	}
	else if (getX() > VIEW_WIDTH / 2) {
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
	getWorld()->addActor(newNew(newX, newY));
	changeFood(-3);
}

bool Bacterium::preventsLevelCompleting() const
{
	return true;
}

Salmonella::Salmonella(StudentWorld* w, double x, double y, int hitPoints)
	:Bacterium(w, IID_SALMONELLA, x ,y, hitPoints)
{
	movementPlanDist = 0;
	beingAggressive = false;
}

void Salmonella::doSomething()
{
	beingAggressive = false;
	if (!(isAlive())) {
		return;
	}
	if (aggression()) {
		Actor* player = getWorld()->getOverlappingSocrates(this);
		if (player != nullptr) {
			player->takeDamage(getDamage());
			if (sqrt((this->getX() - player->getX()) * (this->getX() - player->getX())
				+ (this->getY() == player->getY()) * (double)(this->getY() == player->getY())) < 2.5) {
				return;
			}
		}
		aggressiveMoves();
	}
	if (getWorld()->getOverlappingSocrates(this) != nullptr) {
		getWorld()->getOverlappingSocrates(this)->takeDamage(getDamage());
		if (aggression()) {
			return;
		}
	}
	else if (getFood() >= 3) {
		duplicate();
	}
	else if (getWorld()->getOverlappingEdible(this) != nullptr) {
		getWorld()->getOverlappingEdible(this)->setDead();
		changeFood(1);
	}

	if (beingAggressive) {
		return;
	}
	if (getPlanDist() > 0) {
		changePlanDist(-1);
		bool blocked = false;
		RegularSalmonella* test = new RegularSalmonella(getWorld(), getX(), getY());
		test->moveAngle(getDirection(), 3);
		if (getWorld()->isBacteriumMovementBlockedAt(test)) {
			blocked = true;
		}

		if (sqrt((test->getX() - VIEW_WIDTH / 2) * (test->getX() - VIEW_WIDTH / 2) + (test->getY() - VIEW_HEIGHT / 2)
			* (test->getY() - VIEW_HEIGHT / 2)) > VIEW_RADIUS) {
			blocked = true;
		}
		delete test;
		if (!(blocked)) {
			moveAngle(getDirection(), 3);
		}
		else {
			setDirection(randInt(0, 359));
			changePlanDist(10 - getPlanDist());
		}
		return;
	}
	else {
		int angle = 0;
		if (!(getWorld()->getAngleToNearestNearbyEdible(this, 128, angle))) {
			setDirection(randInt(0, 359));
			changePlanDist(10 - getPlanDist());
			return;
		}
		else {
			setDirection(angle);
			bool blocked = false;
			RegularSalmonella* test = new RegularSalmonella(getWorld(), getX(), getY());
			test->moveAngle(getDirection(), 3);
			if (getWorld()->isBacteriumMovementBlockedAt(test)) {
				blocked = true;
			}
			if (sqrt((test->getX() - VIEW_WIDTH / 2) * (test->getX() - VIEW_WIDTH / 2) + (test->getY() - VIEW_HEIGHT / 2)
				* (test->getY() - VIEW_HEIGHT / 2)) > VIEW_RADIUS) {
				blocked = true;
			}
			delete test;
			if (!(blocked)) {
				moveAngle(getDirection(), 3);
				changePlanDist(10 - getPlanDist());
			}
			else {
				setDirection(randInt(0, 359));
				changePlanDist(10 - getPlanDist());
			}
			return;
		}
	}
}

int Salmonella::soundWhenHurt() const
{
	return SOUND_SALMONELLA_HURT;
}

int Salmonella::soundWhenDie() const
{
	return SOUND_SALMONELLA_DIE;
}

int Salmonella::getPlanDist() const
{
	return movementPlanDist;
}

void Salmonella::changePlanDist(int num)
{
	movementPlanDist += num;
}

bool Salmonella::aggression() const
{
	return false;
}

void Salmonella::aggressiveMoves()
{
}

bool Salmonella::changeAggression(bool change)
{
	return beingAggressive = change;
}

RegularSalmonella::RegularSalmonella(StudentWorld* w, double x, double y)
	:Salmonella(w, x, y, 4)
{
}

int RegularSalmonella::getDamage() const
{
	return 1;
}

Actor* RegularSalmonella::newNew(double x, double y)
{
	RegularSalmonella* dup = new RegularSalmonella(getWorld(), x, y);
	return dup;
}

AggressiveSalmonella::AggressiveSalmonella(StudentWorld* w, double x, double y)
	:Salmonella(w, x, y, 10)
{
}

int AggressiveSalmonella::getDamage() const
{
	return 2;
}

Actor* AggressiveSalmonella::newNew(double x, double y)
{
	AggressiveSalmonella* dup = new AggressiveSalmonella(getWorld(), x, y);
	return dup;
}

bool AggressiveSalmonella::aggression() const
{
	return true;
}

void AggressiveSalmonella::aggressiveMoves()
{
	int angle;
	if (getWorld()->getAngleToNearbySocrates(this, 72, angle)) {
		changeAggression(true);
		setDirection(angle);
		AggressiveSalmonella* test = new AggressiveSalmonella(getWorld(), getX(), getY());
		for (int i = 0; i < 3; i++) {
			test->moveAngle(getDirection(), 1);
			if (!(getWorld()->isBacteriumMovementBlockedAt(test))) {
				moveAngle(getDirection(), 1);
			}
			
			else {
				break;
			}
		}
		delete test;
	}
	
}

EColi::EColi(StudentWorld* w, double x, double y)
	:Bacterium(w, IID_ECOLI, x ,y, 5)
{
}

void EColi::doSomething()
{
	if (!(isAlive())) {
		return;
	}

	Actor* player = getWorld()->getOverlappingSocrates(this);
	if (player != nullptr) {
		player->takeDamage(getDamage());
		if (sqrt((this->getX()-player->getX())*(this->getX() - player->getX()) 
			+ (this->getY() == player->getY())* (double)(this->getY() == player->getY())) < 1.5) {
			return;
		}	
	}
	else if (getFood() >= 3) {
		duplicate();
	}
	else if (getWorld()->getOverlappingEdible(this) != nullptr) {
		getWorld()->getOverlappingEdible(this)->setDead();
		changeFood(1);
	}
	int angle = 0;
	if (getWorld()->getAngleToNearbySocrates(this, 256, angle)) {
		setDirection(angle);
		for (int i = 0; i < 10; i++) {
			EColi* test = new EColi(getWorld(), getX(), getY());
			test->moveAngle(getDirection(), 2);
			if (!(getWorld()->isBacteriumMovementBlockedAt(test))) {
				moveAngle(getDirection(), 2);
				delete test;
				return;
			}
			else {
				setDirection(getDirection() + 10);
			}
			delete test;
		}
	}
}

int EColi::soundWhenHurt() const
{
	return SOUND_ECOLI_HURT;
}

int EColi::soundWhenDie() const
{
	return SOUND_ECOLI_DIE;
}

Actor* EColi::newNew(double x, double y)
{
	EColi* dup = new EColi(getWorld(), x, y);
	return dup;
}

int EColi::getDamage() const
{
	return 4;
}

