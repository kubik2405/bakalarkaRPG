#include "StavVolbaZamerania.h"
#include <string>
#include "Loader.h"
#include "Hra.h"
#include "Hrac.h"
#include "Zameranie.h"
#include "StavHranieHry.h"
#include "Mapa.h"
#include "Akcia.h"



StavVolbaZamerania::StavVolbaZamerania(std::string paNazov, sf::RenderWindow* paOkno,Hra* paHra) : Stav(paNazov,paOkno,paHra) {
	font = Loader::Instance()->nacitajFont("font2.otf");
	oznacene = 1;
	std::cout << "Volba zamerania vytvorena" << std::endl;
}



StavVolbaZamerania::~StavVolbaZamerania() {
	std::cout << "Volba zamerania zmazana" << std::endl;
}





void StavVolbaZamerania::onEnter() {
	Stav::onEnter();
    stlacenaKlavesa = true;
	
}


void StavVolbaZamerania::onExit() {
	Stav::onExit();
}


void StavVolbaZamerania::render() {
	
	int i = 60;
	sf::RectangleShape rectangle;
	
	rectangle.setSize(sf::Vector2f(20.f+okno->getSize().x / 4, okno->getSize().y -40.f));
	rectangle.setFillColor(sf::Color::Yellow);
	rectangle.setPosition(  i*oznacene -10.f + (oznacene-1)*(okno->getSize().x / 4.f)  , 30-10.f);
	okno->draw(rectangle);
	
	rectangle.setFillColor(sf::Color::White);
	rectangle.setSize(sf::Vector2f(okno->getSize().x/4.f, okno->getSize().y /2.f));
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition((float)i, 30.f);
	okno->draw(rectangle);

	rectangle.setSize(sf::Vector2f(okno->getSize().x / 4.f, okno->getSize().y / 2.f));
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(i*2 + okno->getSize().x / 4.f, 30.f);
	okno->draw(rectangle);
	
	rectangle.setSize(sf::Vector2f(okno->getSize().x / 4.f, okno->getSize().y / 2.f));
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition(i*3 + 2*okno->getSize().x / 4.f, 30.f);
	okno->draw(rectangle);
}


void StavVolbaZamerania::update(double delta) {
	if (hra->maFocus()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !stlacenaKlavesa) {
			stlacenaKlavesa = true;
			if (oznacene > 1) {
				oznacene--;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !stlacenaKlavesa) {
			stlacenaKlavesa = true;
			if (oznacene < 3) {
				oznacene++;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !stlacenaKlavesa) {
			stlacenaKlavesa = true;
			hra->zmenStavRozhrania("hlavneMenu");
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !stlacenaKlavesa) {
			stlacenaKlavesa = true;
			Zameranie* zameranie = new Zameranie("Warrior", 10, 3, 3, 2, 1);

			zameranie->vlozAkciu(new Akcia("Liecenie", "healing1", 0, 1000, 0, "Vylieci ta!"), 2);
			zameranie->vlozAkciu(new Akcia("Utok", "attack", 0, 1000, 0, "Bezny utok zbranou"), 1);

			Hrac* hrac = new Hrac(zameranie);
			hra->SetHrac(hrac);

			Loader* loader = Loader::Instance();
			loader->nacitajMapu("start", 0, 2, 2);
			hra->zmenStavRozhrania("hranieHry");

		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			stlacenaKlavesa = false;
		}
	}
}