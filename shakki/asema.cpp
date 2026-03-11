#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "asema.h"
#include "minMaxPaluu.h"
#include "nappula.h"
#include "ruutu.h"
#include "kayttoliittyma.h"
#include <string>

// Static piece objects.
// The board stores pointers to these shared piece instances instead of creating new piece objects for every square.

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

using namespace std;


Asema::Asema()
{
	// Initialize every square as empty.
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

	// Set the initial black back rank.
	_lauta[0][0] = mt; _lauta[0][1] = mr; _lauta[0][2] = ml; _lauta[0][3] = md;
	_lauta[0][4] = mk; _lauta[0][5] = ml; _lauta[0][6] = mr; _lauta[0][7] = mt;
	// Set black pawns.
	for (int c = 0; c < 8; c++) _lauta[1][c] = ms;

	// Set the initial white back rank.
	_lauta[7][0] = vt; _lauta[7][1] = vr; _lauta[7][2] = vl; _lauta[7][3] = vd;
	_lauta[7][4] = vk; _lauta[7][5] = vl; _lauta[7][6] = vr; _lauta[7][7] = vt;
	// Set white pawns.
	for (int c = 0; c < 8; c++) _lauta[6][c] = vs;
}


void Asema::paivitaAsema(Siirto* siirto)
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

	// By default there is no new double-step pawn column.
	// If the current move is a two-square pawn move, this is updated below.
	kaksoisaskelSarakkeella = -1;


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

	Nappula* kohdeEnnen = _lauta[loppuRivi][loppuSarake];
	kaksoisaskelSarakkeella = -1;

	// We move the piece to the ending square by setting the pointer in the ending square to the pointer we stored in "liikutettava".
	_lauta[loppuRivi][loppuSarake] = liikutettava;
	// We clean up the starting square by setting it to nullptr.
	_lauta[alkuRivi][alkuSarake] = nullptr;

	// Pawn special cases: en passant + double step
	if (liikutettava != nullptr && (liikutettava->getKoodi() == VS || liikutettava->getKoodi() == MS))
	{
		// En passant:
		// If a pawn moved diagonally into an empty square, remove the enemy pawn from the square behind the destination.
		if (kohdeEnnen == nullptr && (loppuSarake != alkuSarake))
		{
			int poistettavaRivi = (liikutettava->getVari() == 0) ? (loppuRivi + 1) : (loppuRivi - 1);

			if (poistettavaRivi >= 0 && poistettavaRivi <= 7)
			{
				Nappula* poistettava = _lauta[poistettavaRivi][loppuSarake];

				if (poistettava != nullptr &&
					(poistettava->getKoodi() == VS || poistettava->getKoodi() == MS) &&
					poistettava->getVari() != liikutettava->getVari())
				{
					_lauta[poistettavaRivi][loppuSarake] = nullptr;
				}
			}
		}
		// If the pawn moved two squares straight forward,
		// store its file for possible en passant on the next move.
		if (alkuSarake == loppuSarake)
		{
			int dRivi = loppuRivi - alkuRivi;
			if (dRivi == 2 || dRivi == -2)
			{
				kaksoisaskelSarakkeella = alkuSarake;
			}
		}
	}

	// Promotion
	if (liikutettava != nullptr && (liikutettava->getKoodi() == VS || liikutettava->getKoodi() == MS))
	{
		if ((liikutettava->getVari() == 0 && loppuRivi == 0) ||
			(liikutettava->getVari() == 1 && loppuRivi == 7))
		{
			// If the move already contains the chosen promotion piece, use it.
			if (siirto->_miksikorotetaan != nullptr)
			{
				_lauta[loppuRivi][loppuSarake] = siirto->_miksikorotetaan;
			}
			else
			{
				// If no promotion piece is provided, default to queen.
				_lauta[loppuRivi][loppuSarake] = (liikutettava->getVari() == 0) ? vd : md;
			}
			// Old version commented out for now.
			/*else
			{
				wstring uusiNappula = Kayttoliittyma::getInstance()->kysyKorotus(liikutettava->getVari());
				int vari = liikutettava->getVari();

				if (vari == 0)
				{
					if (uusiNappula == L"D") _lauta[loppuRivi][loppuSarake] = vd;
					else if (uusiNappula == L"T") _lauta[loppuRivi][loppuSarake] = vt;
					else if (uusiNappula == L"L") _lauta[loppuRivi][loppuSarake] = vl;
					else if (uusiNappula == L"R") _lauta[loppuRivi][loppuSarake] = vr;
				}
				else
				{
					if (uusiNappula == L"D") _lauta[loppuRivi][loppuSarake] = md;
					else if (uusiNappula == L"T") _lauta[loppuRivi][loppuSarake] = mt;
					else if (uusiNappula == L"L") _lauta[loppuRivi][loppuSarake] = ml;
					else if (uusiNappula == L"R") _lauta[loppuRivi][loppuSarake] = mr;
				}
			}*/
		}
	}


	// Update castling rights after king / rook move
	// In the final part of our epic journey we check if the piece that was moved was a king or a rook.
	// First we check to make sure we are not trying to move an empty square.
	if (liikutettava != nullptr)
	{
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


// -------------------------------------------------
// Piece-square tables (PST)
// -------------------------------------------------
// These give positional bonuses based on square.
// Values are written from white's point of view.
// For black pieces the table is mirrored vertically.


// PST for Pawn.
static const int PST_PAWN[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	 5,  5, 10, 25, 25, 10,  5,  5,
	 0,  0,  0, 20, 20,  0,  0,  0,
	 5, -5,-10,  0,  0,-10, -5,  5,
	 5, 10, 10,-20,-20, 10, 10,  5,
	 0,  0,  0,  0,  0,  0,  0,  0
};

// PST for Knight.
static const int PST_KNIGHT[64] = {
   -50,-40,-30,-30,-30,-30,-40,-50,
   -40,-20,  0,  5,  5,  0,-20,-40,
   -30,  5, 10, 15, 15, 10,  5,-30,
   -30,  0, 15, 20, 20, 15,  0,-30,
   -30,  5, 15, 20, 20, 15,  5,-30,
   -30,  0, 10, 15, 15, 10,  0,-30,
   -40,-20,  0,  0,  0,  0,-20,-40,
   -50,-40,-30,-30,-30,-30,-40,-50
};

// Returns the value from a piece-square table.
// White uses the table directly, black uses the vertically mirrored version.
static int pstLookup(const int table[64], int vari, int r, int c)
{
	int rr = (vari == 0) ? r : (7 - r);
	return table[rr * 8 + c];
}
// Returns the PST bonus for the given piece.
// In the current version only pawns and knights use PST.
static int pstBonus(int koodi, int vari, int r, int c)
{
	if (koodi == VS || koodi == MS) return pstLookup(PST_PAWN, vari, r, c);
	if (koodi == VR || koodi == MR) return pstLookup(PST_KNIGHT, vari, r, c);
	return 0;
}
// -------------------------------------------------
// Evaluation function
// -------------------------------------------------
// Returns a score from white's point of view:
// positive = white is better
// negative = black is better
//
// Scores are handled in centipawns:
// pawn = about 100, knight = about 320, etc.
double Asema::evaluoi()
{
	int score = 0;

	auto val = [&](int koodi) -> int {
		if (koodi == VS || koodi == MS) return 100;
		if (koodi == VR || koodi == MR) return 320;
		if (koodi == VL || koodi == ML) return 330;
		if (koodi == VT || koodi == MT) return 500;
		if (koodi == VD || koodi == MD) return 900;
		return 0;
		};
	// Pawn counts by file for doubled-pawn penalties and rook file bonuses.
	int wPawnFile[8] = { 0 }, bPawnFile[8] = { 0 };
	// Bishop counts for bishop-pair bonus.
	int wBish = 0, bBish = 0;
	// King locations for castling bonus.
	int wKr = -1, wKc = -1, bKr = -1, bKc = -1;

	// First pass: collects global information.
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Nappula* n = _lauta[r][c];
			if (!n) continue;

			int k = n->getKoodi();
			int v = n->getVari();

			if (k == VS) wPawnFile[c]++;
			if (k == MS) bPawnFile[c]++;

			if (k == VL) wBish++;
			if (k == ML) bBish++;

			if (k == VK) { wKr = r; wKc = c; }
			if (k == MK) { bKr = r; bKc = c; }
		}
	}
	// Second pass: evaluates each piece.
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Nappula* n = _lauta[r][c];
			if (!n) continue;

			int k = n->getKoodi();
			int v = n->getVari(); 

			int s = val(k);
			// Add piece-square-table bonus.
			s += pstBonus(k, v, r, c);
			// Simple center bonus: pieces closer to the center get a larger bonus.
			int dr = abs(r - 3);
			int dc = abs(c - 3);
			int center = (6 - (dr + dc)); 
			if (center < 0) center = 0;
			// Bishops and queens also get small center bonuses.
			if (k == VL || k == ML) s += center * 4;  
			if (k == VD || k == MD) s += center * 2;   
			// Rook bonus for open / semi-open files.
			if (k == VT || k == MT)
			{
				int friendly = (v == 0) ? wPawnFile[c] : bPawnFile[c];
				int enemy = (v == 0) ? bPawnFile[c] : wPawnFile[c];
				if (friendly == 0 && enemy == 0) s += 12;
				else if (friendly == 0) s += 6;
			}
			// White = +, black = -.
			if (v == 0) score += s;
			else score -= s;
		}
	}
	// Bishop pair bonus.
	if (wBish >= 2) score += 25;
	if (bBish >= 2) score -= 25;
	// Doubled pawn penalties.
	for (int f = 0; f < 8; f++)
	{
		if (wPawnFile[f] > 1) score -= (wPawnFile[f] - 1) * 12;
		if (bPawnFile[f] > 1) score += (bPawnFile[f] - 1) * 12;
	}
	// Castling bonus: only checks whether the king is on a typical castled square.
	if (wKr == 7 && (wKc == 6 || wKc == 2)) score += 15;
	if (bKr == 0 && (bKc == 6 || bKc == 2)) score -= 15;
	// Small tempo bonus for the side to move.
	score += (_siirtovuoro == 0) ? 5 : -5;

	return (double)score;
}


