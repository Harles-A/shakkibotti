#include <iostream>
#include "asema.h"
#include "minMaxPaluu.h"
#include "nappula.h"
#include "ruutu.h"

Nappula* Asema::vk = new Kuningas(L"\u2654", 0, VK);
Nappula* Asema::vd = new Daami(L"\u2655", 0, VD);
Nappula* Asema::vt = new Torni(L"\u2656", 0, VT);
Nappula* Asema::vl = new Lahetti(L"\u2657", 0, VL);
Nappula* Asema::vr = new Ratsu(L"\u2658", 0, VR);
Nappula* Asema::vs = new Sotilas(L"\u2659", 0, VS);

Nappula* Asema::mk = new Kuningas(L"\u265A", 1, MK);
Nappula* Asema::md = new Daami(L"\u265B", 1, MD);
Nappula* Asema::mt = new Torni(L"\u265C", 1, MT);
Nappula* Asema::ml = new Lahetti(L"\u265D", 1, ML);
Nappula* Asema::mr = new Ratsu(L"\u265E", 1, MR);
Nappula* Asema::ms = new Sotilas(L"\u265F", 1, MS);


Asema::Asema()
{
	// Loop through all 8 rows
	for (int r = 0; r < 8; r++)
		// And all 8 columns
		for (int c = 0; c < 8; c++)
			// Set each square to nullptr because in c++ arrays are not automatically initialized, without this code the board could contain garbage values.
			_lauta[r][c] = nullptr;

	// At the beginning of the game it is white's turn to move. So we set _siirtovuoro to 0 (white).
	_siirtovuoro = 0;
	// No pieces have moved at the start of the game, so all these booleans are set to false.
	_onkoValkeaKuningasLiikkunut = false;
	_onkoMustaKuningasLiikkunut = false;
	_onkoValkeaDTliikkunut = false;
	_onkoValkeaKTliikkunut = false;
	_onkoMustaDTliikkunut = false;
	_onkoMustaKTliikkunut = false;

	// Place the black piece on the board
	_lauta[0][0] = mt; _lauta[0][1] = mr; _lauta[0][2] = ml; _lauta[0][3] = md;
	_lauta[0][4] = mk; _lauta[0][5] = ml; _lauta[0][6] = mr; _lauta[0][7] = mt;
	// This is for pawns
	for (int c = 0; c < 8; c++) _lauta[1][c] = ms;

	// Set the white pieces on the board
	_lauta[7][0] = vt; _lauta[7][1] = vr; _lauta[7][2] = vl; _lauta[7][3] = vd;
	_lauta[7][4] = vk; _lauta[7][5] = vl; _lauta[7][6] = vr; _lauta[7][7] = vt;
	// This is for pawns
	for (int c = 0; c < 8; c++) _lauta[6][c] = vs;
}


