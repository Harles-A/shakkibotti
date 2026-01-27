#include "ruutu.h"
using namespace std;

// Constructor which runs when you create a Ruutu object with (sarake, rivi)
Ruutu::Ruutu(int sarake, int rivi) 
{
	// Stores the given column index into the private member variable _sarake.
	_sarake = sarake;
	// Stores the given row index into the private member variable _rivi.
	_rivi = rivi;
}

// Getter which returns the row index of this square
int Ruutu::getRivi()
{
	return _rivi;
}

// Getter which returns the column index of this square.
int Ruutu::getSarake()
{
	return _sarake;
}

// Setter which changes the row index stored in this object.
void Ruutu::setRivi(int rivi) 
{
	_rivi = rivi;
}

// Setter which changes the column index stored in this object
void Ruutu::setSarake(int sarake) 
{
	_sarake = sarake;
}
