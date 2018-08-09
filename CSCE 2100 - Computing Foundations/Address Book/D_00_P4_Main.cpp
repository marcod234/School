#include "D_00_P4_Header.h"

int main(void)
{
	string cmd, subcmd, subcmd2, uid, Fname, Lname, info; // used to process commands
	int count = 0, exit = 1, error=0;

	welcome(); // prints menu
	commands();
	list <Person> a_book; // creating list of type class type Person

	while(exit !=0)
	{
		cout << endl << "cmd> ";
		getline(cin, cmd); // whole command
	        if(cmd == "")
		  error = 1;
		size_t pos = cmd.find(' ');//finds the space, if any between commands
		subcmd = cmd.substr(0,pos);// makes substring of first command

		if(subcmd == "add" || subcmd == "ADD")//checks if substring matches any command
		{
                  size_t pos2 = cmd.find(' ',pos+1);
		  if(pos2 == string::npos)
		    error =1;
		  else
		  {
                    subcmd2 = cmd.substr(pos+1, pos2-pos-1); //second command

		    if(subcmd2 == "name" || subcmd2 == "NAME")
		    {
		      size_t pos3 = cmd.find(' ',pos2+1);

		      if(pos3 == string::npos)
			error =1;

		      else
		      {
		      Fname = cmd.substr(pos2+1, pos3-pos2-1); //first name
		      Lname = cmd.substr(pos3+1, cmd.length());//last name
		      add(a_book, Fname, Lname, count, 0);
		      }
		    }

		    else if(subcmd2 == "spouse" || subcmd2 == "SPOUSE")
		    {
		      size_t pos3 = cmd.find(' ', pos2+1);
		      size_t pos4 = cmd.find(' ',pos3+1);

		      if(pos3 == string::npos || pos4 == string::npos)
			error = 1;

		      else
		      {
		        uid = cmd.substr(pos2+1, pos3-pos2-1);
		        Fname = cmd.substr(pos3+1, pos4-pos3-1);
		        Lname = cmd.substr(pos4+1, cmd.length());
		        add_spouse(a_book, uid, Fname, Lname, count);
		      }
		    }

		    else if (subcmd2 == "child" || subcmd2 == "CHILD")
		    {
		      size_t pos3 = cmd.find(' ', pos2+1);
                      size_t pos4 = cmd.find(' ',pos3+1);

                      if(pos3 == string::npos || pos4 == string::npos)
                        error = 1;

		      else
		      {
                        uid = cmd.substr(pos2+1, pos3-pos2-1);
                        Fname = cmd.substr(pos3+1, pos4-pos3-1);
                        Lname = cmd.substr(pos4+1, cmd.length());
		        add_child(a_book, uid, Fname, Lname, count);
		      }
		    }

		    else
		    {
		      size_t pos3 = cmd.find(' ', pos2+1);

		      if(pos3 == string::npos)
		        error = 1;
		      else
		      {
                        uid = cmd.substr(pos2+1, pos3-pos2-1);
		        info = cmd.substr(pos3+1, cmd.length());
		        add_info(a_book, subcmd2, uid, info);
		      }
		    }
		  }
		}
                else if (subcmd == "search" || subcmd == "SEARCH")
		{
		  size_t pos2 = cmd.find(' ',pos+1);

		  if(pos2 == string::npos)
		    error = 1;
		  else
		  {
		    Fname = cmd.substr(pos+1,pos2-pos-1);
		    Lname = cmd.substr(pos2+1, cmd.length());
                    search(a_book, Fname, Lname, 0);
		  }
		}

//		else if (subcmd == "remove" || subcmd == "REMOVE")
//		  remove(a_book, cmd, pos);

                else if (subcmd == "print" || subcmd == "PRINT")
		{

		    if(cmd == "print" || cmd == "PRINT")
			error = 1;

		    else
		    {
                      subcmd2 = cmd.substr(pos+1, cmd.length());

		      if(subcmd2 == "all" || subcmd2 == "ALL")
		        print_all(a_book);

		      else
		        print_uid(a_book, subcmd2);
		    }
		}

                else if (subcmd == "file" || subcmd == "FILE")
		{
		  if(cmd == "file")
		    cout << "ERROR: Incorrect Parameters" << endl;
		  else
		  {
		    size_t pos2 = cmd.find(' ',pos+1);
                    string location = cmd.substr(pos+1,pos2-pos-1);
                    file(a_book, location, count);
		  }
		}

               	else if (subcmd == "quit" || subcmd == "QUIT")
		  exit = quit(a_book);

		else if (subcmd == "commands" || subcmd == "COMMANDS")
		{
		  system("clear"); 
		  commands();
		}

		else if(subcmd == "ftree" || subcmd == "FTREE")
		{
                  if(cmd == "ftree")
                    cout << "ERROR: Incorrect Parameters" << endl;
                  else
                  {
                    size_t pos2 = cmd.find(' ',pos+1);
                    string uid = cmd.substr(pos+1,pos2-pos-1);
                    ftree(a_book, uid);
                  }
		}

		else if(subcmd == "pepe" || subcmd == "PEPE")
		  pepe(); 

		else
		{
		  pepe();
                  cout << "ERROR: Incorrect parameters" << endl; // error if sub string does not match any command
		}
		if(error == 1)
		{
		  pepe();
		  cout << "ERROR: Incorrect parameters" << endl;
		}
		error = 0;
	}
	return 0;
}
