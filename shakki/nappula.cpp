#include <list>
#include <string>
#include "asema.h"
#include "nappula.h"
#include <iostream>
using namespace std;
static void laskeLiukuvat(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari, int dx[], int dy[], int suuntia) {
	int alkurivi = ruutu->getRivi();
	int alkusarake = ruutu->getSarake();

	for (int i = 0; i < suuntia; ++i) {
		for (int askel = 1; askel < 8; ++askel) {
			int uusiRivi = alkurivi + askel * dy[i];
			int uusiSarake = alkusarake + askel * dx[i];

			// Pysyykö laudalla?
			if (uusiRivi < 0 || uusiRivi > 7 || uusiSarake < 0 || uusiSarake > 7) break;

			Nappula* kohde = asema->_lauta[uusiRivi][uusiSarake];

			if (kohde == nullptr) {
				// Tyhjä ruutu -> siirto ok
				lista.push_back(Siirto(Ruutu(alkusarake, alkurivi), Ruutu(uusiSarake, uusiRivi))); // Swapped the order of alkuSarake and alkurivi to match Ruutu constructor, otherwise our moves are going the wrong way around.
			}
			else {
				// Nappula edessä
				if (kohde->getVari() != vari) {
					// Vastustaja -> syönti ja break
					lista.push_back(Siirto(Ruutu(alkusarake, alkurivi), Ruutu(uusiSarake, uusiRivi))); // Swapped the order of alkuSarake and alkurivi to match Ruutu constructor, otherwise our moves are going the wrong way around.
				}
				// Oma tai vastustaja -> liike loppuu tähän suuntaan
				// Oma tai vastustaja -> liike loppuu tähän suuntaan
				break;
			}
		}
	}
}
static void laskePomppivat(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari, int dx[], int dy[], int suuntia) {
	int alkuRivi = ruutu->getRivi();
	int alkuSarake = ruutu->getSarake();

	for (int i = 0; i < suuntia; ++i) {
		int uusiRivi = alkuRivi + dy[i];
		int uusiSarake = alkuSarake + dx[i];

		if (uusiRivi < 0 || uusiRivi > 7 || uusiSarake < 0 || uusiSarake > 7) continue;

		Nappula* kohde = asema->_lauta[uusiRivi][uusiSarake];

		if (kohde == nullptr || kohde->getVari() != vari)
		{
			lista.push_back(Siirto(Ruutu(alkuSarake, alkuRivi), Ruutu(uusiSarake, uusiRivi)));
		}
	}
}

Nappula::Nappula(wstring unicode, int vari, int koodi)
{
	// Saves the unicode symbol for this piece. Will be used when drawing the board with wcout.
	_unicode = unicode;
	// Saves the colour of each piece. 0 = white, 1 = black.
	_vari = vari;
	// Saces to "code" of each pice, like VT, VL, VR, VK, VS etc.
	_koodi = koodi;
}


void Torni::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int dx[] = { 1, -1, 0, 0 };
	int dy[] = { 0, 0, 1, -1 };
	laskeLiukuvat(lista, ruutu, asema, vari, dx, dy, 4);
}


void Ratsu::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int dx[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
	int dy[] = { -2, -1, 1, 2, -2, -1, 1, 2 };
	laskePomppivat(lista, ruutu, asema, vari, dx, dy, 8);
}
void Lahetti::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int dx[] = { 1, 1, -1, -1 };
	int dy[] = { 1, -1, 1, -1 };
	laskeLiukuvat(lista, ruutu, asema, vari, dx, dy, 4);
}


void Daami::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int dx[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
	int dy[] = { 0, 0, 1, -1, 1, -1, 1, -1 };
	laskeLiukuvat(lista, ruutu, asema, vari, dx, dy, 8);
}


void Kuningas::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int dx[] = { 1, 1, 1, 0, -1, -1, -1, 0 };
	int dy[] = { 0, 1, -1, 1, 0, -1, 1, -1 }; // Replaced some of the numbers here to fix king's movement directions.
	laskePomppivat(lista, ruutu, asema, vari, dx, dy, 8);
}