// Commented out as the following functions are no longer used.
// 
//double Asema::laskeNappuloidenArvo(int vari)
//{
//	double summa = 0;
//	for (int r = 0; r < 8; ++r) // Create variable and go trough it 8 times.
//	{
//		for (int s = 0; s < 8; ++s) // Same thing but vertical.
//		{
//			Nappula* n = _lauta[r][s];
//			if (n != nullptr && n->getVari() == vari) // If not empty and the color is correct then go trough the if statement.
//			{
//				int koodi = n->getKoodi(); // Get the piece.
//				// Add value based on what piece it is.
//				if (koodi == VS || koodi == MS) summa += 1.0;
//				else if (koodi == VR || koodi == MR) summa += 3.0;
//				else if (koodi == VL || koodi == ML) summa += 3.25;
//				else if (koodi == VT || koodi == MT) summa += 5.0;
//				else if (koodi == VD || koodi == MD) summa += 9.0;
//			}
//		}
//	}
//	return summa;
//}


//bool Asema::onkoAvausTaiKeskipeli(int vari)
//{
//	int upseerit = 0;
//	bool daamiLaudalla = false;
//
//	for (int r = 0; r < 8; ++r)
//	{
//		for (int s = 0; s < 8; ++s)
//		{
//			Nappula* n = _lauta[r][s];
//			if (n != nullptr && n->getVari() != vari) // Look only at enemy pieces
//			{
//				int k = n->getKoodi();
//				if (k == VD || k == MD) daamiLaudalla = true;
//				if (k == VR || k == MR || k == VL || k == ML || k == VT || k == MT)
//				{
//					upseerit++;
//				}
//			}
//		}
//	}
//
//	if (daamiLaudalla && upseerit >= 1) // If queen is alive and atleast 1 other general exists it is not late game.
//	{
//		return true;
//	}
//	else if (upseerit > 3) // If there is more than 3 generals alive it is not late game.
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}


