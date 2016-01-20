#ifndef tlacidlo_h
#define tlacidlo_h
#include "SFML\Graphics.hpp"

class Tlacidlo {
public:
	Tlacidlo(sf::Sprite* normal, sf::Sprite* clicked, std::string, sf::Vector2f pozicia, sf::Vector2f velkost, sf::Font *font, unsigned int velkostPisma);
	void skontrolujKlik(sf::Vector2i pozicia);
	void Setzakliknute(bool);
	void Settext(std::string);
	bool Getzakliknute();
	sf::Sprite* Getsprite();
	sf::Text Gettext();
private:
	sf::Sprite* normalne;
	sf::Sprite* kliknute;
	sf::Sprite* aktualne;
	sf::Text text;
	bool zakliknute;
	sf::Vector2f pozicia;
	sf::Vector2f velkost;
};
#endif