#pragma once

#include <SFML\Graphics.hpp>
#include <string>

class Policko;
class Hrac;

class Mapa
{

public:
	Mapa(std::string menoMapy,Hrac* paHrac);
	~Mapa();

	void setHrac(Hrac* paHrac);
	int Getvyska();
	int Getsirka();

	bool jeMoznyPohyb(int x, int y);
	void posunVpravo();
	void posunVlavo();
	void posunHore();
	void posunDole();
	int Getsmerpohybu();

	void posunHracaNaPolicko(int x, int y);
	void posun(int posunX, int posunY);
	void render(sf::RenderWindow* okno);
	void update(double delta);

	void nacitajMapu(std::string paCesta);

private:
	Hrac* hrac;
	Policko* mapa[100][100];
	std::map<int, sf::Texture*> textury;

	int sirka;
	int vyska;

	int posunX;
	int posunY;
	bool hybeSa;
	int pohybDelta;
	int smerPohybu;

};