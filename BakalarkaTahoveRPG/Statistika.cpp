#include "Statistika.h"
#include <map>
#include <math.h>
#include "Predmet.h"
#include "Pouzitelny.h"
#include "Zbran.h"
#include "Akcia.h"
#include "Efekt.h"
#include "AkciaPridanieEfektu.h"

Statistika::Statistika() {
	akcie = new vector<Akcia*>();
	uroven = 1;
	hp = 80;
	hpMax = 80;
	mp = 0;
	mpMax = 30;
	sila = 1;
	intelekt = 1;
	obrana = 1;
	rychlost = 1;
	obrana = 0;
	skusenosti = 0;
	oblecene = new map<int, Predmet*>();
	aktivneEfekty = new map<Efekt*, sf::Time>();
	prepocitajPoskodenia();
	timerMp.reset();
	combat = false;


}

Statistika::Statistika(int paUroven, int paHp, int paHpMax, int paMp, int paMpMax, int paSila, int paIntelekt, int paRychlost, int paObrana,map<int, Predmet*>* paObleceneVeci) {
	
	skusenosti = 20;
	uroven = paUroven;
	hp = paHp;
	hpMax = paHpMax;
	mp = paMp;
	mpMax = paMpMax;
	sila = paSila;
	intelekt = paIntelekt;
	rychlost = paRychlost;
	obrana = paObrana;
	oblecene = paObleceneVeci;
	akcie = new vector<Akcia*>();
	aktivneEfekty = new map<Efekt*, sf::Time>();
	this->prepocitajPoskodenia();
	timerMp.reset();
	combat = false;
}


Statistika::~Statistika() {
	delete oblecene;
}


int Statistika::dajUroven() const
{
	return uroven;
}


int Statistika::Gethp() const
{
	return hp;
}


int Statistika::GethpMax() const
{
	int bonus = 0;
	double mult = 0;

	typedef map<int, Predmet*>::iterator it_t;

	for (it_t it = oblecene->begin(); it != oblecene->end(); ++it)
	{
		Pouzitelny* p = static_cast<Pouzitelny*>(it->second);
		bonus += p->Gethp();
		mult += p->GethpMult();
	}
	return static_cast<int>(round((hpMax+bonus)*(1+mult)));
}


int Statistika::Getintelekt() const
{

	int bonus = 0;
	double mult = 0;

	typedef map<int, Predmet*>::iterator it_t;

	for (it_t it = oblecene->begin(); it != oblecene->end(); ++it)
	{
		Pouzitelny* p = static_cast<Pouzitelny*>(it->second);
		bonus += p->Getinteligencia();
		mult += p->GetinteligenciaMult();
	}
	return static_cast<int>(round((intelekt + bonus)*(1 + mult)));
}


int Statistika::Getmp() {
	if (!combat) {
		timerMp.pause();
		int pocetObnov = timerMp.getElapsedTime().asMilliseconds() / 10000;
		int obnova = static_cast<int>(round(0.5*intelekt));

		zvysStat(pocetObnov* obnova, "mp");

		int doDalsej = timerMp.getElapsedTime().asMilliseconds() - pocetObnov * 10000;
		timerMp.reset(false);
		timerMp.add(sf::milliseconds(doDalsej));
		timerMp.resume();
	}
	return mp;
}

int Statistika::GetmpMax() const
{

	int bonus = 0;
	double mult = 0;

	typedef map<int, Predmet*>::iterator it_t;

	for (it_t it = oblecene->begin(); it != oblecene->end(); ++it)
	{
		Pouzitelny* p = static_cast<Pouzitelny*>(it->second);
		bonus += p->Getmp();
		mult += p->GetmpMult();
	}
	return static_cast<int>(round((mpMax + bonus)*(1 + mult)));
}