void Asema::paivitaAsema(Siirto *siirto)
{

	// Kaksoisaskel-lippu on oletusarvoisesti pois päältä.
	// Asetetaan myöhemmin, jos tarvii.


	//Tarkastetaan on siirto lyhyt linna


	// onko pitkä linna



	// Kaikki muut siirrot


		//Ottaa siirron alkuruudussa olleen nappulan talteen 


		//Laittaa talteen otetun nappulan uuteen ruutuun


		// Tarkistetaan oliko sotilaan kaksoisaskel
		// (asetetaan kaksoisaskel-lippu)

		// Ohestalyönti on tyhjään ruutuun. Vieressä oleva (sotilas) poistetaan.

		//// Katsotaan jos nappula on sotilas ja rivi on päätyrivi niin ei vaihdeta nappulaa 
		////eli alkuruutuun laitetaan null ja loppuruudussa on jo kliittymän laittama nappula MIIKKA, ei taida minmaxin kanssa hehkua?

		//
		////muissa tapauksissa alkuruutuun null ja loppuruutuun sama alkuruudusta lähtenyt nappula

		// katsotaan jos liikkunut nappula on kuningas niin muutetaan onkoKuningasLiikkunut arvo (molemmille väreille)

		// katsotaan jos liikkunut nappula on torni niin muutetaan onkoTorniLiikkunut arvo (molemmille väreille ja molemmille torneille)

	//päivitetään _siirtovuoro


	// First we check to see if the move is "short castle" a.ka 0-0.
	if (siirto->onkoLyhytLinna()) // We call the method onkoLyhytLinna() to check if the move is short castle. Returns true if it is.
	{
		// Here we check whose turn it is. (0 = white, 1 = black).
		if (_siirtovuoro == 0) //If it is white's turn then we do the short castling for white.
		{
			// row 7 is white's back row. Column 4 is "e" column. So [7][4] is e1 where the white king starts.
			_lauta[7][6] = _lauta[7][4]; // Move the king to g1 (column 6).
			_lauta[7][4] = nullptr; // Set the e1 square to empty (nullptr) because king went bye-bye from there.
			_lauta[7][5] = _lauta[7][7]; // Move the rook to f1 (column 5).
			_lauta[7][7] = nullptr; // Set the h1 square to empty (nullptr) because rook went bye-bye from there.
			// These booleans are here to keep track whether the white king and/or rook has moved at least once.
			// Because if they have moved, castling is no longer allowed.
			_onkoValkeaKuningasLiikkunut = true; // The white king has moved so the boolean is set to true.
			_onkoValkeaKTliikkunut = true; // The white king-side chick / rook has moved so the boolean is set to true.
		}
		else // If it is not white's turn, it must be black's turn , so we do the short castling for black. All the code is same, just from black's perspective.
		{
			_lauta[0][6] = _lauta[0][4];
			_lauta[0][4] = nullptr;
			_lauta[0][5] = _lauta[0][7];
			_lauta[0][7] = nullptr;

			_onkoMustaKuningasLiikkunut = true;
			_onkoMustaKTliikkunut = true;
		}

		// After we have done whichever colour's turn it was, we change the turn to the other color. If it was 0 (white) it becomes 1 (black) and the other way around.
		_siirtovuoro = 1 - _siirtovuoro;
		// We return from castling because it is a special move and we can't transition directly into normal move section below.
		return;
	}

	// Here is the code for "long castling" which is a lot like short castling but longer. I assume. Don't really know much about chess tbh.
	if (siirto->onkoPitkälinna()) // Returns true if Siirto is 0-0-0 a.ka long castle.
	{
		if (_siirtovuoro == 0) // As always, we check first to see if it is white's turn (because if it isn't then it is black's turn, duh).
		{
			_lauta[7][2] = _lauta[7][4]; // Move king's pointer from e1 to c1.
			_lauta[7][4] = nullptr; // Clean up time!
			_lauta[7][3] = _lauta[7][0]; // Move rook pointer from a1 to d1.
			_lauta[7][0] = nullptr; // Clean up time!

			// Same as before, we set the booleans true so that we know that king and the a1 rook have moved.
			_onkoValkeaKuningasLiikkunut = true;
			_onkoValkeaDTliikkunut = true;
		}
		// And here we do the same for black if it is their turn.
		else
		{
			_lauta[0][2] = _lauta[0][4];
			_lauta[0][4] = nullptr;
			_lauta[0][3] = _lauta[0][0];
			_lauta[0][0] = nullptr;

			_onkoMustaKuningasLiikkunut = true;
			_onkoMustaDTliikkunut = true;
		}
		// After we have done whichever colour's turn it was, we change the turn to the other color. If it was 0 (white) it becomes 1 (black) and the other way around.
		_siirtovuoro = 1 - _siirtovuoro;
		// We return from castling because it is a special move and we can't transition directly into normal move section below.
		return;
	}
	// And now that we have finished playing around with "special super fun moves", we can finally do the normal moves. Hooray!
	// We only get to this part of the code if the move was not castling. You win some, you lose some i suppose.

	// We get the starting and ending squares of the move.
	Ruutu alku = siirto->getAlkuruutu();
	Ruutu loppu = siirto->getLoppuruutu();

	// We extract the row and column index...es?
	int alkuRivi = alku.getRivi();
	int alkuSarake = alku.getSarake();
	// And then we do the same for the ending square.
	int loppuRivi = loppu.getRivi();
	int loppuSarake = loppu.getSarake();

	// Here we check which piece is being moved. A.ka which piece's pointer is in the starting square.
	// We store that pointer in a variable called "liikutettava".
	Nappula* liikutettava = _lauta[alkuRivi][alkuSarake];
	
	// We move the piece to the ending square by setting the pointer in the ending square to the pointer we stored in "liikutettava".
	_lauta[loppuRivi][loppuSarake] = liikutettava;
	// We clean up the starting square by setting it to nullptr.
	_lauta[alkuRivi][alkuSarake] = nullptr;

	// In the final part of our epic journey we check if the piece that was moved was a king or a rook.
	// First we check to make sure we are not trying to move an empty square.
	if (liikutettava != nullptr)
	{
		// We check what the "code" for the button in question is.
		int koodi = liikutettava->getKoodi();
		// If it is a king (VK or MK), we set the corresponding boolean to true.
		if (koodi == VK) _onkoValkeaKuningasLiikkunut = true;
		if (koodi == MK) _onkoMustaKuningasLiikkunut = true;

		// If it is a rook, we check which rook it is by looking at the starting square. This way we find out if it was white or black and if it was the rook involved in short or long castling.
		if (koodi == VT)
		{
			// White rooks
			if (alkuRivi == 7 && alkuSarake == 0) _onkoValkeaDTliikkunut = true;
			if (alkuRivi == 7 && alkuSarake == 7) _onkoValkeaKTliikkunut = true;
		}
		if (koodi == MT)
		{
			// Black rooks.
			if (alkuRivi == 0 && alkuSarake == 0) _onkoMustaDTliikkunut = true;
			if (alkuRivi == 0 && alkuSarake == 7) _onkoMustaKTliikkunut = true;
		}
	}
	// Last but not least, we change the turn to the other color.
	_siirtovuoro = 1 - _siirtovuoro;

}


// This function returns whose turn it is.
int Asema::getSiirtovuoro() 
{
	return _siirtovuoro;
}

