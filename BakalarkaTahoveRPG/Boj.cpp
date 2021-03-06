#include "Boj.h"
#include "Nepriatel.h"
#include "Hrac.h"
#include "Akcia.h"
#include "Loader.h"
#include "Hra.h"
#include "Mapa.h"
#include "StavHranieHry.h"
#include "PopupOkno.h"
#include "Inventar.h"
#include "Efekt.h"
#include "QuestManager.h"
#include "Policko.h"
#include "Generator.h"
#include "Quest.h"

#include <map>
#include <random>
#include <deque>
#include "AudioManager.h"



Boj::Boj(Hrac* paHrac, Nepriatel* paNpc) {
	hrac = paHrac;
	npc = paNpc;
	vyber = true;
	hracAkcia = nullptr;
	npcAkcia = nullptr;
	hracCasVykonaniaAkcie = 0;
	npcCasVykonaniaAkcie = 0;
	koniec = true;
	boloVyhodnotenie = true;
}



Boj::~Boj() {

}

void Boj::bojStart() {
	casovac.reset(true);
	casovac.add(sf::milliseconds(1));
	koniec = false;
	boloVyhodnotenie = false;
	hrac->Getstatistika()->setCombat(true);
	npc->Getstatistika()->setCombat(true);
}

Nepriatel* Boj::Getnepriatel() const
{
	return npc;
}

bool Boj::koniecBoja() const
{
	return koniec;
}

Statistika* Boj::Gethracovastatistika() const
{
	return hrac->Getstatistika();
}

Statistika* Boj::Getnpcstatistika() const
{
	return npc->Getstatistika();
}

bool Boj::cakaNaVybratieAkcie() const
{
	return vyber;
}

sf::Time Boj::Getcasvboji() const
{
	return casovac.getElapsedTime();
}

void Boj::hracVybralAkciu(Akcia* paAkcia) {
	vyber = false;
	hracAkcia = paAkcia;
	hracCasVykonaniaAkcie = casovac.getElapsedTime().asMilliseconds() + hracAkcia->GetcasCastenia();
	casovac.resume();
}

float Boj::castBarProgres() const
{
	if (hracAkcia == nullptr) {
		return -1;
	}
	else {
		if (hracAkcia->GetcasCastenia() != 0) {
			return  (float)(casovac.getElapsedTime().asMilliseconds() - (hracCasVykonaniaAkcie - hracAkcia->GetcasCastenia())) / hracAkcia->GetcasCastenia();
		}
		else return -1;
	}
}

bool Boj::maAkciaCooldown(Akcia* paAkcia) {
	if (!vyber) {
		return true;
	}
	else {
		if (paAkcia->Getcenamany() > hrac->Getstatistika()->Getmp()) {
			return true;
		}
		map<Akcia*, int>::iterator it = hracCooldowny.find(paAkcia);
		if (it != hracCooldowny.end())
		{
			return true;
		}
	}
	return false;
}

