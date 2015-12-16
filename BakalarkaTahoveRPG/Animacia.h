#pragma once

#include <SFML/Graphics.hpp>

class Animacia
{

public:
	Animacia(std::string cesta, int pocetObrazkov, int trvanie, int velkostX,int velkostY);
	virtual ~Animacia();

	sf::Sprite* dajObrazok();
	void reset();
	void tick();
	int GetvelkostX();
	int GetvelkostY();

private:
	int aktObrazok;
	sf::Texture textura;
	sf::Sprite* obrazok;
	int pocetObrazkov;
	int trvanie;
	int trvanieCounter;
	int velkostTexturyX;
	int velkostTexturyY;


};

