#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include "kayttoliittyma.h"
#include "Siirto.h"
#include "asema.h"
#include <list>

using namespace std; 

int main()
{
	Asema asema; // Create a new Asema object representing the chess position.
	Kayttoliittyma* ui = Kayttoliittyma::getInstance(); // Get the singleton instance of the Kayttoliittyma (user interface).
	ui->aseta_asema(&asema); // Gives the UI a pointer to the current Asema so it can draw and read pieces

	while (true) // Infinite loop: keeps asking for moves forever. For testing purposes.
	{
		ui->piirraLauta(); // Draws the current board position.

		cout << "\nVuoro: " << (asema.getSiirtovuoro() == 0 ? "VALKEA" : "MUSTA") << "\n"; // Helper to keep track whose turn it is as i keep forgetting.

		Siirto s = ui->annaVastustajanSiirto(); // Asks the user for their move and returns it as a Siirto object.


		// Quit check:
		if (s.getAlkuruutu().getRivi() == -1 && s.getAlkuruutu().getSarake() == -1)
			break;

		// Special check for Castling
		if (s.onkoLyhytLinna() || s.onkoPitkälinna())
		{
			asema.paivitaAsema(&s);
			continue;
		}
		// Get the piece that is being moved (if any) for promotion purposes later. We need to do this before we check if the move is legal, because if the move is illegal then we will ask the user for input again and we don't want to lose this information.
		Nappula* nappula = asema._lauta[s.getAlkuruutu().getRivi()][s.getAlkuruutu().getSarake()];

		// Generate all raw moves for the side whose turn it is
		list<Siirto> sallitut;
		asema.annaLaillisetSiirrot(sallitut);

		bool loytyi = false;

		// Check if the user's move exists in the generated move list (start + end match)
		for (Siirto& sallittu : sallitut)
		{
			if (sallittu.getAlkuruutu().getRivi() == s.getAlkuruutu().getRivi() &&
				sallittu.getAlkuruutu().getSarake() == s.getAlkuruutu().getSarake() &&
				sallittu.getLoppuruutu().getRivi() == s.getLoppuruutu().getRivi() &&
				sallittu.getLoppuruutu().getSarake() == s.getLoppuruutu().getSarake())
			{
				loytyi = true;
				break;
			}
		}

		if (loytyi)
		{
			// If the move is a pawn promotion move, we need to ask the user what piece they want to promote to and store that information in the Siirto object before we apply the move to the Asema.
			if (nappula != nullptr && (nappula->getKoodi() == VS || nappula->getKoodi() == MS))
			{
				int loppuRivi = s.getLoppuruutu().getRivi();
				// White promotes at row 0, black promotes at row 7.
				if ((nappula->getVari() == 0 && loppuRivi == 0) ||
					(nappula->getVari() == 1 && loppuRivi == 7))
				{
					// Ask the UI for the choice ("D","T","L","R").
					std::wstring valinta = ui->kysyKorotus(nappula->getVari());
					// Store the chosen piece pointer in the move object.
					// paivitaAsema() will then place that piece on the board.
					if (nappula->getVari() == 0)
					{
						if (valinta == L"D") s._miksikorotetaan = Asema::vd;
						else if (valinta == L"T") s._miksikorotetaan = Asema::vt;
						else if (valinta == L"L") s._miksikorotetaan = Asema::vl;
						else if (valinta == L"R") s._miksikorotetaan = Asema::vr;
					}
					else
					{
						if (valinta == L"D") s._miksikorotetaan = Asema::md;
						else if (valinta == L"T") s._miksikorotetaan = Asema::mt;
						else if (valinta == L"L") s._miksikorotetaan = Asema::ml;
						else if (valinta == L"R") s._miksikorotetaan = Asema::mr;
					}
				}
			}
			asema.paivitaAsema(&s); // Apply that move to the board state (updates _lauta + turn + flags)
		}
		else
		{
			cout << "Virheellinen siirto!\n";
		}
	}

	
	//wcout << "HeippariShakki\n";
	//wcout << "Tervetuloa pelaamaan!\n";
	//int lopetus = 100;
	//Asema asema; 
	//Kayttoliittyma::getInstance()->aseta_asema(&asema);

	//Peli peli(Kayttoliittyma::getInstance()->
	//	kysyVastustajanVari());
	//std::list<Siirto> lista;
	//system("cls");
	//int koneenVari = peli.getKoneenVari();

	//while (lopetus != 0) {
	//	lista.clear();
	//	Kayttoliittyma::getInstance()->piirraLauta();
	//	wcout << "\n";
	//	// Tarkasta onko peli loppu?
	//	asema.annaLaillisetSiirrot(lista);
	//	if (lista.size() == 0) {
	//		lopetus = 0;
	//		std::wcout << "Peli loppui";
	//		continue;
	//	}
	//	Siirto siirto;
	//	if (asema.getSiirtovuoro() == koneenVari) {
	//		MinMaxPaluu paluu;
	//		if (koneenVari == 0) {
	//			paluu = asema.maxi(3);
	//		}
	//		else {
	//			paluu = asema.mini(3);
	//		}
	//		siirto = paluu._parasSiirto;
	//	}
	//	else {
	//		siirto = Kayttoliittyma::getInstance()->
	//			annaVastustajanSiirto();
	//	}
	//	asema.paivitaAsema(&siirto);
	//}

	
	return 0;
}
