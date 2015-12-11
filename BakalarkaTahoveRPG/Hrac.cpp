#include "Hrac.h"
#include "Zameranie.h"
#include "Animacia.h"
#include <iostream>

#define RYCHLOST 2;

Hrac::Hrac(Zameranie* paZameranie) {
	zameranie = paZameranie;
	std::cout << "Hrac vytvoreny" << std::endl;
	animaciaDole = new Animacia("Data/Grafika/Hrac/Animacia/hrac_dole.png", 4, 10,32);
	animaciaHore = new Animacia("Data/Grafika/Hrac/Animacia/hrac_hore.png", 4, 10, 32);
	animaciaVlavo = new Animacia("Data/Grafika/Hrac/Animacia/hrac_vlavo.png", 4, 10, 32);
	animaciaVpravo = new Animacia("Data/Grafika/Hrac/Animacia/hrac_vpravo.png", 4, 10, 32);
	smerPohladu = SmerPohladu::dole;
	hybeSa = false;
	

}



Hrac::~Hrac() {
	delete(zameranie);
	std::cout << "Hrac zmazany" << std::endl;
}





void Hrac::posunSa(int kam) {

}


void Hrac::render(sf::RenderWindow* paOkno) {

	sf::Sprite* obrazok;
	if (smerPohladu == hore) {
		obrazok = animaciaHore->dajObrazok();
	}

	if (smerPohladu == dole) {
		obrazok = animaciaDole->dajObrazok();
	}

	if (smerPohladu == vlavo) {
		obrazok = animaciaVlavo->dajObrazok();
	}

	if (smerPohladu == vpravo) {
		obrazok = animaciaVpravo->dajObrazok();
	}

	obrazok->setPosition(sf::Vector2f(offsetX+0.f, offsetY+0.f));

	paOkno->draw(*obrazok);
}


void Hrac::update(double delta) {
		if (smerPohybu == vpravo && hybeSa) {
			if (pohybDelta < 32) {
				animaciaVpravo->tick();
				pohybDelta++;
				posun(1, 0);
			}else {
				smerPohybu = SmerPohladu::stoji;
				pohybDelta = 0;
				polickoX++;
				hybeSa = false;
			}
		}

	if (smerPohladu == hore && hybeSa) {
		if (pohybDelta < 32) {
			animaciaHore->tick();
			pohybDelta++;
			posun(0, -1);
		}
		else {
			smerPohybu = SmerPohladu::stoji;
			pohybDelta = 0;
			polickoY--;
			hybeSa = false;
		}
	}

	if (smerPohladu == dole && hybeSa) {
		if (pohybDelta < 32) {
			animaciaDole->tick();
			pohybDelta++;
			posun(0, 1);
		}
		else {
			smerPohybu = SmerPohladu::stoji;
			pohybDelta = 0;
			polickoY++;
			hybeSa = false;
		}
	}

	if (smerPohladu == vlavo && hybeSa) {
		if (pohybDelta < 32) {
			animaciaVlavo->tick();
			pohybDelta++;
			posun(-1, 0);
		}
		else {
			smerPohybu = SmerPohladu::stoji;
			pohybDelta = 0;
			polickoX--;
			hybeSa = false;
		}
	}


}

bool Hrac::GethybeSa() {
	return hybeSa;
}

void Hrac::chodVpravo() {
	pohybDelta = 0;
	smerPohybu = SmerPohladu::vpravo;
	hybeSa = true;
}

void Hrac::chodVlavo() {
	pohybDelta = 0;
	smerPohybu = SmerPohladu::vlavo;
	hybeSa = true;
}

void Hrac::chodHore() {
	pohybDelta = 0;
	smerPohybu = SmerPohladu::hore;
	hybeSa = true;
}

void Hrac::chodDole() {
	pohybDelta = 0;
	smerPohybu = SmerPohladu::dole;
	hybeSa = true;
}

void Hrac::posun(int x, int y) {
	offsetX += x;
	offsetY += y;
}


void Hrac::zmenSmerPohladu(SmerPohladu paSmer) {
	smerPohladu = paSmer;
	
	if (smerPohladu == hore) {
		animaciaHore->reset();
	}

	if (smerPohladu == dole) {
		animaciaDole->reset();
	}

	if (smerPohladu == vlavo) {
		animaciaVlavo->reset();
	}

	if (smerPohladu == vpravo) {
		animaciaVpravo->reset();
	}

	
}

void Hrac::animaciaTick() {
	animaciaDole->tick();
	animaciaHore->tick();
	animaciaVlavo->tick();
	animaciaVpravo->tick();
}


int Hrac::GetoffsetX() {
	return offsetX;
}

int Hrac::GetoffsetY() {
	return offsetY;
}

int Hrac::GetpolickoX() {
	return polickoX;
}

int Hrac::GetpolickoY() {
	return polickoY;
}

void Hrac::setPolickoX(int paX) {
	polickoX = paX;
}

void Hrac::setPolickoY(int paY) {
	polickoY = paY;
}

void Hrac::setOffsetX(int paX) {
	offsetX = paX;
}

void Hrac::setOffsetY(int paY) {
	offsetY = paY;
}