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

    // Save the old stdout mode and switch stdout into UTF-16 mode.
    // This is required so the Unicode chess piece symbols print correctly with wcout.
    // Without restoring the old mode, later cout output can become problematic... such as becoming chinese characters.
    int oldMode = _setmode(_fileno(stdout), _O_U16TEXT);
    // Required to change console text and background colors.
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Loops through the board rows starting with 0 and ending on 7. i is the index of the row.
    for (int i = 0; i < 8; i++)
    {
        // Makes the background of the number on the left light.
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        // Prints 8 - the current row on the left before the board.
        wcout << 8 - i;
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
    // Prints the corresponding letters at the bottom of the board
    wcout << L"  a "; wcout << L" b "; wcout << L" c "; wcout << L" d "; wcout << L" e "; wcout << L" f "; wcout << L" g "; wcout << L" h";

    // Restores the old stdout mode so that normal cout output works correctly afterwards.
    _setmode(_fileno(stdout), oldMode);
}



/*
	Aliohjelma tarkistaa että käyttäjän antama syöte siirroksi on 
	muodollisesti korrekti (ei tarkista aseman laillisuutta)
	Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
*/
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
	// Endless loop until user inputs a valid move.
    while (true)
    {
        // Asks user to input a move. This uses cout (narrow output).
        // This is why restoring stdout mode in piirraLauta() matters.
        cout << " Anna siirto : ";

		//Stores the input line into string s.
        string s;
		// "ws" skips any leading whitespace characters to ensure that getline reads the actual input without problems.
        getline(cin >> ws, s);

		// If the input is "O-O", then we return a Siirto object representing short castling.
        if (s == "O-O")
            return Siirto(true, false);
		// If the input is "O-O-O", then we return a Siirto object representing long castling.
        if (s == "O-O-O")
            return Siirto(false, true);
		// Normal move input validation that checks to make sure the input is exactly 6 characters long. For example "Rg1-f3".
        if (s.length() != 6)
        {
            cout << "Virheellinen muoto.\n";
            continue;
        }
		// First part of the 6 part series of the "split" input. This is the piece letter for example 'R' for ratsu. Check below for other parts.
        char nappula = s[0];
        // Makes sure the first character of the input s[0] is a valid piece letter.
        if (!(nappula == 'T' || nappula == 'R' || nappula == 'L' || nappula == 'D' || nappula == 'K'))
        {
            cout << "Virheellinen nappulan kirjain.\n";
            continue;
        }
        // Here we take the user input and "split" it into 6 different parts.
		// First part we already had a few lines ago: char nappula = s[0];
		char aFile = s[1]; // Second part is the starting square's column for example "g".
		char aRank = s[2]; // Third part is starting square's row for example "1".
		char dash = s[3]; // Fourth part is the symbol '-' that separates the starting and ending squares.
		char lFile = s[4]; // Fifth part is ending square's column for example "f".
		char lRank = s[5]; // Sixth part is ending square's row for example "3".

		// Here we check to make sure that the starting and ending squares are valid chess board squares. In other words, columns a-h and rows 1-8.
        if (!(aFile >= 'a' && aFile <= 'h') || !(lFile >= 'a' && lFile <= 'h'))
        {
            cout << "Virheellinen sarake (a-h).\n";
            continue;
        }

        if (!(aRank >= '1' && aRank <= '8') || !(lRank >= '1' && lRank <= '8'))
        {
            cout << "Virheellinen rivi (1-8).\n";
            continue;
        }
		// Here we check to make sure that the separating character is a proper dash '-' and not one of those discount lousy dashes from Temu.
        if (dash != '-')
        {
            cout << "Virheellinen muoto (puuttuu '-').\n";
            continue;
        }
		// If we reach this point, then the input is valid. We can now convert the input into a Siirto object and return it.
		int alkuSarake = aFile - 'a'; // Converts 'a'-'h' into 0-7.
		int loppuSarake = lFile - 'a'; // Converts 'a'-'h' into 0-7.        We need to do all these conversions because our internal board representation uses indexes from 0-7.
		int alkuRivi = 8 - (aRank - '0'); // Converts '1'-'8' into 7-0.
		int loppuRivi = 8 - (lRank - '0'); // Converts '1'-'8' into 7-0.

		// Creates Ruutu objects for the starting and ending squares.
        Ruutu alku(alkuSarake, alkuRivi);
        Ruutu loppu(loppuSarake, loppuRivi);
		// Returns a Siirto object constructed with the starting and ending squares a.ka "a normal move".
        return Siirto(alku, loppu);
    }
}



int Kayttoliittyma::kysyVastustajanVari()
{
	return 0;
}
