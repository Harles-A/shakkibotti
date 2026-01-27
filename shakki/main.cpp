#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include "kayttoliittyma.h"
#include "Siirto.h"
#include "asema.h"

using namespace std; 

int main()
{
	Asema asema; // Create a new Asema object representing the chess position.
	Kayttoliittyma* ui = Kayttoliittyma::getInstance(); // Get the singleton instance of the Kayttoliittyma (user interface).
	ui->aseta_asema(&asema); // Gives the UI a pointer to the current Asema so it can draw and read pieces

	while (true) // Infinite loop: keeps asking for moves forever. For testing purposes.
	{
		ui->piirraLauta(); // Draws the current board position.
		Siirto s = ui->annaVastustajanSiirto(); // Asks the user for their move and returns it as a Siirto object.
		asema.paivitaAsema(&s); // Apply that move to the board state (updates _lauta + turn + flags)
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