// This function sets whose turn it is.
void Asema::setSiirtovuoro(int vuoro) 
{
	_siirtovuoro = vuoro;
}

// This returns true if white king has moved at least once.
bool Asema::getOnkoValkeaKuningasLiikkunut() 
{
	return _onkoValkeaKuningasLiikkunut;
}

// Returns true if black king has moved at least once.
bool Asema::getOnkoMustaKuningasLiikkunut() 
{
	return _onkoMustaKuningasLiikkunut;
}

// Returns true if white queen-side rook has moved at least once.
bool Asema::getOnkoValkeaDTliikkunut() 
{
	return _onkoValkeaDTliikkunut;
}

// Returns true if white king-side rook has moved at least once.
bool Asema::getOnkoValkeaKTliikkunut() 
{
	return _onkoValkeaKTliikkunut;
}

// Returns true if black queen-side rook has moved at least once.
bool Asema::getOnkoMustaDTliikkunut() 
{
	return _onkoMustaDTliikkunut;
}

// Returns true if black king-side rook has moved at least once.
bool Asema::getOnkoMustaKTliikkunut() 
{
	return _onkoMustaKTliikkunut;
}


/* 1. Laske nappuloiden arvo
Daami = 9
Torni = 5
Lähetti = 3,25
Ratsu = 3
Sotilas = 1

2. Kuninkaan hyvyys
Jos avaus tai keskipeli, niin hyvä että kunigas g1 tai b1/c1
Loppupelissä vaikea sanoa halutaanko olla auttamassa omaa sotilasta korottumaan
vai olla estämässä vastustajan korotusta siksi ei oteta kantaa
3. Arvosta keskustaa sotilailla ja ratsuilla
4. Arvosta pitkiä linjoja daami, torni ja lähetti
*/
double Asema::evaluoi() 
{
	return 0;

	//kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta
	
	//1. Nappuloiden arvo
	
	//2. Kuningas turvassa
	
	//3. Arvosta keskustaa
	
	// 4. Arvosta linjoja
	
}


double Asema::laskeNappuloidenArvo(int vari) 
{
	return 0;
	
}


bool Asema::onkoAvausTaiKeskipeli(int vari) 
{
	return 0;
	// Jos upseereita 3 tai vähemmän on loppupeli
	// mutta jos daami laudalla on loppueli vasta kun kuin vain daami jäljellä
	
	//Jos vari on 0 eli valkoiset
	//niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1
	

}


double Asema::nappuloitaKeskella(int vari) 
{
	return 0;

	//sotilaat ydinkeskustassa + 0.25/napa
	//ratsut ydinkeskustassa + 0.25/napa
	//sotilaat laitakeskustassa + 0.11/napa
	//ratsut laitakeskustassa + 0.11/napa
	
	//valkeille ydinkeskusta

	
	
	//valkeille laitakeskusta
	


	//mustille ydinkeskusta
	
	//mustille laitakeskusta
	
}


double Asema::linjat(int vari) 
{
	return 0;
	
	//valkoiset
	
	//mustat
	
}


// https://chessprogramming.wikispaces.com/Minimax MinMax-algoritmin pseudokoodi (lisäsin parametrina aseman)
//int maxi(int depth, asema a) 
//	if (depth == 0) return evaluate();
//	int max = -oo;
//	for (all moves ) {
//		score = mini(depth - 1, seuraaja);
//		if (score > max)
//			max = score;
//	}
//	return max;
//}

//int mini(int depth, asema a) {
//	if (depth == 0) return -evaluate();
//	int min = +oo;
//	for (all moves) {
//		score = maxi(depth - 1);
//		if (score < min)
//			min = score;
//	}
//	return min;
//}
MinMaxPaluu Asema::minimax(int syvyys)
{
	MinMaxPaluu paluuarvo;

	// Generoidaan aseman lailliset siirrot.
	
	// Rekursion kantatapaus 1: peli on loppu
	
	// Rekursion kantatapaus 2: katkaisusyvyydessä
	
	// Rekursioaskel: kokeillaan jokaista laillista siirtoa s
	// (alustetaan paluuarvo huonoimmaksi mahdolliseksi).
	
	return paluuarvo;
}


MinMaxPaluu Asema::maxi(int syvyys) 
{
	MinMaxPaluu paluu;
	return paluu;
}


MinMaxPaluu Asema::mini(int syvyys) 
{
	MinMaxPaluu paluu;
	return paluu;
}


bool Asema::onkoRuutuUhattu(Ruutu* ruutu, int vastustajanVari)
{

	return false;
}


void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>& lista, int vari) 
{ 
	
}


void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista)
{
	// Go through every square on the board.
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			// Pointer to the piece on this square (nullptr if empty).
			Nappula* p = _lauta[r][c];

			// Only generate moves for the side whose turn it currently is.
			if (p != nullptr && p->getVari() == _siirtovuoro)
			{
				// PS! Ruutu constructor is (sarake, rivi),
				// but _lauta is indexed as [rivi][sarake].
				Ruutu ruutu(c, r);

				// Ask that piece to add all its moves into the list.
				p->annaSiirrot(lista, &ruutu, this, _siirtovuoro);
			}
		}
	}
}

