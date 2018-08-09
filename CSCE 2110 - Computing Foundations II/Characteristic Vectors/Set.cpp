/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 2110:001                            |
|                           February 11, 2016                         |
|                                                                     |
|    These are methods used by the Set class in order to implement    |
|    characterisic vectors and use set implementations such as        |
|    unions and intersections.                                                                 |
|                                                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Set.h"

Set::Set() // Constructor
{
	for (int j = 0; j < 4; j++) // initialize int array and count to 0
		bitString[j] = 0;
}

Set::Set(const Set& s) // copy constructor 
{
	for (int i = 0; i < 4; i++) // copy contents of one array to current array
		bitString[i] = s.bitString[i];
}

Set::~Set(){} // Destructor

void Set::add(int i) //add new member
{
	int array_pos = i/32; // position in the array to access, ranges from 0 to 3
	int bit_pos = i - (array_pos * 32); // bit position to access, ranges from 0 to 127
	unsigned int holder = bitString[array_pos]; // temp unsigned int, set equal to current unsigned it at position array_pos

	holder &= 1 << bit_pos; // AND holder and 1 left shifted bit_pos amount of places

	if (!holder) // if holder = 0, bit is not added 
		bitString[array_pos] |= 1 << bit_pos; // OR int at array_pos with 1 left shifted bit_pos amount of places, bit is turned on 

	else
		cout << i << " already added" << endl;
}

void Set::remove(int i)
{
	int array_pos = i / 32;
	int bit_pos = i - (array_pos * 32);
	unsigned int holder = bitString[array_pos];

	holder &= 1 << bit_pos;

	if (holder) // bit is already added
		bitString[array_pos] ^= 1 << bit_pos; // turns bit to zero

	else
		cout << i << " doesn't exist" << endl;
}

int Set::size() // return amount of bits flipped to 1
{
	int set_count = 0;
	for (int i = 0; i < 128; i++)
		if (is_member(i))
			set_count++;

	return set_count;
}

int Set::is_member(int i) // check if bit is added, same process as add function
{
	int array_pos = i / 32;
	int bit_pos = i - (array_pos * 32);
	unsigned int holder = bitString[array_pos];

	holder &= 1 << bit_pos;

	if (!holder)
		return 0; // bit is not a member

	else
		return 1; // bit is a member
}

void Set::operator=(const Set& s) // copy contents of one set to another
{
	for (int i = 0; i < 4; i++)
		bitString[i] = s.bitString[i];
}

Set Set::operator&(const Set& s)
{
	Set p; // temporary Set
	for (int i = 0; i < 4; i++)  
		p.bitString[i] = bitString[i] & s.bitString[i]; // intersection

	return p;
}

Set Set::operator|(const Set& s)
{
	Set p;
	for (int i = 0; i < 4; i++)
		p.bitString[i] = bitString[i] | s.bitString[i]; // union

	return p;
}


Set Set::operator^(const Set& s)
{
	Set p;
	for (int i = 0; i < 4; i++)
		p.bitString[i] = bitString[i] ^ s.bitString[i]; // exclusive or
	return p;
}


Set Set::operator-(const Set& s)
{
	Set p;
	for (int i = 0; i < 4; i++)
		p.bitString[i] = bitString[i] & ~s.bitString[i]; // set difference

	return p;
}


void Set::printSet()
{
	cout << "{ ";
	for (int i = 0; i < 128; i++)
		if (is_member(i))
			cout << i << ", ";
			
	cout << "}" << endl;
}