int Statistika::Getobrana() const
{

	int bonus = 0;
	double mult = 0;

	typedef map<int, Predmet*>::iterator it_t;

	for (it_t it = oblecene->begin(); it != oblecene->end(); ++it)
	{
		Pouzitelny* p = static_cast<Pouzitelny*>(it->second);
		bonus += p->Getarmor();
		mult += p->GetarmorMult();
	}
	return static_cast<int>(round((obrana + bonus)*(1 + mult)));
}


int Statistika::Getrychlost() const
{

	int bonus = 0;
	double mult = 0;

	typedef map<int, Predmet*>::iterator it_t;

	for (it_t it = oblecene->begin(); it != oblecene->end(); ++it)
	{
		Pouzitelny* p = static_cast<Pouzitelny*>(it->second);
		bonus += p->Getrychlost();
		mult += p->GetrychlostMult();
	}
	return static_cast<int>(round((rychlost + bonus)*(1 + mult)));
}


int Statistika::Getsila() const
{

	int bonus = 0;
	double mult = 0;

	typedef map<int, Predmet*>::iterator it_t;

	for (it_t it = oblecene->begin(); it != oblecene->end(); ++it)
	{
		Pouzitelny* p = static_cast<Pouzitelny*>(it->second);
		bonus += p->Getsila();
		mult += p->GetsilaMult();
	}
	return static_cast<int>(round((sila + bonus)*(1 + mult)));
}


int Statistika::Getskusenosti() const
{

	return skusenosti;
}


void Statistika::Sethp(int novaHodnota) {

	hp = novaHodnota;
}


void Statistika::SethpMax(int novaHodnota) {

	hpMax = novaHodnota;
}


void Statistika::Setintelekt(int novaHodnota) {

	intelekt = novaHodnota;
}


void Statistika::Setmp(int novaHodnota) {

	mp = novaHodnota;
}


void Statistika::Setobrana(int novaHodnota) {

	obrana = novaHodnota;
}


void Statistika::Setrychlost(int novaHodnota) {

	rychlost = novaHodnota;
}


void Statistika::Setsila(int novaHodnota) {

	sila = novaHodnota;
	prepocitajPoskodenia();
}


void Statistika::Setskusenosti(int novaHodnota) {
	skusenosti = novaHodnota;
}

void Statistika::pridajXp(int pocet) {
	skusenosti += pocet;
}

void Statistika::setCombat(bool paNa) {
	combat = paNa;
	if (paNa) {
		timerMp.reset(false);
	}
	else {
		timerMp.reset(true);
	}
}

void Statistika::zvysStat(int kolko, string paCo) {

	//hp
	if (paCo == "hp") {
		if (hp + kolko > GethpMax()) {
			hp = GethpMax();
		}
		else {
			hp += kolko;
		}
	}

	//hpMax
	if (paCo == "hpMax") {
		hpMax += kolko;
		if (kolko >= 0) {
			hp += kolko;
		}
		else {
			if (hp >= hpMax) {
				hp = hpMax;
			}
		}
	}

	//mp
	if (paCo == "mp") {
		if (mp + kolko > GetmpMax()) {
			mp = GetmpMax();
		}
		else {
			mp += kolko;
		}
	}

	//mpMax
	if (paCo == "mpMax") {
		mpMax += kolko;
		mp += kolko;
	}

	//sila
	if (paCo == "sila") {
		sila += kolko;
	}

	//intel
	if (paCo == "intel") {
		intelekt += kolko;
	}

	//rychlost
	if (paCo == "rychlost") {
		rychlost += kolko;
	}

	//obrana
	if (paCo == "obrana") {
		obrana += kolko;
	}
	prepocitajPoskodenia();
}

int Statistika::Getstat(string paCo) const
{

	//hp
	if (paCo == "hp") {
		return hp;
	}

	//hpMax
	if (paCo == "hpMax") {
		return hpMax;
	}

	//mp
	if (paCo == "mp") {
		return mp;
	}

	//mpMax
	if (paCo == "mpMax") {
		return mpMax;
	}

	//sila
	if (paCo == "sila") {
		return sila;
	}

	//intel
	if (paCo == "intel") {
		return intelekt;
	}

	//rychlost
	if (paCo == "rychlost") {
		return rychlost;
	}

	//obrana
	if (paCo == "obrana") {
		return obrana;
	}
	return 0;
}

