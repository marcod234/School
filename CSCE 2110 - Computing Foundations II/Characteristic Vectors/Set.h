/*****************************************************************************/
//
//  Set.h --- supplied by Phil Sweany, 1/29/2016
//
//  No rights reserved                           
//
/*****************************************************************************/

#pragma once
using namespace std;
#include <iostream>
#include <assert.h>

class Set
{
public:

	Set();
	Set(const Set& s);	// copy constructor
	~Set();
	void add(int i);
	void remove(int i);
	int size();
	int is_member(int i);
	void operator=(const Set& s);
	Set operator&(const Set& s);	// intersection
	Set operator|(const Set& s);	// union
	Set operator^(const Set& s);	// exclusive OR
	Set operator-(const Set& s);	// set difference

	void printSet();

private:   // whatever you want to add, probably including any data
		   // associated with a Set, like maybe an array of unsigned ints

	unsigned int bitString[4];
};

