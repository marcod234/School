// main.cpp : Defines the entry point for the console application.
//
#include "Set.h"

int main()
{
	// *******************************************
	//   
	//  This file is meant to give you SOME idea   
	//  of how we'll test your code.  We WILL use
	//  this program as ONE of our tests.  But 
	//  there WILL be others.   
	//   
	// ******************************************* 

	/*   Commented out for now since I don't have a
	Set.C to test.  That's YOUR job students!.
	However, don't be surprised if, when you
	uncomment this, that there are syntax errors.
	I'm sure you'll get them fixed and then email
	them to me.
	*/

	Set s1, s2, s3;

	s1.add(27);
	s1.add(0);
	s1.add(109);
	cout << "s1: ";
	s1.printSet();

	s2.add(43);
	s2.add(34);
	s2.add(27);
	cout << "s2: ";
	s2.printSet();

	cout << "s3: ";
	s3.printSet();

	s3 = s1 & s2;
	cout << "The intersection of s1 and s2 is ";
	s3.printSet();

	s3 = s1 | s2;
	cout << "The union of s1 and s2 is ";
	s3.printSet();

	s3 = s1 ^ s2;
	cout << "s1 xor s2 is ";
	s3.printSet();

	s3 = s1 - s2;
	cout << "s1 minus s2 is ";
	s3.printSet();

	s1.add(27);



	

	return 0;
}
