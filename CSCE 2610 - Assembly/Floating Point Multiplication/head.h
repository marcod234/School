/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                           Richard Ervin Jr                          |
|                            CSCE 2610:001                            |
|                           April  25, 2016                           |
\*-------------------------------------------------------------------*/

//directives
#include <iostream>
#include <iomanip>
#include <cmath>
#include <bitset>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

//floating point number class
class floatP{
	public:
	//constructor
	floatP(unsigned int a) : num(a) {split();}
	
	//splits a float into its constituent parts
	void split();
	
	//field getters
	unsigned int getS(){return s;}
	unsigned int getExp(){return exp;}
	unsigned int getFrac(){return frac;}
	unsigned int getExpS(){return expS;}
	unsigned int getExpNum(){return expNum;}
	int getDecNum(){return decNum;}
	
	private:
	//fields
	unsigned int num , s, frac, exp, expNum, expS;
	int decNum;
};

//add exponent fun
unsigned int addExp(floatP num1, floatP num2, unsigned int& expS);

//multiplying func
string multSig(unsigned int a, unsigned int b);

//normalizing func
void normalize(unsigned int &exp,unsigned int expS, int place, string& product);
