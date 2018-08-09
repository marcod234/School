/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                           Richard Ervin Jr                          |
|                            CSCE 2610:001                            |
|                           April  25, 2016                           |
\*-------------------------------------------------------------------*/

#include "head.h"

//splits float into its constituent part
void floatP::split(){
	//variable declaration
	stringstream sstm;
	string decimals;
	size_t pos;
	
	//extracting s
	s = num >> 31;
	
	//extracting exp
	exp = num << 1;
	exp = exp >> 24;
	//taking into a account the negative potential
	if(exp < 127){
		expNum = 127 - exp;
		expS = 1;
	}
	else{
		expNum = exp - 127;
		expS = 0;
	}
	
	//extracting frac
	frac = num << 9;
  frac >>= 9;
	
	//finding the number of decimal places
	sstm << bitset<23>(frac);
	decimals = sstm.str();
	pos = decimals.find_last_of('1');
	decNum = pos+1;
}

//adds exponents
unsigned int addExp(floatP num1, floatP num2, unsigned int &expS){
	//variable declaration
	unsigned int sum;

	//negative exponents
	if(num1.getExpS() ^ num2.getExpS() == 1){ 
		//determining which is negative
		if(num1.getExpS() == 1){
			//determining what the resulting sign will be
			if(num1.getExpNum() > num2.getExpNum()){
				sum = 127 - (num1.getExpNum() - num2.getExpNum());
				expS = 1;
			}
			else{
				sum = 127 + (num2.getExpNum() - num1.getExpNum());
				expS = 0;
			}
		}
		else{
			//determining which is negative
			if(num2.getExpNum() > num1.getExpNum()){
				sum = 127 - (num2.getExpNum() - num1.getExpNum());
				expS = 1;
			}
			else{
				sum = 127 + (num1.getExpNum() - num2.getExpNum());
				expS = 0;
			}
		}
	}
	// both  negative
	else if(num1.getExpS() == 1) {
		sum = 127 - (num1.getExpNum() + num2.getExpNum());
		expS = 1;
	}
	//both positive
	else {
		sum = 127 + (num1.getExpNum() + num2.getExpNum());
		expS = 0;
	}
	
	return sum; 
}

//multiplies the fractional part of the numbers
string multSig(unsigned int a, unsigned int b){
	//variable declaration
	stringstream sstm;
	string product;
	unsigned int multiplier = a + pow(2,23);
	unsigned long high = 0UL, low = 0UL;
	unsigned long multiplicand = 0UL, mhigh = 0UL;
	multiplicand = b + pow(2,23);

	//simulating multiplication
	for(int i = 0; i < 24; i++){
		if(multiplier % 2 == 1){
			if(i < 8)
			  low += multiplicand;
			else
				high += mhigh;
		}
		multiplier >>= 1;
		if(i < 8)
			multiplicand <<= 1;
		else if(i >= 8 && i < 12 )
		{
			mhigh <<= 1;
			mhigh += multiplicand >> (31);
			multiplicand <<= 1;
		}
		else
			mhigh <<= 1;
	}
	
	//printing the product
	sstm << bitset<32>(high) << bitset<32>(low) << endl;
	product = sstm.str();
	size_t pos = product.find('1');
	product = product.substr(pos, 24);

	return product;
}

//normalizing the product
void normalize(unsigned int &exp,unsigned int expS, int place, string& product){
	size_t pos = product.find_last_of('1');
	pos -= place;
	
	//determining if already normalized
	if(product[pos] == '1') { 
		product.erase(product.begin()+pos);
		return;
	}
	//determining if the exponent is pos or neg
	if(expS == 1)
		exp -= pos;
	else
		exp += pos;
	
	//removing the leading one
	product.erase(product.begin());
}

