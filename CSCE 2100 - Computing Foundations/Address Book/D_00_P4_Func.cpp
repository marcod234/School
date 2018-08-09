#include "D_00_P4_Header.h"

ostream& operator<<(ostream& os, Person &b)
{

	os << endl << "+-------------------------------------------------------------+" << endl;
	os << "           UID:  " << b.id << endl;
	os << "          NAME:  " << b.Fname << ' ' << b.Lname << endl;
	os << "      ADDRESS1:  " << b.address << endl;
	os << "          CITY:  " << b.city << endl;
	os << "         STATE:  " << b.state << endl;
	os << "           ZIP:  " << b.zip << endl;
	os << "    DATE_BIRTH:  " << b.date_birth << endl;
	os << "    DATE_DEATH:  " << b.date_death << endl;
	os << "  DATE_WEDDING:  " << b.date_wedding << endl;
	os << "        SPOUSE:  " << b.spouse << endl;

	return os;
}
void welcome()
{
        cout << "+---------------------------------------------------------------------------------------------------------------------+" << endl;
        cout << "|                                           Welcome to Team D's ADDRESS BOOK                                          |" << endl;
        cout << "+---------------------------------------------------------------------------------------------------------------------+" << endl << endl;
}

void commands()
{
        cout << "+---------------------------------------------------------------------------------------------------------------------+" << endl;
        cout << "|                                                 AVAILABLE COMMANDS:                                                 |" << endl;
        cout << "+---------------------------------------------------------------------------------------------------------------------|" << endl;
        cout << "|  add name <name>                    ==> Add a new name (First Last) to the Address Book                             |" << endl;
	cout << "|  add spouse <uid> <name>            ==> Add spouse of <uid> (First Last) to the Address Book                        |" << endl;
        cout << "|  add child <uid> <name>             ==> Add a child of <uid> (First Last) to the Address Book                       |" << endl;
        cout << "|  add address1 <uid> <address>       ==> Add/change the address for <uid>                                            |" << endl;
        cout << "|  add city <uid> <city>              ==> Add/change the city for <uid>                                               |" << endl;
        cout << "|  add state <uid> <state>            ==> Add/change the state for <uid>                                              |" << endl;
        cout << "|  add zip <uid> <zipcode>            ==> Add/change the zipcode for <uid>                                            |" << endl;
        cout << "|  add date_birth <uid> <ddmmyyyy>    ==> Add/change the birthday for <uid>                                           |" << endl;
        cout << "|  add date_wedding <uid> <ddmmyyyy>  ==> Add/change the wedding day for <uid>                                        |" << endl;
        cout << "|  add date_death <uid> <ddmmyyyy>    ==> Add/Change the date of death for <uid>                                      |" << endl;
        cout << "|  search <name>                      ==> Searches for name (First Last) and returns the <uid>, if found              |" << endl;
//      cout << "|  remove <name>                                                                                                      |" << endl;
        cout << "|  print all                          ==> Prints a list of ALL of the names in the Address Book with their <uid>      |" << endl;
        cout << "|  print <uid>                        ==> Prints all of the fields for <uid>                                          |" << endl;
        cout << "|  file  <filename>                   ==> User is prompted for a filename that contains correctly formatted commands  |" << endl;
	cout << "|                                            --the file must be in CSV format with one full command per line          |" << endl;
	cout << "|  ftree <uid>                        ==> Prints family tree for <uid> (up to 3rd generation, 3 children per parent)  |" << endl;
        cout << "|  quit                               ==> Processing complete                                                         |" << endl;
        cout << "+---------------------------------------------------------------------------------------------------------------------+" << endl << endl;
}

string add(list<Person> &a, string Fname, string Lname, int &count, int check)
{
	stringstream sstm;			      //stringstream used to concatenate a string and counter for uid
	string result;
        list<Person>::iterator i;                     //declare a list iterator i


	if(a.empty())                                 //checks if list is empty
	{
	  count++;                                    //increments counter which is also used for the id's

	  sstm << "ID00" << count;                    //http://stackoverflow.com/questions/191757/c-concatenate-string-and-int
	  result = sstm.str();
	  a.push_front(Person(Fname, Lname, result)); // constructs new Person
	}

	else                                          //if list is NOT empty
	{

	  string test = search(a, Fname, Lname, 1);//checks if name exists already, returns the UID or "false"
	  if(test != "false")//if name exists
          {
	    if(check == 0)//accessed from main
              cout << "DUPLICATE: " << Fname << ' ' << Lname << ' ' << "UID: " << test << endl;
	    if(check == 1)//accessed from add_spouse or add_child
	      cout << "ALREADY IN LIST: " << Fname << ' ' << Lname << ' ' << "UID: " << test << endl; 
            return "false";                                 //if so return
          }

	  count++;

          if (count < 10)
            sstm << "ID00" << count;

          else if (count < 100 && count > 9)
            sstm << "ID0" << count;

	  else if (count > 99)
	    sstm << "ID" << count;

	  result = sstm.str();
          a.push_back(Person(Fname, Lname, result));

	}
	    //Prints who was added and their id
          cout << "ADDED: " << Fname << ' ' << Lname << ' ' << "UID: " << result << endl;
	  return result;
}