void Boj::update() {
	if (!koniec) {
		casovac.pause();

		//bonusy hraca


		vector<Efekt* >trebaZmazatEfekty;

		map<Efekt*, sf::Time>* aktivneEfekty = hrac->Getstatistika()->Getaktivneefekty();
		for (map<Efekt*, sf::Time>::iterator it = aktivneEfekty->begin(); it != aktivneEfekty->end(); ++it)
		{
			if (it->second <= casovac.getElapsedTime()) {
				trebaZmazatEfekty.push_back(it->first);
			}
		}

		for (auto e : trebaZmazatEfekty) {
			hrac->Getstatistika()->zrusEfekt(e);
			logBoja.push_front("Player: effect fade off\n" + e->popis());
		}

		trebaZmazatEfekty.clear();



		//bonusy npc
		aktivneEfekty = npc->Getstatistika()->Getaktivneefekty();

		for (map<Efekt*, sf::Time>::iterator it = aktivneEfekty->begin(); it != aktivneEfekty->end(); ++it)
		{
			if (it->second <= casovac.getElapsedTime()) {
				trebaZmazatEfekty.push_back(it->first);	
			}
		}

		for (auto e : trebaZmazatEfekty) {
			npc->Getstatistika()->zrusEfekt(e);
			logBoja.push_front("Npc: effect fade off\n" + e->popis());
		}

		trebaZmazatEfekty.clear();

		
		vector<Akcia* >trebaZmazatAkcie;
		//aktualizuje cooldowny hraca
		for (map<Akcia*, int>::iterator it = hracCooldowny.begin(); it != hracCooldowny.end(); ++it)
		{
			if (it->second <= casovac.getElapsedTime().asMilliseconds()) {
				trebaZmazatAkcie.push_back(it->first);
			}
		}

		for (auto e : trebaZmazatAkcie) {
			hracCooldowny.erase(e);
		}
		trebaZmazatAkcie.clear();

		//aktualizuje cooldowny npc
		for (map<Akcia*, int>::iterator it = npcCooldowny.begin(); it != npcCooldowny.end(); ++it)
		{
			if (it->second <= casovac.getElapsedTime().asMilliseconds()) {
				trebaZmazatAkcie.push_back(it->first);
			}
		}

		for (auto e : trebaZmazatAkcie) {
			npcCooldowny.erase(e);
		}
		trebaZmazatAkcie.clear();


		if (casovac.getElapsedTime().asMilliseconds() > hracCasVykonaniaAkcie && hracAkcia != nullptr) {
			string vysledok = hracAkcia->vykonajSa(hrac->Getstatistika(), npc->Getstatistika(), casovac.getElapsedTime());
			logBoja.push_front("Player: " + vysledok);
			hracCooldowny.insert(pair<Akcia*, int>(hracAkcia, casovac.getElapsedTime().asMilliseconds() + hracAkcia->Getcooldown()));
			hracAkcia = nullptr;
		}

		if (casovac.getElapsedTime().asMilliseconds() > npcCasVykonaniaAkcie && npcAkcia != nullptr) {
			string vysledok = npcAkcia->vykonajSa(npc->Getstatistika(), hrac->Getstatistika(), casovac.getElapsedTime());
			logBoja.push_front("Npc: " + vysledok);
			npcCooldowny.insert(pair<Akcia*, int>(npcAkcia, casovac.getElapsedTime().asMilliseconds() + npcAkcia->Getcooldown()));
			npcAkcia = nullptr;
		}

		if (hrac->Getstatistika()->Gethp() <= 0 || npc->Getstatistika()->Gethp() <= 0) {
			koniec = true;
		}


		if (npcAkcia == nullptr) {// vyberie si npc akciu
			Akcia* akcia;
			akcia = npc->vyberAkciu(&npcCooldowny);
			npcAkcia = akcia;
			if (akcia != nullptr) {
				npcCasVykonaniaAkcie = casovac.getElapsedTime().asMilliseconds() + akcia->GetcasCastenia();
				casovac.resume();
			}
		}



		if (hracAkcia == nullptr) {
			vyber = true;
			casovac.pause();
		}
		else {
			casovac.resume();
		}
	}
	else {
		this->vyhodnotenie();
	}
}

