#include "StavHlavneMenu.h"
#include "Loader.h"
#include "Hra.h"
#include "Tlacidlo.h"
#include "PopupOkno.h"
#include "AudioManager.h"


StavHlavneMenu::StavHlavneMenu(std::string paNazov, sf::RenderWindow* paOkno,Hra* paHra): Stav(paNazov,paOkno,paHra){

	opening.init("opening.ogg", true,50);
	
	if (!pozadieTextura.loadFromFile("./Data/Grafika/uvod.jpg", sf::IntRect(0, 0, okno->getSize().x, okno->getSize().y)))
	{
		
	}
	pozadie.setTexture(pozadieTextura);

	sf::Texture texture;
	texture.create(1000, 1000);

	sf::Sprite* normalne = new sf::Sprite();
	normalne->setTexture(texture);
	normalne->setTextureRect(sf::IntRect(0, 0, 48, 48));
	normalne->setColor(sf::Color(0, 0, 0, 255));



	tlacidla.push_back(new Tlacidlo(normalne, normalne, "New game", sf::Vector2f(0, 150), sf::Vector2f(500, 0), font, 85));
	tlacidla.push_back(new Tlacidlo(normalne, normalne, "Load save", sf::Vector2f(0, 260), sf::Vector2f(500, 0), font,85));
	tlacidla.push_back(new Tlacidlo(normalne, normalne, "Credits", sf::Vector2f(0, 350), sf::Vector2f(500, 0), font, 85));
	tlacidla.push_back(new Tlacidlo(normalne, normalne, "Exit", sf::Vector2f(0, 440), sf::Vector2f(500, 0), font, 85));
	
	for (unsigned int i = 0; i < tlacidla.size(); i++) {
		Tlacidlo* t = tlacidla.at(i);
		t->setSize(sf::Vector2f(t->Gettext().getGlobalBounds().width, t->Gettext().getGlobalBounds().height));
		t->Setpozicia(sf::Vector2f(paOkno->getSize().x / 2 - t->getSize().x/2,t->Gettext().getPosition().y));
	}

	stlacenaMys = true;
	oznacene = 0;

	
}


StavHlavneMenu::~StavHlavneMenu() {
	for each (Tlacidlo* tlacidlo in tlacidla)
	{
		delete tlacidlo;
	}
}



void StavHlavneMenu::onEnter() {
	Stav::onEnter();
	stlacenaMys = true;
	opening.play();
}


void StavHlavneMenu::onExit(){
	Stav::onExit();
	opening.stop();
}


void StavHlavneMenu::render() {

	okno->draw(pozadie);

	for (unsigned int i = 0; i < tlacidla.size(); i++)
	{
		sf::RectangleShape r = tlacidla.at(i)->Getramcek();
		r.setOutlineColor(sf::Color::Black);
		r.setOutlineThickness(5.f);
		r.setSize(sf::Vector2f(r.getSize().x + 10.f, r.getSize().y + 10.f));
		r.setPosition(r.getPosition().x - 5.f, r.getPosition().y - 5.f);
		okno->draw(r);
		sf::Text text = tlacidla.at(i)->Gettext();
		text.setPosition(text.getPosition().x, text.getPosition().y - text.getCharacterSize()/2 +3);
		if(oznacene == i){
			text.setColor(sf::Color::Red);
		}
		else {
			text.setColor(sf::Color::Black);
		}
		okno->draw(text);
	}
	
	Stav::render();
}


void StavHlavneMenu::update() {
	
	if (hra->maFocus()) {

		Stav::update();

		if (stav == StavAkcia::NORMAL) {

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && stlacenaMys == false)
			{
				sf::Vector2i pozicia = sf::Mouse::getPosition(*okno);
				for (unsigned int i = 0; i < tlacidla.size(); i++)
				{
					tlacidla[i]->skontrolujKlik(pozicia);
					if (tlacidla[i]->Getzakliknute()) {
						stlacenaMys = true;
						AudioManager::Instance()->playEfekt("klik");
						tlacidla[i]->Setzakliknute(false);
						if (i == 0) {
							hra->zmenStavRozhrania("volbaZamerania");
						}else if (i == 1) {
							try {
								Loader::Instance()->load();
							}
							catch (...) {
								zobrazPopup(new PopupOkno("Error! Save is corupted or isn't exist."));
								AudioManager::Instance()->playEfekt("beep");
							}
						}else if(i == 2)
						{
							hra->zmenStavRozhrania("stavCredits");
						}else {
							okno->close();
						}

					}
				}
			}

			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && stlacenaMys == true) {
				stlacenaMys = false;
			}

			sf::Vector2i pozicia = sf::Mouse::getPosition(*okno);
			for (unsigned int i = 0; i < tlacidla.size(); i++)
			{
				if (tlacidla[i]->hover(pozicia))
				{
					oznacene = i;
				}
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !stlacenaKlavesa) {
				stlacenaKlavesa = true;
				if (oznacene > 0) {
					oznacene--;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !stlacenaKlavesa) {
				stlacenaKlavesa = true;
				if (oznacene < static_cast<signed int>(tlacidla.size()) - 1) {
					oznacene++;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !stlacenaKlavesa) {
				stlacenaKlavesa = true;
				if (oznacene == 0) {
					AudioManager::Instance()->playEfekt("klik");
					hra->zmenStavRozhrania("volbaZamerania");
				}else if(oznacene == 1)
				{
					try {
						AudioManager::Instance()->playEfekt("klik");
						Loader::Instance()->load();
					}
					catch (...) {
						zobrazPopup(new PopupOkno("Error! Save is corupted or isn't exist."));
						AudioManager::Instance()->playEfekt("beep");
					}
				}else if(oznacene == 2)
				{
					hra->zmenStavRozhrania("stavCredits");
				}else
				{
					okno->close();
				}
			}
			
	


			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				stlacenaKlavesa = false;
			}
		}
	}

}