//double Asema::nappuloitaKeskella(int vari)
//{
//	double bonus = 0;
//	for (int r = 2; r <= 5; ++r) // Go between lanes 2 and 5
//	{
//		for (int s = 2; s <= 5; ++s)
//		{
//			Nappula* n = _lauta[r][s];
//			if (n == nullptr || n->getVari() != vari) continue; // If empty or incorrect color, skip.
//
//			int k = n->getKoodi();
//
//			if (k == VS || k == MS || k == VR || k == MR) // Knights and Soldiers only
//			{
//				if (r >= 3 && r <= 4 && s >= 3 && s <= 4)
//				{
//					bonus += 0.25; // If they are in the middle.
//				}
//				else
//				{
//					bonus += 0.11; // If they are somewhat in the middle.
//				}
//			}
//		}
//	}
//	return bonus;
//}
//
//
//double Asema::linjat(int vari)
//{
//	double bonus = 0;
//	for (int r = 0; r < 8; ++r)
//	{
//		for (int s = 0; s < 8; ++s)
//		{
//			Nappula* n = _lauta[r][s];
//			if (n != nullptr && n->getVari() == vari)
//			{
//				int k = n->getKoodi();
//				if (k == VT || k == MT || k == VL || k == ML || k == VD || k == MD)
//				{
//					list<Siirto> siirrot;
//					Ruutu ruutu(s, r);
//					n->annaSiirrot(siirrot, &ruutu, this, vari); // Give the list of moves, location, board and color so that we can see all the moves possible.
//					bonus += siirrot.size() * 0.02; // Give 0.02 value for each possible move
//				}
//			}
//		}
//	}
//	return bonus;
//}


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