void Boj::vyhodnotenie() {
	if (!boloVyhodnotenie) {
		boloVyhodnotenie = true;

		Hra* hra = Loader::Instance()->Gethra();
		StavHranieHry* stavHranieHry = static_cast<StavHranieHry*>(Loader::Instance()->Gethra()->Getstav("hranieHry"));
		Mapa* mapa = stavHranieHry->getMapa();

		if (hrac->Getstatistika()->Gethp() > 0) {
			int ziskaneXp = npc->Getstatistika()->dajUroven() - hrac->Getstatistika()->dajUroven();
			if (ziskaneXp < 1) {
				ziskaneXp = 1;
			}

			int ziskaneZlato = 2 * npc->Getstatistika()->dajUroven();
			hrac->pridajSkusenosti(ziskaneXp);
			hrac->Getinventar()->pridajZlato(ziskaneZlato);
			string text = "You succesfully defeat " + npc->Getmeno() + "\n";
			text += "Reward: " + to_string(ziskaneXp) + " xp and " + to_string(ziskaneZlato) + " gold";
			stavHranieHry->zobrazPopup(new PopupOkno(text));
			QuestManager* qm = hrac->Getmanazerquestov();
			qm->udalost(QuestEvent::ZABITIE_NPC, npc);

			SmerPohladu smerPohladu = hrac->GetSmerPohladu();
			int hracX = hrac->GetpolickoX();
			int hracY = hrac->GetpolickoY();
			if (smerPohladu == hore) {
				hracY--;
			}

			if (smerPohladu == dole) {
				hracY++;
			}

			if (smerPohladu == vlavo) {
				hracX--;
			}

			if (smerPohladu == vpravo) {
				hracX++;
			}

			if(!mapa->jeMoznyPohyb(hracX,hracY))
			{
				hracX = hrac->GetpolickoX();
				hracY = hrac->GetpolickoY();
			}

			if (rand() % 100 < 35) {// 35% �e padne predmet
				int cislo = rand() % 100;
				int lvlOd = npc->Getstatistika()->dajUroven() - 2;
				int lvlDo = npc->Getstatistika()->dajUroven() + 2;
				if (lvlOd < 1) {
					lvlOd = 1;
				}

				int vyslednyLvl = lvlOd + rand() % (lvlDo - lvlOd + 1);

				if (cislo < 25) {
					mapa->GetPolicko(hracX, hracY)->polozPredmet(Generator::Instance()->nahodnaZbran(vyslednyLvl), mapa->aktCas());
				}
				else if (cislo < 50) {
					mapa->GetPolicko(hracX, hracY)->polozPredmet(Generator::Instance()->nahodneOblecenie(vyslednyLvl), mapa->aktCas());
				}
				else {
					mapa->GetPolicko(hracX, hracY)->polozPredmet(Generator::Instance()->nahodnyElixir(), mapa->aktCas());
				}
			}

			deque<Quest*>* nedokonceneQuesty = hrac->Getmanazerquestov()->Getnedokoncenequesty();
			map<string, Predmet*>* questDrop = npc->dropQuestPredmetov();

			for (unsigned int i = 0; i < nedokonceneQuesty->size(); i++) {
				Quest* q = nedokonceneQuesty->at(i);
				if (questDrop->count(q->Getnazov()) && q->Getstav() == StavQuestu::ROZROBENY) {// predmet je v npc drope a quest je rozrobeny
					if (rand() % 100 < 40) {
						mapa->GetPolicko(hracX, hracY)->polozPredmet(questDrop->at(q->Getnazov())->copy(), mapa->aktCas());
					}
				}
			}


		}
		else {

			Statistika* s = hrac->Getstatistika();
			s->Sethp(s->GethpMax());
			s->Setmp(s->GetmpMax());

			int pocet = 0;

			while (hrac->Getinventar()->pocetPredmetov() != pocet) {

				Predmet * p = hrac->Getinventar()->dajPredmetNaIndexe(hrac->Getinventar()->pocetPredmetov() - 1 - pocet);
				if (p->Gettyp() == 13) {
					pocet++;
				}
				else
				{
					hrac->Getinventar()->zmazPredmet(p);
					mapa->GetPolicko(hrac->GetpolickoX(), hrac->GetpolickoY())->polozPredmet(p, mapa->aktCas());
				}
				
			}

			mapa->posunHracaNaPolicko(mapa->Gethrobsuradnice().x, mapa->Gethrobsuradnice().y, 0);
			stavHranieHry->zobrazPopup(new PopupOkno("You Lost! Your items has been dropped where you die."));
		}

		//zrusenie aktivnych bufov
		map<Efekt*, sf::Time>* aktivneEfekty = hrac->Getstatistika()->Getaktivneefekty();
		for (map<Efekt*, sf::Time>::iterator it = aktivneEfekty->begin(); it != aktivneEfekty->end(); ++it)
		{
			hrac->Getstatistika()->zrusEfekt(it->first);
		}

		hrac->Getstatistika()->setCombat(false);
		if (hrac->Getstatistika()->Gethp() > 0)
		{
			AudioManager::Instance()->playEfekt("bojVyhra");
		}else
		{
			AudioManager::Instance()->playEfekt("bojPrehra");
		}
		hra->zmenStavRozhrania("hranieHry");
	}
}

string Boj::Getlog(int paOd, int paDo) {
	int indexOd, indexDo;
	if (paOd < 0) {
		indexOd = 0;
	}
	else indexOd = paOd;

	if (paDo >= (signed int)logBoja.size()) {
		indexDo = logBoja.size() - 1;
	}
	else indexDo = paDo;

	string log = "";
	for (auto i = indexOd; i <= indexDo; i++) {
		log += logBoja.at(i) + "\n";
	}
	return log;
}