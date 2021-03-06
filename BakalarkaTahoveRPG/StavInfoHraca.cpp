#include "StavInfoHraca.h"
#include "Loader.h"
#include "Hra.h"
#include "Hrac.h"
#include "Statistika.h"
#include "Zameranie.h"
#include "Pouzitelny.h"
#include "Zbran.h"
#include "Akcia.h"
#include "Tlacidlo.h"
#include "AkciaDmg.h"
#include "AkciaLiecenie.h"
#include "AkciaPoskodenieZbranou.h"
#include "AkciaPridanieEfektu.h"
#include "AudioManager.h"

StavInfoHraca::StavInfoHraca(std::string paNazov, sf::RenderWindow* paOkno, Hra* paHra) : Stav(paNazov, paOkno, paHra) {
	
	ukazovatel.setSize(sf::Vector2f(48, 48));
	ukazovatel.setFillColor(sf::Color(255, 0, 0, 128));

	oznacene = 1;

	hrac = nullptr;
	hracoveAkcie = nullptr;
	oblecene = nullptr;

	sf::Texture textura;
	textura.create(50,50);
	sf::Sprite* normalne = new sf::Sprite();
	sf::Sprite* zakliknute = new sf::Sprite(textura);
	zakliknute->setTextureRect(sf::IntRect(0, 0, 48, 48));
	zakliknute->setColor(sf::Color(255, 0, 0, 128));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			tlacidlaAkcie.push_back(new Tlacidlo(normalne, zakliknute, "",sf::Vector2f(500.f+ j*55 + 3, 100.f+ i*55+3),sf::Vector2f(48,48),font,35U));
		}
	}
	


}


StavInfoHraca::~StavInfoHraca() {
}



void StavInfoHraca::onEnter() {
	Stav::onEnter();


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		stlacenaKlavesa = true;
	}
	stlacenaMys = true;

	oznacene = 1;
	hrac = hra->Gethrac();
	oblecene = hrac->Getstatistika()->Getoblecene();
	hracoveAkcie = hrac->Getstatistika()->Getakcie();
	
}


void StavInfoHraca::onExit() {
	Stav::onExit();
}