// Start the alpha-beta search with the widest possible window.
// alfa = lower bound (best score white is guaranteed so far)
// beta = upper bound (best score black is guaranteed so far)
//
// These initial values are the same for both maxi() and mini():
// alfa starts from a very small number, beta from a very large number.
// The difference between maxi and mini is not here, but inside the functions themselves:
// - maxi() tries to increase the score
// - mini() tries to decrease the score
	if (_siirtovuoro == 0) 
		paluuarvo = maxi(syvyys, -1e18, 1e18);
	else
		paluuarvo = mini(syvyys, -1e18, 1e18);

	return paluuarvo;
}


MinMaxPaluu Asema::maxi(int syvyys, double alfa, double beta)
{
	MinMaxPaluu paluu;
	const double INF = 1e18;
	const double MATE = 100000.0;

	// Generate pseudo-legal moves.
	// "Pseudo-legal" means moves are generated quickly first and moves that leave the own king in check are filtered out later.
	list<Siirto> lista;
	annaHakuSiirrot(lista);

	// Move ordering: promotions first, then captures, then castling, then other moves.
	vector<Siirto> siirrot(lista.begin(), lista.end());
	auto heur = [&](Siirto s) -> int
		{
			if (s.onkoLyhytLinna() || s.onkoPitkälinna()) return 1;

			Ruutu a = s.getAlkuruutu();
			Ruutu b = s.getLoppuruutu();
			Nappula* mover = _lauta[a.getRivi()][a.getSarake()];
			if (!mover) return 0;

			int k = mover->getKoodi();
			// Promotion
			if ((k == VS && b.getRivi() == 0) || (k == MS && b.getRivi() == 7)) return 3;
			// Capture
			Nappula* victim = _lauta[b.getRivi()][b.getSarake()];
			if (victim != nullptr) return 2;
			// En passant-style pawn diagonal capture into empty square.
			if ((k == VS || k == MS) && a.getSarake() != b.getSarake()) return 2;

			return 0;
		};
	stable_sort(siirrot.begin(), siirrot.end(),
		[&](Siirto a, Siirto b) { return heur(a) > heur(b); });
	// Leaf node
	if (syvyys == 0)
	{
		// If white is not in check, evaluate normally.
		if (!onkoKuningasShakissa(0))
		{
			paluu._evaluointiArvo = evaluoi();
			return paluu;
		}
		// If white is in check at the leaf, test if any legal escape exists.
		bool anyLegal = false;
		for (auto& s : siirrot)
		{
			Asema seuraaja = *this;
			seuraaja.paivitaAsema(&s);
			if (!seuraaja.onkoKuningasShakissa(0))
			{
				anyLegal = true;
				break;
			}
		}
		// No legal escape = checkmate.
		paluu._evaluointiArvo = anyLegal ? evaluoi() : (-MATE - syvyys);
		return paluu;
	}

	double parasArvo = -INF;
	bool loytyiLaillinen = false;
	bool eka = true;

	for (auto& s : siirrot)
	{
		Asema seuraaja = *this;
		seuraaja.paivitaAsema(&s);
		// Illegal if white king remains in check after the move.
		if (seuraaja.onkoKuningasShakissa(0))
			continue;

		loytyiLaillinen = true;

		double arvo = seuraaja.mini(syvyys - 1, alfa, beta)._evaluointiArvo;

		if (eka || arvo > parasArvo)
		{
			eka = false;
			parasArvo = arvo;
			paluu._parasSiirto = s;
		}

		if (parasArvo > alfa) alfa = parasArvo;
		if (alfa >= beta) break; 
	}
	// No legal move found = mate or stalemate.
	if (!loytyiLaillinen)
	{
		if (onkoKuningasShakissa(0))
			paluu._evaluointiArvo = -MATE - syvyys;
		else
			paluu._evaluointiArvo = 0.0;
		return paluu;
	}

	paluu._evaluointiArvo = parasArvo;
	return paluu;
}

