#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "kayttoliittyma.h"

using namespace std;


Kayttoliittyma* Kayttoliittyma::instance = 0;


Kayttoliittyma* Kayttoliittyma::getInstance()
{
	if (instance == 0)
		instance = new Kayttoliittyma();
	return instance;
}


void Kayttoliittyma::piirraLauta()
{
    // If the UI is not pointing to the correct Asema object, then we return to avoid crashes.
    if (_asema == nullptr) return;

	// This enables unicode output for Windows console (might be redunant but better safe than sorry).
    _setmode(_fileno(stdout), _O_U16TEXT);
	// Requited to change console tect and background colors.
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Loops through the board rows starting with 0 and ending on 7. i is the index of the row.
    for (int i = 0; i < 8; i++)
    {
        //Loops through the board columns starting with 0 and ending on 7. j is the index of the column
        for (int j = 0; j < 8; j++)
        {
			// If the row + column is even, then the square is one color, if it is odd then it is a different color.
            if ((i + j) % 2 == 0)
            {
				// Sets the background color of the "light" squares.
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else
            {
				// Sets the background color of the "dark" squares.
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED |
                    BACKGROUND_GREEN);
            }

            // Check which piece's pointer is stored in this board square.
            Nappula* p = _asema->_lauta[i][j];
            // If there is supposed to be a piece in this quare,
            if (p != nullptr)
                // then we print a 3-character "cell"
                wcout << L" " << p->getUnicode() << L" ";
            else
				// if there square is supposed to be empty, we print 3 spaces so the "cell" has same width. This keeps the board aligned.
                wcout << L"   ";
        }
		// After we finish a row, we print a newline to move to the next row.
        wcout << L"\n";
    }
}


/*
	Aliohjelma tarkistaa että käyttäjän antama syöte siirroksi on 
	muodollisesti korrekti (ei tarkista aseman laillisuutta)
	Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
*/
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
	Siirto siirto;
	return siirto;
	
}


int Kayttoliittyma::kysyVastustajanVari()
{
	return 0;
}
