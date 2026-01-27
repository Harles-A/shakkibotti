#include "siirto.h"
using namespace std;

// Constructor for a normal move: start square -> end square.
Siirto::Siirto(Ruutu _alkuRuutu, Ruutu _loppuRuutu)
{
	// "this->" means "the member variable of this current object".
	// _alkuRuutu/_loppuRuutu on the right side is the input value.
	this->_alkuRuutu = _alkuRuutu; // Stores the starting square inside this Siirto object.
	this->_loppuRuutu = _loppuRuutu; // Store the ending square inside this Siirto object.
	// A normal move is not castling, so both castling flags are false.
	this->_lyhytLinna = false;
	this->_pitkaLinna = false;
}

// Constructor for castling special move.
Siirto::Siirto(bool _lyhytLinna, bool _pitkaLinna) 
{
	this->_lyhytLinna = _lyhytLinna; // Store whether the move is short castling
	this->_pitkaLinna = _pitkaLinna; // Store whether the move is long castling
}

// Getter which returns the starting square of the move
Ruutu Siirto::getAlkuruutu()
{
	return _alkuRuutu;
}

// Getter which returns the ending square of the move
Ruutu Siirto::getLoppuruutu()
{
	return _loppuRuutu;
}

// Returns true if this move is short castling (O-O)..............................<O_O>
bool Siirto::onkoLyhytLinna() 
{
	return _lyhytLinna;
}

// Returns true if this move is long castling (O-O-O)
bool Siirto::onkoPitkälinna() 
{
	return _pitkaLinna;
}