// Same as maxi but from the other perspective.
MinMaxPaluu Asema::mini(int syvyys, double alfa, double beta)
{
	MinMaxPaluu paluu;
	const double INF = 1e18;
	const double MATE = 100000.0;

	list<Siirto> lista;
	annaHakuSiirrot(lista);

	vector<Siirto> siirrot(lista.begin(), lista.end());
	auto heur = [&](Siirto s) -> int
		{
			if (s.onkoLyhytLinna() || s.onkoPitkälinna()) return 1;

			Ruutu a = s.getAlkuruutu();
			Ruutu b = s.getLoppuruutu();
			Nappula* mover = _lauta[a.getRivi()][a.getSarake()];
			if (!mover) return 0;

			int k = mover->getKoodi();
			if ((k == VS && b.getRivi() == 0) || (k == MS && b.getRivi() == 7)) return 3;

			Nappula* victim = _lauta[b.getRivi()][b.getSarake()];
			if (victim != nullptr) return 2;
			if ((k == VS || k == MS) && a.getSarake() != b.getSarake()) return 2;

			return 0;
		};
	std::stable_sort(siirrot.begin(), siirrot.end(),
		[&](Siirto a, Siirto b) { return heur(a) > heur(b); });

	if (syvyys == 0)
	{
		if (!onkoKuningasShakissa(1))
		{
			paluu._evaluointiArvo = evaluoi();
			return paluu;
		}

		bool anyLegal = false;
		for (auto& s : siirrot)
		{
			Asema seuraaja = *this;
			seuraaja.paivitaAsema(&s);
			if (!seuraaja.onkoKuningasShakissa(1))
			{
				anyLegal = true;
				break;
			}
		}

		paluu._evaluointiArvo = anyLegal ? evaluoi() : (MATE + syvyys);
		return paluu;
	}

	double parasArvo = INF;
	bool loytyiLaillinen = false;
	bool eka = true;

	for (auto& s : siirrot)
	{
		Asema seuraaja = *this;
		seuraaja.paivitaAsema(&s);
		// Illegal if black king remains in check after the move.
		if (seuraaja.onkoKuningasShakissa(1))
			continue;

		loytyiLaillinen = true;

		double arvo = seuraaja.maxi(syvyys - 1, alfa, beta)._evaluointiArvo;

		if (eka || arvo < parasArvo)
		{
			eka = false;
			parasArvo = arvo;
			paluu._parasSiirto = s;
		}

		if (parasArvo < beta) beta = parasArvo;
		if (beta <= alfa) break; 
	}

	if (!loytyiLaillinen)
	{
		if (onkoKuningasShakissa(1))
			paluu._evaluointiArvo = MATE + syvyys; 
		else
			paluu._evaluointiArvo = 0.0;
		return paluu;
	}

	paluu._evaluointiArvo = parasArvo;
	return paluu;
}

