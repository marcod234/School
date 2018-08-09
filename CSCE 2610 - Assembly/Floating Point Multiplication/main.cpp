/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                           Richard Ervin Jr                          |
|                            CSCE 2610:001                            |
|                           April  25, 2016                           |
\*-------------------------------------------------------------------*/

#include "head.h"// the  header file

int main()
{
  //variable declaration
  unsigned int in, in2;
  unsigned int exp, expS, s, result = 0U, frac = 0U;
  
  //prmpting user
  cout << "Enter a single precision floating point (in hex): ";
  cin >> hex >>in;
  cout << "Enter another single precision floating point (in hex): ";
  cin >> hex >> in2;

  //constructing floatP variables
  floatP num = floatP(in);
  floatP num2 = floatP(in2);
  
  //calculating and printing the exponent
  cout << "Step 1 - Add the exponents" << endl;
  exp = addExp(num, num2, expS);
  cout << "Result (in binary): " << bitset<8>(exp) << endl;
  
  //calculating and printing the product
  cout << "\nStep 2 - Multiply the significands" << endl;
  string product = multSig(num.getFrac(), num2.getFrac());
  cout << "Result (in binary): " << setfill('0') << setw(48) << left << product << endl;

  //normalizing and printing the product with updated exponent
  cout << "\nStep 3 - Normalize the significand" << endl;
  normalize(exp, expS, num.getDecNum() + num2.getDecNum(), product);
  cout << "Result of fraction (in binary): " << setfill('0') << setw(23) << left << product << endl;
  cout << "Result of exponent (in binary): " << bitset<8>(exp) << endl;

  //finding and printing the sign bit
  cout << "\nStep 4 - Determine the sign" << endl;
  s = num.getS() ^ num2.getS();
  cout << "Result (in binary): " << s << endl;
  
  //Constucting the final answer and printing the final product
  cout << "\nProduct (in hex): " << endl;
  result |= s << 31;  //adding in the sign bit
  result |= exp << 23;  //adding in the exp
  //constructing the fractional part
  frac = strtol(product.c_str(), NULL, 2);
  result |= frac; //adding in the factional part
  cout << hex << result << endl;
  
  return 0;
  
  
  
} 