void StavInfoHraca::render() {
		

		sf::Text text("Player info:",*font, 45U);
		text.setPosition(sf::Vector2f(10.f, 10.f));
		okno->draw(text);
		
		text.setCharacterSize(35U);
		text.setString(hrac->GetZameranie()->Getnazov());
		text.setPosition(sf::Vector2f(32.f, 56.f));
		okno->draw(text);

		int lvl = hrac->Getstatistika()->dajUroven();
		text.setCharacterSize(33U);
		text.setString("Level: " + std::to_string(lvl));
		text.setPosition(sf::Vector2f(32.f, 91.f));
		okno->draw(text);

		int xpTeraz = hrac->Getstatistika()->Getskusenosti();
		int xpNaDalsi = hrac->GetZameranie()->xpNaLevel(lvl+1);
		text.setCharacterSize(18U);
		text.setString("Experience: " + std::to_string(xpTeraz) + " / " + std::to_string(xpNaDalsi));
		text.setPosition(sf::Vector2f(15.f, 135.f));
		okno->draw(text);


		int hpTeraz = hrac->Getstatistika()->Gethp();
		int hpMax = hrac->Getstatistika()->GethpMax();
		text.setString("Hp: " + std::to_string(hpTeraz) + " / " + std::to_string(hpMax));
		text.setPosition(sf::Vector2f(15.f, 152.f));
		okno->draw(text);

		int mpTeraz = hrac->Getstatistika()->Getmp();
		int mpMax = hrac->Getstatistika()->GetmpMax();
		text.setString("Mp: " + std::to_string(mpTeraz) + " / " + std::to_string(mpMax));
		text.setPosition(sf::Vector2f(15.f, 170.f));
		okno->draw(text);

		int stat = hrac->Getstatistika()->Getsila();
		text.setString("Strength: " + std::to_string(stat));
		text.setPosition(sf::Vector2f(15.f, 187.f));
		okno->draw(text);

		stat = hrac->Getstatistika()->Getintelekt();
		text.setString("Intellect: " + std::to_string(stat));
		text.setPosition(sf::Vector2f(15.f, 204.f));
		okno->draw(text);

		stat = hrac->Getstatistika()->Getrychlost();
		text.setString("Speed: " + std::to_string(stat));
		text.setPosition(sf::Vector2f(15.f, 221.f));
		okno->draw(text);
		
		stat = hrac->Getstatistika()->Getobrana();
		text.setString("Armor: " + std::to_string(stat));
		text.setPosition(sf::Vector2f(15.f, 238.f));
		okno->draw(text);

		int pMin = hrac->Getstatistika()->Getminposkodenie();
		int pMax = hrac->Getstatistika()->Getmaxposkodenie();
		text.setString("Damage: " + std::to_string(pMin) + " / " + std::to_string(pMax));
		text.setPosition(sf::Vector2f(15.f, 255.f));
		okno->draw(text);

		text.setString("Attack speed: 1x per " + floattostring(hrac->Getstatistika()->Getrychlostutoku()/1000.f)+" s");
		text.setPosition(sf::Vector2f(15.f, 272.f));
		okno->draw(text);

		// vykreslovanie oblecenych veci

		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(48, 48));

		sf::Sprite* predmetObrazok;
		int x = 20;
		int y = 300;

		//helma
		rectangle.setPosition(x + 0.f, y + 0.f);
		okno->draw(rectangle);
		
		if(oblecene->count(1)){
			predmetObrazok = oblecene->at(1)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
			
		}
		//ramena
		rectangle.setPosition(x + 0.f, y + 60.f);
		okno->draw(rectangle);
		
		if (oblecene->count(2)) {
			predmetObrazok = oblecene->at(2)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//brnenie
		rectangle.setPosition(x+0.f, y + 120.f);
		okno->draw(rectangle);
		
		if (oblecene->count(3)) {
			predmetObrazok = oblecene->at(3)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//nohavice
		rectangle.setPosition(x+0.f, y + 180.f);
		okno->draw(rectangle);
		
		if (oblecene->count(4)) {
			predmetObrazok = oblecene->at(4)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//nahrdelnik
		rectangle.setPosition(x+200.f, y + 0.f);
		okno->draw(rectangle);
		if (oblecene->count(5)) {
			predmetObrazok = oblecene->at(5)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//prsten
		rectangle.setPosition(x + 200.f, y+60.f);
		okno->draw(rectangle);
		if (oblecene->count(6)) {
			predmetObrazok = oblecene->at(6)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//rukavice
		rectangle.setPosition(x + 200.f, y + 120.f);
		okno->draw(rectangle);
		if (oblecene->count(7)) {
			predmetObrazok = oblecene->at(7)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//topanky
		rectangle.setPosition(x + 200.f, y + 180.f);
		okno->draw(rectangle);
		if (oblecene->count(8)) {
			predmetObrazok = oblecene->at(8)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}

		//zbran v lavej
		rectangle.setPosition(x + 70.f, y + 180.f);
		okno->draw(rectangle);
		if (oblecene->count(9)) {
			predmetObrazok = oblecene->at(9)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}
		//zbran v pravej
		rectangle.setPosition(x + 130.f, y + 180.f);
		okno->draw(rectangle);
		if (oblecene->count(10)) {
			predmetObrazok = oblecene->at(10)->Getobrazok();
			predmetObrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			predmetObrazok->setPosition(rectangle.getPosition());
			okno->draw(*predmetObrazok);
		}


		// oznacenie a info predmetu
		switch (oznacene)
		{
		case (1) :
			ukazovatel.setPosition(x + 0.f, y + 0.f);
			okno->draw(ukazovatel);
			if(oblecene->count(1))  vykresliOknoPredmetu(oblecene->at(1), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (2) :
			ukazovatel.setPosition(x + 0.f, y + 60.f);
			okno->draw(ukazovatel);
			if (oblecene->count(2)) vykresliOknoPredmetu(oblecene->at(2), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (3) :
			ukazovatel.setPosition(x + 0.f, y + 120.f);
			okno->draw(ukazovatel);
			if (oblecene->count(3)) vykresliOknoPredmetu(oblecene->at(3), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (4) :
			ukazovatel.setPosition(x + 0.f, y + 180.f);
			okno->draw(ukazovatel);
			if (oblecene->count(4)) vykresliOknoPredmetu(oblecene->at(4), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (5) :
			ukazovatel.setPosition(x + 200.f, y + 0.f);
			okno->draw(ukazovatel);
			if (oblecene->count(5)) vykresliOknoPredmetu(oblecene->at(5), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (6) :
			ukazovatel.setPosition(x + 200.f, y + 60.f);
			okno->draw(ukazovatel);
			if (oblecene->count(6)) vykresliOknoPredmetu(oblecene->at(6), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (7) :
			ukazovatel.setPosition(x + 200.f, y + 120.f);
			okno->draw(ukazovatel);
			if (oblecene->count(7)) vykresliOknoPredmetu(oblecene->at(7), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (8) :
			ukazovatel.setPosition(x + 200.f, y + 180.f);
			okno->draw(ukazovatel);
			if (oblecene->count(8)) vykresliOknoPredmetu(oblecene->at(8), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (9) :
			ukazovatel.setPosition(x + 70.f, y + 180.f);
			okno->draw(ukazovatel);
			if (oblecene->count(9)) vykresliOknoPredmetu(oblecene->at(9), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		case (10) :
			ukazovatel.setPosition(x + 130.f, y + 180.f);
			okno->draw(ukazovatel);
			if (oblecene->count(10)) vykresliOknoPredmetu(oblecene->at(10), ukazovatel.getPosition().x, ukazovatel.getPosition().y, okno);
			break;
		default:
			break;
		}

	
		//vykreslenie akcii , ktor� hr�� mo�e robi� pri boji		

		float kuzlaStartX = 500;
		float kuzlaStartY = 100;
		int nasirku = 9;
		int riadok = 0;
		
		text.setString("Spells:");
		text.setCharacterSize(30);

		text.setPosition(sf::Vector2f(kuzlaStartX, kuzlaStartY - 40));
		okno->draw(text);


		sf::RectangleShape pozadieSpellov;
		pozadieSpellov.setSize(sf::Vector2f(nasirku*55.f, 165.f + 3));
		pozadieSpellov.setFillColor(sf::Color::White);
		pozadieSpellov.setPosition(sf::Vector2f(kuzlaStartX,kuzlaStartY));
		okno->draw(pozadieSpellov);

	
		for (unsigned int i = 0; i < static_cast<unsigned int>(hracoveAkcie->size()); i++)
		{
			if (i >= static_cast<unsigned int>(riadok + 1)*nasirku) {
				riadok++;
			}
			Akcia* akcia = hracoveAkcie->at(i);
			sf::Sprite* obrazok = akcia->Getobrazok();
			obrazok->setScale(sf::Vector2f(1.5f, 1.5f));
			obrazok->setPosition(sf::Vector2f(kuzlaStartX+ (i- riadok*nasirku) * 55 + 3, kuzlaStartY+ riadok*55 +3));
			okno->draw(*obrazok);

			okno->draw(*tlacidlaAkcie.at(i)->Getsprite());
			
		}

		sf::RectangleShape pozadieInfo;
		pozadieInfo.setSize(sf::Vector2f(nasirku * 55.f, 200.f));
		pozadieInfo.setFillColor(sf::Color::White);
		pozadieInfo.setPosition(sf::Vector2f(kuzlaStartX, kuzlaStartY+200));
		okno->draw(pozadieInfo);

		for (unsigned int i = 0; i < static_cast<unsigned int>(hracoveAkcie->size()); i++)
		{
			if (tlacidlaAkcie.at(i)->Getzakliknute()) {
				
				Akcia* akcia = hracoveAkcie->at(i);

				sf::Sprite* obrazok = akcia->Getobrazok();
				obrazok->setScale(sf::Vector2f(1.0f, 1.0f));
				obrazok->setPosition(sf::Vector2f(pozadieInfo.getPosition().x + pozadieInfo.getSize().x - obrazok->getTextureRect().width -5 , pozadieInfo.getPosition().y + 5));
				okno->draw(*obrazok);

				text.setColor(sf::Color::Black);
				std::string info = akcia->Getmeno();
				info += "\n" + akcia->dajPopis()+"\n";

				if (dynamic_cast<AkciaDmg*>(akcia) != nullptr) {
					AkciaDmg* dmgakcia = static_cast<AkciaDmg*>(akcia);
					info += "Damage: " + std::to_string(dmgakcia->minPoskodenie(hrac->Getstatistika())) + " - " + std::to_string(dmgakcia->maxPoskodenie(hrac->Getstatistika())) + "\n";
				}

				if (dynamic_cast<AkciaPoskodenieZbranou*>(akcia) != nullptr) {
					AkciaPoskodenieZbranou* dmgakcia = static_cast<AkciaPoskodenieZbranou*>(akcia);
					info += "Damage: " + std::to_string(dmgakcia->minPoskodenie()) + " - " + std::to_string(dmgakcia->maxPoskodenie()) + "\n";
				}

				if (dynamic_cast<AkciaLiecenie*>(akcia) != nullptr) {
					AkciaLiecenie* liecenieAkcia = static_cast<AkciaLiecenie*>(akcia);
					info += "Healing: " + std::to_string(liecenieAkcia->minLiecenie(hrac->Getstatistika())) + " - " + std::to_string(liecenieAkcia->maxLiecenie(hrac->Getstatistika())) + "\n";
				}

				if (dynamic_cast<AkciaPridanieEfektu*>(akcia) != nullptr) {
					info += "Last: " + floattostring(akcia->Gettrvanie()/1000.f) + " s\n";
				}

				if (akcia->Getcenamany() != 0) {
					info += "Mana needed: " + std::to_string(hracoveAkcie->at(i)->Getcenamany())+ "\n";
				}

				if (akcia->GetcasCastenia() != 0) {
					info += "Casting time: " + floattostring(akcia->GetcasCastenia() / 1000.f) + "s\n";
				}
				if (akcia->Getcooldown() != 0) {
					info += "Cooldown: " + floattostring(akcia->Getcooldown() / 1000.f) + " s\n";
				}

				text.setCharacterSize(17);
				text.setString(info);
				text.setPosition(sf::Vector2f(kuzlaStartX + 5, kuzlaStartY + 205));
				okno->draw(text);
			}
		}

		Stav::render();
}


void StavInfoHraca::update() {

	if (hra->maFocus()) {

		Stav::update();
		if (stav == StavAkcia::NORMAL) {

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && stlacenaMys == false)
			{
				sf::Vector2i pozicia = sf::Mouse::getPosition(*okno);
				for (unsigned int i = 0; i < hracoveAkcie->size(); i++)
				{
					tlacidlaAkcie[i]->skontrolujKlik(pozicia);
					//zrusenie zakliknutia ostatnych
					if (tlacidlaAkcie[i]->Getzakliknute()) {
						AudioManager::Instance()->playEfekt("klik");
						for (unsigned int j = 0; j < static_cast<unsigned int>(hracoveAkcie->size()); j++)
						{
							if (i != j) {
								tlacidlaAkcie.at(j)->Setzakliknute(false);
							}
						}
					}

				}
				stlacenaMys = true;
			}

			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && stlacenaMys == true) {
				stlacenaMys = false;
			}



			if (!stlacenaKlavesa && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				stlacenaKlavesa = true;
				if (oznacene < 10) {
					oznacene++;
				}
				else {
					oznacene = 1;
				}

			}

			if (!stlacenaKlavesa && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				stlacenaKlavesa = true;
				if (oznacene > 1) {
					oznacene--;
				}
				else {
					oznacene = 10;
				}
			}

			if (!stlacenaKlavesa && sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				stlacenaKlavesa = true;
				if (oblecene->count(oznacene)) {
					oblecene->at(oznacene)->pouzi(hrac);
				}
			}

			if (!stlacenaKlavesa && (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::C))) {
				hra->zmenStavRozhrania("hranieHry");
			}

			if (stlacenaKlavesa
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::C)
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
				&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				stlacenaKlavesa = false;
			}
		}
		
	}
}


void StavInfoHraca::vykresliOknoPredmetu(Predmet*predmet, float x, float y, sf::RenderWindow* okno) const
{

	// vykreslenie obdlznika na ktorom sa bude vypisovat info predmetu 
	sf::RectangleShape obdlznik;
	obdlznik.setSize(sf::Vector2f(230.f, 195.f));// vyska a sirka okienka
	obdlznik.setOutlineColor(sf::Color::Red);
	obdlznik.setOutlineThickness(2);


	float posX, posY;
	posX = x + 30.f;
	posY = y +20.f;

	obdlznik.setPosition(sf::Vector2f(posX, posY));
	// tu u� je samotny vypis infa
	sf::Text text(predmet->Getmeno(), *font, 18U);//vypisanie mena
	if (text.getGlobalBounds().width > obdlznik.getGlobalBounds().width - 5) {
		obdlznik.setSize(sf::Vector2f(text.getGlobalBounds().width + 20, 200));
	}
	
	okno->draw(obdlznik);

	text.setColor(sf::Color::Black);

	if (hrac->Getstatistika()->dajUroven() < predmet->Geturoven()) text.setColor(sf::Color::Red);

	text.setPosition(sf::Vector2f(posX + 10.f, posY + 5.f));
	okno->draw(text);


	text.setCharacterSize(14U);
	text.setString(predmet->Getstringovytyp());
	text.setPosition(sf::Vector2f(posX + 5.f, posY + 23.f));
	okno->draw(text);


	text.setString("Level needed: " + std::to_string(predmet->Geturoven()));
	text.setPosition(sf::Vector2f(posX + 5.f, posY + 35.f));
	okno->draw(text);

		text.setColor(sf::Color::Black);
		text.setCharacterSize(14U);
		Pouzitelny* pouzitelny = static_cast<Pouzitelny*>(predmet);
		std::string bonusy = "";

		if (dynamic_cast<Zbran*>(predmet) != nullptr)
		{
			Zbran* pom = static_cast<Zbran*>(predmet);
			if (pom->Gettyp() != 11) {
				bonusy += "Damage: ";
				bonusy += std::to_string(pom->Getminposkodenie()) + " - ";
				bonusy += std::to_string(pom->Getmaxposkodenie());
				bonusy += "\n";
				bonusy += "Attack speed: " + std::to_string(pom->GetrychlostUtoku()) + " ms\n";
			}
		}


		if (pouzitelny->Getarmor() != 0) {
			bonusy += "Armor: ";
			bonusy += std::to_string(pouzitelny->Getarmor());
			bonusy += "\n";
		}

		if (pouzitelny->Gethp() != 0) {
			bonusy += "Hp: ";
			bonusy += std::to_string(pouzitelny->Gethp());
			bonusy += "\n";
		}

		if (pouzitelny->Getmp() != 0) {
			bonusy += "Mp: ";
			bonusy += std::to_string(pouzitelny->Getmp());
			bonusy += "\n";
		}

		if (pouzitelny->Getsila() != 0) {
			bonusy += "Strength: ";
			bonusy += std::to_string(pouzitelny->Getsila());
			bonusy += "\n";
		}

		if (pouzitelny->Getrychlost() != 0) {
			bonusy += "Speed: ";
			bonusy += std::to_string(pouzitelny->Getrychlost());
			bonusy += "\n";
		}

		if (pouzitelny->Getinteligencia() != 0) {
			bonusy += "Intellect: ";
			bonusy += std::to_string(pouzitelny->Getinteligencia());
			bonusy += "\n";
		}

		if (pouzitelny->GetarmorMult() != 0) {
			bonusy += "Armor: ";
			bonusy += std::to_string(static_cast<int>(round(pouzitelny->GetarmorMult() * 100)));
			bonusy += " %\n";
		}

		if (pouzitelny->GethpMult() != 0) {
			bonusy += "Hp: ";
			bonusy += std::to_string(static_cast<int>(round(pouzitelny->GethpMult() * 100)));
			bonusy += " %\n";
		}

		if (pouzitelny->GetmpMult() != 0) {
			bonusy += "Mp: ";
			bonusy += std::to_string(static_cast<int>(round(pouzitelny->GetmpMult() * 100)));
			bonusy += " %\n";
		}

		if (pouzitelny->GetsilaMult() != 0) {
			bonusy += "Strength: ";
			bonusy += std::to_string(static_cast<int>(round(pouzitelny->GetsilaMult() * 100)));
			bonusy += " %\n";
		}

		if (pouzitelny->GetrychlostMult() != 0) {
			bonusy += "Speed: ";
			bonusy += std::to_string(static_cast<int>(round(pouzitelny->GetrychlostMult() * 100)));
			bonusy += " %\n";
		}

		if (pouzitelny->GetinteligenciaMult() != 0) {
			bonusy += "Intellect: ";
			bonusy += std::to_string(static_cast<int>(round(pouzitelny->GetinteligenciaMult() * 100)));
			bonusy += " %\n";
		}


		text.setPosition(sf::Vector2f(posX + 5.f, posY + 48.f));
		text.setString(bonusy);
		okno->draw(text);

	
}