// Returns true if the given square is attacked by the given side.
bool Asema::onkoRuutuUhattu(Ruutu* ruutu, int vastustajanVari)
{

	int kohdeRivi = ruutu->getRivi();
	int kohdeSarake = ruutu->getSarake();

	for (int rivi = 0; rivi < 8; rivi++)
	{
		for (int sarake = 0; sarake < 8; sarake++)
		{
			Nappula* p = _lauta[rivi][sarake];

			if (p == nullptr) continue;
			if (p->getVari() != vastustajanVari) continue;

			int koodi = p->getKoodi();
			// Pawns attack diagonally, so they need special handling.
			if (koodi == VS || koodi == MS)
			{
				int suunta = (vastustajanVari == 0) ? -1 : 1;
				int uhkaRivi = rivi + suunta;

				if (uhkaRivi == kohdeRivi)
				{
					if (sarake - 1 == kohdeSarake) return true;
					if (sarake + 1 == kohdeSarake) return true;
				}

				continue;
			}
			// For other piece types, generate moves and see if the target square appears.
			Ruutu alku(sarake, rivi);
			list<Siirto> siirrot;
			p->annaSiirrot(siirrot, &alku, this, vastustajanVari);

			for (auto& s : siirrot)
			{
				if (s.getLoppuruutu().getRivi() == kohdeRivi &&
					s.getLoppuruutu().getSarake() == kohdeSarake)
				{
					return true;
				}
			}
		}
	}

	return false;
}
// Castling move generation.
void Asema::annaLinnoitusSiirrot(list<Siirto>& lista, int vari)
{
	int vastustaja = 1 - vari;

	if (vari == 0)
	{
		// White short castling
		if (_onkoValkeaKuningasLiikkunut == false && _onkoValkeaKTliikkunut == false)
		{
			if (_lauta[7][4] == vk && _lauta[7][7] == vt &&
				_lauta[7][5] == nullptr && _lauta[7][6] == nullptr)
			{
				Ruutu e1(4, 7), f1(5, 7), g1(6, 7);

				if (!onkoRuutuUhattu(&e1, vastustaja) &&
					!onkoRuutuUhattu(&f1, vastustaja) &&
					!onkoRuutuUhattu(&g1, vastustaja))
				{
					lista.push_back(Siirto(true, false));
				}
			}
		}
		// White long castling
		if (_onkoValkeaKuningasLiikkunut == false && _onkoValkeaDTliikkunut == false)
		{
			if (_lauta[7][4] == vk && _lauta[7][0] == vt &&
				_lauta[7][1] == nullptr && _lauta[7][2] == nullptr && _lauta[7][3] == nullptr)
			{
				Ruutu e1(4, 7), d1(3, 7), c1(2, 7);

				if (!onkoRuutuUhattu(&e1, vastustaja) &&
					!onkoRuutuUhattu(&d1, vastustaja) &&
					!onkoRuutuUhattu(&c1, vastustaja))
				{
					lista.push_back(Siirto(false, true));
				}
			}
		}
	}
	else
	{
		// Black short castling
		if (_onkoMustaKuningasLiikkunut == false && _onkoMustaKTliikkunut == false)
		{
			if (_lauta[0][4] == mk && _lauta[0][7] == mt &&
				_lauta[0][5] == nullptr && _lauta[0][6] == nullptr)
			{
				Ruutu e8(4, 0), f8(5, 0), g8(6, 0);

				if (!onkoRuutuUhattu(&e8, vastustaja) &&
					!onkoRuutuUhattu(&f8, vastustaja) &&
					!onkoRuutuUhattu(&g8, vastustaja))
				{
					lista.push_back(Siirto(true, false));
				}
			}
		}
		// Black long castling
		if (_onkoMustaKuningasLiikkunut == false && _onkoMustaDTliikkunut == false)
		{
			if (_lauta[0][4] == mk && _lauta[0][0] == mt &&
				_lauta[0][1] == nullptr && _lauta[0][2] == nullptr && _lauta[0][3] == nullptr)
			{
				Ruutu e8(4, 0), d8(3, 0), c8(2, 0);

				if (!onkoRuutuUhattu(&e8, vastustaja) &&
					!onkoRuutuUhattu(&d8, vastustaja) &&
					!onkoRuutuUhattu(&c8, vastustaja))
				{
					lista.push_back(Siirto(false, true));
				}
			}
		}
	}
}

