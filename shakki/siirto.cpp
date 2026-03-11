#include "siirto.h"

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

// Constructor for special castling moves.
// In castling, we do not use normal start/end squares the same way as in a normal move,
// so we initialize them to dummy values and store the castling information in the flags.
Siirto::Siirto(bool _lyhytLinna, bool _pitkaLinna)
{
	this->_alkuRuutu = Ruutu(0, 0);
	this->_loppuRuutu = Ruutu(0, 0);
	this->_lyhytLinna = _lyhytLinna;
	this->_pitkaLinna = _pitkaLinna;
	this->_miksikorotetaan = nullptr;
}

// Default constructor.
// Creates a safe "empty" move so all member variables have known values.
Siirto::Siirto()
{
	_alkuRuutu = Ruutu(0, 0);
	_loppuRuutu = Ruutu(0, 0);
	_lyhytLinna = false;
	_pitkaLinna = false;
	_miksikorotetaan = nullptr;
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

// Returns true if this move is short castling (O-O)
bool Siirto::onkoLyhytLinna() 
{
	return _lyhytLinna;
}

// Returns true if this move is long castling (O-O-O)
bool Siirto::onkoPitkälinna() 
{
	return _pitkaLinna;
}