void add_spouse(list<Person> &a, string uid, string Fname, string Lname, int &count)
{
	string test = add(a, Fname, Lname, count, 1);

	if(test == "false")
	  return;

	else
	{
	  list<Person>::iterator i;

	  for(i=a.begin(); i!=a.end(); i++)
	    if(uid == (*i).get_id())
	    {
	      (*i).set_spouse((Fname + " " + Lname));
	      break;
	    }
	  list<Person>::iterator j;

	  for(j=a.begin(); j!=a.end(); j++)
	    if(test == (*j).get_id())
	    {
	      (*j).set_spouse(((*i).get_Fname() + " " + (*i).get_Lname()));
	      return;
	    }

	}
	//cout << "UID: " << uid << " NOT FOUND" << endl;
}

void add_child(list<Person> &a, string uid, string Fname, string Lname, int &count)
{
        string test = add(a, Fname, Lname, count, 1);

        if(test == "false")
          return;

        else
        {
          list<Person>::iterator i;
	  list<Person>::iterator j;

          for(i=a.begin(); i!=a.end(); i++)
            if(uid == (*i).get_id())
            {
              (*i).children.push_back((Fname + " " + Lname));
              break;
            }

	  for(j=a.begin(); j!=a.end(); j++)
	    if((*i).get_spouse() == ((*j).get_Fname() + ' ' + (*j).get_Lname()))
	    {
	      (*j).children.push_back((Fname + " " + Lname));
              break;
	    }
        }

}

void add_info(list<Person> &a, string subcmd, string uid, string info)
{
	list<Person>::iterator i;
	int j;
	string upper = subcmd;

	for(j=0; j< subcmd.length(); j++)
	  upper[j] = toupper(upper[j]);

	for(i = a.begin(); i != a.end(); i++)
	  if(uid == (*i).get_id())
	  {
	    if(subcmd == "address1" || subcmd == "ADDRESS1")
		(*i).set_address(info);

	    else if(subcmd == "city" || subcmd == "CITY")
		(*i).set_city(info);

            else if(subcmd == "state" || subcmd == "STATE")
                (*i).set_state(info);

            else if(subcmd == "zip" || subcmd == "ZIP")
                (*i).set_zip(info);

            else if(subcmd == "date_birth" || subcmd == "DATE_BIRTH")
                (*i).set_date_birth(info);

            else if(subcmd == "date_wedding" || subcmd == "DATE_WEDDING")
                (*i).set_date_wedding(info);

            else if(subcmd == "date_death" || subcmd == "DATE_DEATH")
                (*i).set_date_death(info);

	    else
		cout << "ERROR: Incorrect Parameters" << endl;

	    cout << "ADDED " << upper << ": " << info << " UID: " << uid << endl;
	    return; 
	}

	cout << "UID: " << uid << " NOT FOUND" << endl;
}

string search(list<Person> &a, string Fname, string Lname, int check)
{

	if(a.empty())//checks if empty
	{
	  cout << "NO NAMES IN LIST" << endl;
	  return "false";
	}

        list<Person>::iterator i;//iterates through main person

        for(i=a.begin(); i !=a.end(); i++)//searches through list for the name
          if(Fname == (*i).get_Fname() && Lname == (*i).get_Lname())
          {
	    if(check == 0)//checks if "search" was accessed from the "add" functions
              cout << "FOUND: " << Fname << ' ' << Lname << ' ' << "UID: " << (*i).get_id() << endl; //name is in list
            return (*i).get_id();
          }

	if(check == 0)
          cout << "NOT FOUND: " << Fname << ' ' << Lname << endl;//name not in list
	return "false";
}

