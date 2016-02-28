#include <iostream>

#include "Predmet.h"



Predmet::Predmet(std::string meno, int typ, std::string paObrazok, int cena,int paUroven) {
	
	const std::string predmety_cesta = "Data/Grafika/Predmety/";
	
	this->sObrazok = paObrazok;
	this->uroven = paUroven;
	this->meno = meno;
	this->typ = typ;
	this->cena = cena;
	obrazok = new sf::Sprite();
	
	textura = new sf::Texture();
	if (!textura->loadFromFile(predmety_cesta + "" + paObrazok+".png", sf::IntRect(0, 0, 32, 32))) {
		
	}
	textura->setSmooth(true);
	obrazok->setTexture(*textura);
	
}



Predmet::~Predmet() {
	delete obrazok;
}


int Predmet::Geturoven() {
	return uroven;
}

int Predmet::Gettyp() {
	return typ;
}

int Predmet::Getcena() {

	return cena;
}


std::string Predmet::Getmeno() {

	return meno;
}


sf::Sprite* Predmet::Getobrazok() {

	return obrazok;
}


void Predmet::pouzi(Hrac* hrac) {

}


void Predmet::Setcena(int newVal) {

	cena = newVal;
}


void Predmet::Setmeno(std::string newVal) {

	meno = newVal;
}


void Predmet::Setobrazok(sf::Sprite* newVal) {

	obrazok = newVal;
}

void Predmet::Settyp(int newVal) {
	typ = newVal;
}

std::string Predmet::Getstringovytyp() {
	switch (typ)
	{
	case (1) :
		return "Helma";
	case (2) :
		return "Ramena";
	case (3) :
		return "Brnenie";
	case (4) :
		return "Nohavice";
	case (5) :
		return "Nahrdelnik";
	case (6) :
		return "Prsten";
	case (7) :
		return "Rukavice";
	case (8) :
		return "Topanky";
	case (9) :
		return "Zbran (1h)";
	case (10) :
		return "Zbran (2h)";
	case (11) :
		return "Stit";
	case(12) :
		return "Elixir";
		break;
	default:
		return "ine";
		break;
	}
}