void Statistika::setUroven(int paUroven) {
	uroven = paUroven;
}

map<int, Predmet*>* Statistika::Getoblecene() const
{
	return oblecene;
}

vector<Akcia*>* Statistika::Getakcie() const
{
	return akcie;
}

void Statistika::prepocitajPoskodenia() {
	if (oblecene->count(9)) {
		Zbran* zbran1 =static_cast<Zbran*>(oblecene->at(9));
		if (oblecene->count(10)) {
			Zbran* zbran2 = static_cast<Zbran*>(oblecene->at(10));
			if (zbran2->Gettyp() != 11) {
				int sucet = static_cast<int>(floor(zbran1->Getminposkodenie() + 0.75*zbran2->Getminposkodenie()));
				
				minPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1)) * sucet;
				sucet = static_cast<int>(floor(zbran1->Getmaxposkodenie() + 0.75*zbran2->Getmaxposkodenie()));
				maxPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1)) * sucet;
			}
			else {
				minPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1)) * zbran1->Getminposkodenie();
				maxPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1)) * zbran1->Getmaxposkodenie();
			}
		}
		else {
			minPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1)) * zbran1->Getminposkodenie();
			maxPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1)) * zbran1->Getmaxposkodenie();
		}
	}
	else {
		
		minPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1));
		maxPoskodenie = static_cast<int>(floor(2 * sila / 10 + 1));
	}
}

int Statistika::Getminposkodenie() const
{
	return minPoskodenie;
}
int Statistika::Getmaxposkodenie() const
{
	return maxPoskodenie;
}

void Statistika::vlozAkciu(Akcia* paAkcia) {

	if (dynamic_cast<AkciaPridanieEfektu *>(paAkcia) != nullptr) {
		AkciaPridanieEfektu* akc = static_cast<AkciaPridanieEfektu*>(paAkcia);
		akc->setStatistika(this);
	}

	int pocet = count(akcie->begin(), akcie->end(), paAkcia);
	if (pocet == 0) {
		akcie->push_back(paAkcia);
	}
		
}

int Statistika::Getrychlostutoku() const
{
	if (oblecene->count(9)) {
		Zbran* zbran1 = static_cast<Zbran*>(oblecene->at(9));
		if (oblecene->count(10)) {
			Zbran* zbran2 = static_cast<Zbran*>(oblecene->at(10));
			if (zbran2->Gettyp() == 9) {
				return (zbran1->GetrychlostUtoku() + zbran2->GetrychlostUtoku())/2;
			}
			else {
				return zbran1->GetrychlostUtoku();
			}
		}
		else {
			return zbran1->GetrychlostUtoku();
		}
	}
	else {
		return 2000;
	}
}

double Statistika::Getsancanauhyb() const
{
	if (static_cast<double>(rychlost) / (6 * uroven) > 0.50) return 0.50;
	return static_cast<double>(rychlost) / (6 * uroven);
}

double Statistika::Getodolnostvociposkodeniu() const
{
	return static_cast<double>(obrana) / (25 * uroven);
}

map<Efekt*, sf::Time>* Statistika::Getaktivneefekty() const
{
	return aktivneEfekty;
}

void Statistika::pridajEfekt(Efekt* paEfekt, sf::Time aktivnyDo) {
	if (aktivneEfekty->count(paEfekt)) {
		aktivneEfekty->at(paEfekt) = aktivnyDo;
	}
	else {
		aktivneEfekty->insert(pair<Efekt*, sf::Time>(paEfekt, aktivnyDo));
		paEfekt->aplikujSa(this);
	}

}
void Statistika::zrusEfekt(Efekt* paEfekt) {
	paEfekt->zrusUcinok(this);
	aktivneEfekty->erase(paEfekt);
}