void Sotilas::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
    // Current position of the "Sotilas" on the board.
    int alkuRivi = ruutu->getRivi();
    int alkuSarake = ruutu->getSarake();

    // Movement direction is dependant on the color:
    // White (0) moves "up" towards row 0 => -1
    // Black (1) moves "down" towards row 7 => +1
    int suunta = (vari == 0) ? -1 : 1;

    // Starting row for double-move
    int aloitusRivi = (vari == 0) ? 6 : 1;

    // Promotion row (when sotilas reaches last row)
    int korotusRivi = (vari == 0) ? 0 : 7;

	// Here we start our move generation for the sotilas.
    int etuRivi = alkuRivi + suunta;

    if (etuRivi >= 0 && etuRivi <= 7)
    {
		// Forward square must be empty to move forward.
        if (asema->_lauta[etuRivi][alkuSarake] == nullptr)
        {
            Siirto siirtoEteen(Ruutu(alkuSarake, alkuRivi), Ruutu(alkuSarake, etuRivi));

            // If reaching promotion row, add promotion moves instead of a normal move.
            if (etuRivi == korotusRivi)
                lisaaSotilaanKorotukset(&siirtoEteen, lista, asema);
            else
                lista.push_back(siirtoEteen);

            // Two step move forward (from start row)
            // Only possible if:
            // 1. sotilas is on its starting row.
            // 2. the square 2 steps ahead is empty.
            int kaksiEteenRivi = alkuRivi + 2 * suunta;

            if (alkuRivi == aloitusRivi && kaksiEteenRivi >= 0 && kaksiEteenRivi <= 7)
            {
                if (asema->_lauta[kaksiEteenRivi][alkuSarake] == nullptr)
                {
                    lista.push_back(Siirto(Ruutu(alkuSarake, alkuRivi), Ruutu(alkuSarake, kaksiEteenRivi)));
                }
            }
        }
    }

    // Capturing "enemy" piece diagonally.
   
    // Sotilas captures one square diagonally forward-left or forward-right.
    int diagSarakeVasen = alkuSarake - 1;
    int diagSarakeOikea = alkuSarake + 1;

    // Forward-left capture.
    if (etuRivi >= 0 && etuRivi <= 7 && diagSarakeVasen >= 0 && diagSarakeVasen <= 7)
    {
        Nappula* kohde = asema->_lauta[etuRivi][diagSarakeVasen];
        if (kohde != nullptr && kohde->getVari() != vari)
        {
            Siirto syontiVasen(Ruutu(alkuSarake, alkuRivi), Ruutu(diagSarakeVasen, etuRivi));

            if (etuRivi == korotusRivi)
                lisaaSotilaanKorotukset(&syontiVasen, lista, asema);
            else
                lista.push_back(syontiVasen);
        }
    }

    // Forward-right capture.
    if (etuRivi >= 0 && etuRivi <= 7 && diagSarakeOikea >= 0 && diagSarakeOikea <= 7)
    {
        Nappula* kohde = asema->_lauta[etuRivi][diagSarakeOikea];
        if (kohde != nullptr && kohde->getVari() != vari)
        {
            Siirto syontiOikea(Ruutu(alkuSarake, alkuRivi), Ruutu(diagSarakeOikea, etuRivi));

            if (etuRivi == korotusRivi)
                lisaaSotilaanKorotukset(&syontiOikea, lista, asema);
            else
                lista.push_back(syontiOikea);
        }
    }
}


void Sotilas::lisaaSotilaanKorotukset(Siirto* siirto, std::list<Siirto>& lista, Asema* asema)
{
    // Determine sotilas' color by looking at the piece on the move's start square.
    Ruutu alku = siirto->getAlkuruutu();
    int alkuRivi = alku.getRivi();
    int alkuSarake = alku.getSarake();

    Nappula* sotilas = asema->_lauta[alkuRivi][alkuSarake];
    int vari = 0; // default to white if something goes wrong.

    if (sotilas != nullptr)
        vari = sotilas->getVari();

    // Promotion options:
    // - Daami
    // - Torni
    // - Lahetti
    // - Ratsu
    // We store the chosen piece pointer into Siirto::_miksikorotetaan
    Nappula* vaihtoehdot[4];

    if (vari == 0)
    {
        vaihtoehdot[0] = Asema::vd;
        vaihtoehdot[1] = Asema::vt;
        vaihtoehdot[2] = Asema::vl;
        vaihtoehdot[3] = Asema::vr;
    }
    else
    {
        vaihtoehdot[0] = Asema::md;
        vaihtoehdot[1] = Asema::mt;
        vaihtoehdot[2] = Asema::ml;
        vaihtoehdot[3] = Asema::mr;
    }

    // Create 4 separate moves (same start/end) but with different promotion targets.
    for (int i = 0; i < 4; i++)
    {
        Siirto uusi(siirto->getAlkuruutu(), siirto->getLoppuruutu());
        uusi._miksikorotetaan = vaihtoehdot[i];
        lista.push_back(uusi);
    }
}
