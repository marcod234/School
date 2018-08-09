/*
Team D
  Marco Duarte
  Louis Gonzalez
  Reynaldo Quiroz
  Eric Suedemeier
  Josh Tews
  Supriya Thapa

CSCE 2100
Program 4

Matt Walling fixed the makefile
*/

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>

using namespace std;

class Person
{
        private:
	  string Fname;
          string Lname;
          string id;
	  string address;
	  string city;
	  string state;
	  string zip;
	  string date_birth;
	  string date_death;
	  string date_wedding; 
	  string spouse;
        public:
	  list<string> children;

	    //Constructor
          Person(string a, string b, string c){Fname = a; Lname = b; id = c;}

	    //setters
	  void set_Fname(string a){Fname = a;}
          void set_Lname(string a){Lname = a;}
	  void set_address(string a){address = a;}
	  void set_city(string a){city = a;}
	  void set_state(string a){state = a;}
	  void set_zip(string a){zip = a;}
	  void set_date_birth(string a){date_birth = a;}
	  void set_date_death(string a){date_death = a;}
	  void set_date_wedding(string a){date_wedding = a;}
	  void set_spouse(string a){spouse = a;}

	    //getters
          string get_Fname(){return Fname;}
          string get_Lname(){return Lname;}
          string get_id(){return id;}
          string get_address(){return address;}
          string get_city(){return city;}
          string get_state(){return state;}
          string get_zip(){return zip;}
          string get_date_birth(){return date_birth;}
          string get_date_death(){return date_death;}
          string get_date_wedding(){return date_wedding;}
	  string get_spouse(){return spouse;}

	    //sorts by name. http://stackoverflow.com/questions/8121837/sorting-a-list-of-a-custom-type
          bool operator < (Person &b)
	  {
	     if(Lname == b.Lname)
               return(Fname < b.Fname);
	     else
	       return(Lname < b.Lname);
	  }

          friend ostream& operator<<(ostream& os, Person &b);

};
//Function Prototypes
//Parameters include: 
  // a     = list of type Person
  // Fname = First name 
  // Lname = Last name
  // count = conter for people
  // check = checks if search was accesed from the "add" function
//----------------------------------------------------------------------//

  // prints initial welcome message and commnand menu
void welcome();

  // prints commands
void commands();

  // adds a new Person to the list.
string add(list<Person> &a, string Fname, string Lname, int &count, int check);

  // adds a spouse to <uid>
void add_spouse(list<Person> &a, string uid, string Fname, string Lname, int &count);

  // adds a child to <uid>
void add_child(list<Person> &a, string uid, string Fname, string Lname, int &count);

  // fills any info field for any entry
void add_info(list<Person> &a, string subcmd, string uid, string info);

  // searches for the specified person
string search(list<Person> &a, string Fname, string Lname, int check);

  // removes person from the list
void remove(list<Person> &a, string cmd, size_t pos);

  // prints every person in the list
void print_all(list<Person> &a);

  // prints all info for a <uid>
void print_uid(list<Person> &a, string uid);

  // loads data from a file
void file(list<Person> &a, string location, int &count);

  // erases the list and quits the program
int quit(list<Person> &a);

  // prints family tree for uid
void ftree(list<Person> &a, string uid);

  // ;)
void pepe();