// Removes pseudo-legal moves that leave own king in check.
void Asema::huolehdiKuninkaanShakeista(list<Siirto>& lista, int vari)
{
	auto it = lista.begin();
	while (it != lista.end())
	{
		Asema testiAsema = *this;

		testiAsema.paivitaAsema(&(*it));

		Nappula* omaKuningas = (vari == 0) ? vk : mk;

		Ruutu kuninkaanPaikka(0, 0);
		bool loytyi = false;

		for (int r = 0; r < 8 && !loytyi; r++)
		{
			for (int c = 0; c < 8; c++)
			{
				if (testiAsema._lauta[r][c] == omaKuningas)
				{
					kuninkaanPaikka = Ruutu(c, r);
					loytyi = true;
					break;
				}
			}
		}

		int vastustaja = 1 - vari;

		if (testiAsema.onkoRuutuUhattu(&kuninkaanPaikka, vastustaja))
			it = lista.erase(it);
		else
			++it;
	}
}

// Full legal move generation
void Asema::annaLaillisetSiirrot(list<Siirto>& lista)
{
	// Generate moves for all pieces of the side to move.
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Nappula* p = _lauta[r][c];

			if (p != nullptr && p->getVari() == _siirtovuoro)
			{
				Ruutu ruutu(c, r);

				p->annaSiirrot(lista, &ruutu, this, _siirtovuoro);
			}
		}
	}

	annaLinnoitusSiirrot(lista, _siirtovuoro);

	huolehdiKuninkaanShakeista(lista, _siirtovuoro);

}
// -------------------------------------------------
// Search move generation
// -------------------------------------------------
// Faster than annaLaillisetSiirrot() because it generates pseudo-legal moves and leaves self-check filtering to maxi/mini.
void Asema::annaHakuSiirrot(list<Siirto>& lista)
{
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Nappula* p = _lauta[r][c];
			if (p != nullptr && p->getVari() == _siirtovuoro)
			{
				Ruutu ruutu(c, r);
				p->annaSiirrot(lista, &ruutu, this, _siirtovuoro);
			}
		}
	}

	annaLinnoitusSiirrot(lista, _siirtovuoro);
}

// Returns true if the given side's king is currently in check.
bool Asema::onkoKuningasShakissa(int omaVari)
{
	Nappula* kingPtr = (omaVari == 0) ? vk : mk;

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (_lauta[r][c] == kingPtr)
			{
				Ruutu k(c, r);
				return onkoRuutuUhattu(&k, 1 - omaVari);
			}
		}
	}
	return false;
}