void remove(list<Person> &a, string cmd, size_t pos)
{/*
        string Lname, test;

        if(cmd == "remove" || cmd == "REMOVE") // checks if there are enough parameters
        {
          cout << "ERROR: Incorrect Parameters" << endl;
          return;
        }

        if(a.empty())//checks if empty
        {
          cout << "NO NAMES IN LIST" << endl;
          return;
        }

        Lname = cmd.substr(pos+1, cmd.length());// gets name

        list<Person>::iterator i;

        for(i=a.begin(); i!=a.end(); i++)// searches for name
          if(Lname == (*i).get_Lname())
          {
            a.erase(i);
            cout << "Removal successful: " << Lname << endl; //name exists and is removed
            return;
          }
        cout << "ERROR: Not found for Removal: " << Lname << endl; // name does not exist
*/
}


void print_all(list<Person> &a)
{
        if(a.empty())// checks if empty
        {
          cout << "NO NAMES IN LIST" << endl;
          return;
        }

        else
        {
          a.sort(); // sorts names alphabetically
          list<Person>::iterator i;

          for(i = a.begin(); i != a.end(); i++) // prints names and ids
            cout << (*i).get_Fname() << ' ' << (*i).get_Lname() << " UID: " << (*i).get_id() << endl;
        }
}

void print_uid(list<Person> &a, string uid)
{
	list<Person>::iterator i;
	int k =1;

	if(a.empty())
	 cout << "NO NAMES IN LIST" << endl;

	else
	{
	  for(i=a.begin(); i != a.end(); i++)

	    if(uid == (*i).get_id())
	    {
	      cout << (*i); // overloaded operator
	      list<string>::iterator j;

	      for(j = (*i).children.begin(); j!=(*i).children.end(); j++)
	      {
	        cout << "        CHILD" << k << ":  " << (*j) << endl;
	        k++;
	      }
	      cout << "+-------------------------------------------------------------+" << endl << endl;
	      return;
	    }

	  cout << "UID: " << uid << " NOT FOUND" << endl;
	}
}

void file(list<Person> &a, string location, int &count)
{

        ifstream file;
        string cmd, subcmd, subcmd2;

        file.open(location.c_str()); // opens file
        if(file.is_open()) // checks if file was opened 
        {
          cout << "opening file..." << endl << endl;
          while(getline(file, cmd)) //reads until end of file
          {
                size_t pos = cmd.find(',');//finds commas
		cmd.erase(cmd.end()-1);//removes carriage return
                subcmd = cmd.substr(0,pos);// makes substring of first command

                if(subcmd == "add" || subcmd == "ADD")//checks if substring matches any command
                {
                  size_t pos2 = cmd.find(',',pos+1);
                  string subcmd2 = cmd.substr(pos+1, pos2-pos-1); //second command

                  if(subcmd2 == "name" || subcmd2 == "NAME")
                  {
                    size_t pos3 = cmd.find(' ',pos2+1);
		    size_t pos4 = cmd.find(',',pos3+1);
                    string Fname = cmd.substr(pos2+1, pos3-pos2-1); //first name
                    string Lname = cmd.substr(pos3+1, pos4-pos3-1);//last name
                    add(a, Fname, Lname, count, 0);
//cout << Fname << endl << Lname << endl;
                  }
                  else if(subcmd2 == "spouse" || subcmd2 == "SPOUSE")
                  {
                    size_t pos3 = cmd.find(',', pos2+1);
                    size_t pos4 = cmd.find(' ',pos3+1);
                    string uid = cmd.substr(pos2+1, pos3-pos2-1);
                    string Fname = cmd.substr(pos3+1, pos4-pos3-1);
                    string Lname = cmd.substr(pos4+1, cmd.length());
                    add_spouse(a, uid, Fname, Lname, count);
//cout << uid << endl << Fname << endl << Lname << endl;
                  }

                  else if (subcmd2 == "child" || subcmd2 == "CHILD")
                  {
                    size_t pos3 = cmd.find(',', pos2+1);
                    size_t pos4 = cmd.find(' ',pos3+1);
                    string uid = cmd.substr(pos2+1, pos3-pos2-1);
                    string Fname = cmd.substr(pos3+1, pos4-pos3-1);
                    string Lname = cmd.substr(pos4+1, cmd.length());
                    add_child(a, uid, Fname, Lname, count);

//cout << uid << endl << Fname << endl << Lname << endl;
                  }
                  else
                  {
                    size_t pos3 = cmd.find(',', pos2+1);
                    string uid = cmd.substr(pos2+1, pos3-pos2-1);
                    string info = cmd.substr(pos3+1, cmd.length());
                    add_info(a, subcmd2, uid, info);

//cout << subcmd2 << endl << uid << endl << info << endl;
                  }
                }
                else if (subcmd == "search" || subcmd == "SEARCH")
                {
                  size_t pos2 = cmd.find(' ',pos+1);
		  size_t pos3 = cmd.find(',',pos2+1);

                  string Fname = cmd.substr(pos+1,pos2-pos-1);
                  string Lname = cmd.substr(pos2+1, pos3-pos2-1);
                  search(a, Fname, Lname, 0);
//cout << Fname << endl << Lname << endl;
                }

                else if (subcmd == "print" || subcmd == "PRINT")
                {
                  size_t pos2 = cmd.find(',',pos+1);
		  size_t pos3 = cmd.find(',',pos2+1);
                  subcmd2 = cmd.substr(pos+1, pos2-pos-1);

                  if(subcmd2 == "all" || subcmd2 == "ALL")
                    print_all(a);
	//		cout << subcmd2 << endl;

                  else
                    print_uid(a, subcmd2);
	//	  cout << subcmd2 << endl;
                }

                else if(subcmd == "ftree" || subcmd == "FTREE")
                {
                  if(cmd == "ftree")
                    cout << "ERROR: Incorrect Parameters" << endl;
                  else
                  {
                    size_t pos2 = cmd.find(' ',pos+1);
                    string uid = cmd.substr(pos+1,pos2-pos-1);
                    ftree(a, uid);
                  }
                }

                else if(subcmd == "pepe" || subcmd == "PEPE")
                  pepe();

          }

          file.close(); // closes file
        }

        else
          cout << "ERROR: CANNOT OPEN INPUT FILE" << endl; //file not opened

}

