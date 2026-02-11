#pragma once
#include "asema.h"
#include "nappula.h"
#include "peli.h"
#include "siirto.h"

#include <string>
using namespace std;


// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.
class Kayttoliittyma
{
public:

	void aseta_asema(Asema* asema) { this->_asema = asema;  }
	void piirraLauta();
	Siirto annaVastustajanSiirto();
	int kysyVastustajanVari();

	static Kayttoliittyma* getInstance();

	// Asks user what piece they want to promote their pawn to and returns the corresponding unicode symbol as a wstring. vari is the color of the pawn being promoted (0 for white, 1 for black).
	// Returns: "D", "T", "L", or "R" as a wide string.
	std::wstring kysyKorotus(int vari);

private:

	Asema* _asema;
	static Kayttoliittyma* instance; // osoitin luokan ainoaan olioon (Singleton).

	Kayttoliittyma() {}
	Kayttoliittyma(Asema* asema) { this->_asema = asema; }
};