int quit (list<Person> &a)
{
	list<Person>:: iterator i;

	for(i=a.begin(); i != a.end();i++)//erases spouse and children for each "name"
	{
	  (*i).children.erase((*i).children.begin(), (*i).children.end());
	}

        a.erase(a.begin(), a.end()); // erases list
	cout << "Have a good day and a pleasant tomorrow! Good-bye." << endl << endl;
        return 0;
}

void pepe()
{
  cout << "          ████████_____██████" << endl;
  cout << "         █░░░░░░░░██_██░░░░░░█" << endl;
  cout << "        █░░░░░░░░░░░█░░░░░░░░░█" << endl;
  cout << "       █░░░░░░░███░░░█░░░░░░░░░█" << endl;
  cout << "       █░░░░███░░░███░█░░░████░█" << endl;
  cout << "      █░░░██░░░░░░░░███░██░░░░██" << endl;
  cout << "     █░░░░░░░░░░░░░░░░░█░░░░░░░░███" << endl;
  cout << "    █░░░░░░░░░░░░░██████░░░░░████░░█" << endl;
  cout << "     █░░░░░░░░░█████░░░████░░██░░██░░█" << endl;
  cout << "    ██░░░░░░░███░░░░░░░░░░█░░░░░░░░███" << endl;
  cout << "   █░░░░░░░░░░░░░░█████████░░█████████" << endl;
  cout << "  █░░░░░░░░░░█████_████___████_█████___█" << endl;
  cout << "  █░░░░░░░░░░█______█_███__█_____███_█___█" << endl;
  cout << " █░░░░░░░░░░░░█___████_████____██_██████" << endl;
  cout << " ░░░░░░░░░░░░░█████████░░░████████░░░█" << endl;
  cout << " ░░░░░░░░░░░░░░░░█░░░░░█░░░░░░░░░░░░█" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░░░██░░░░█░░░░░░██" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░██░░░░░░░███████" << endl;
  cout << " ░░░░░░░░░░░░░░░░██░░░░░░░░░░█░░░░░█" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█" << endl;
  cout << " ░░░░░░░░░░░█████████░░░░░░░░░░░░░░██" << endl;
  cout << " ░░░░░░░░░░█▒▒▒▒▒▒▒▒███████████████▒▒█" << endl;
  cout << " ░░░░░░░░░█▒▒███████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█" << endl;
  cout << " ░░░░░░░░░█▒▒▒▒▒▒▒▒▒█████████████████" << endl;
  cout << " ░░░░░░░░░░████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░██████████████████" << endl;
  cout << " ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█" << endl;
  cout << " ██░░░░░░░░░░░░░░░░░░░░░░░░░░░██" << endl;
  cout << " ▓██░░░░░░░░░░░░░░░░░░░░░░░░██" << endl;
  cout << " ▓▓▓███░░░░░░░░░░░░░░░░░░░░█" << endl;
  cout << " ▓▓▓▓▓▓███░░░░░░░░░░░░░░░██" << endl;
  cout << " ▓▓▓▓▓▓▓▓▓███████████████▓▓█" << endl;
  cout << " ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██" << endl;
  cout << " ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█" << endl;
  cout << " ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█" << endl;

}
