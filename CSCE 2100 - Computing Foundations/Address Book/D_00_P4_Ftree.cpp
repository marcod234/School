#include "D_00_P4_Header.h"

void ftree(list<Person> &a, string uid)
{
	if(a.empty())
	{
	  cout << "NO NAMES IN LIST" << endl;
	  return;
	}

	list<Person>::iterator i,h,l,m,n;
	list<string>::iterator j;

	for(i = a.begin(); i != a.end(); i++)
	{
	  if(uid == (*i).get_id())
	  {
	    string fl,F,L,f2,l2;
	    F = (*i).get_Fname();
	    L = (*i).get_Lname();
	    fl = (*i).get_spouse(); 
	    size_t pos = fl.find(' ');
	    f2 = fl.substr(0,pos);
	    l2 = fl.substr(pos+1, fl.length());


	    cout << " +--------+" << endl;
	    cout << " | " << F; for(size_t k = F.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
            cout << " | " << L; for(size_t k = L.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
	    cout << " |        |" << endl;
            cout << " | " << f2; for(size_t k = f2.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
            cout << " | " << l2; for(size_t k = l2.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
            cout << " +--------+" << endl;

	    if(!(*i).children.empty())
	    {
	      int count = 0;
	      size_t Gen2, Gen21, Gen22, Gen23;
	      string lblanks, lblanks2, blanks, blanks2, hyphens, hyphens2, line = "     |    ", line_blank = "          ";
	      Gen2 = (*i).children.size();


	      for(j = (*i).children.begin(); j!=(*i).children.end(); j++)
	      {
		count ++;

		for(h = a.begin(); h!= a.end(); h++)
		{
		  //cout << (*h).get_Fname() << ' ' << (*h).get_Lname() << endl;
		  if((*j) == ((*h).get_Fname() + ' ' + (*h).get_Lname()))
		    if(count == 1)
		       l = h;
		    else if ( count == 2)
		       m = h;
		    else if (count == 3)
		       n = h;
		}
	      }

	      if(count >= 1)
	      {
	        if(!(*l).children.empty())
		  Gen21 = (*l).children.size();

	        else
		  Gen21 = 0;
	      }

	      if(count >= 2)
	      {
	        if(!(*m).children.empty())
		  Gen22 = (*m).children.size();

                else
                  Gen22 = 0;
	      }

	      if(count >= 3)
	      {
	        if(!(*n).children.empty())
		  Gen23 = (*n).children.size();

                else
                  Gen23 = 0;
	      }

	      if(Gen2 == 1)
		Gen22 = Gen23 = 0;
	      else if (Gen2 == 2)
		Gen23 = 0;

	      cout << "     |" << endl;
              cout << "     |" << endl;
              cout << "     |" << endl;


	      if (Gen21 == 0)
	      {
		blanks = "                 ";
		hyphens = "-------";
		lblanks = "       ";
	      }

              else if (Gen21 == 1)
              {
                blanks = "       ";
                hyphens = "-------";
                lblanks = "       ";
              }


              else if (Gen21 == 2)
              {
                blanks = "       ";
                hyphens = "---------------------";
                lblanks = "                     ";
              }

              else if (Gen21 == 3)
              {
                blanks = "       ";
                hyphens = "-----------------------------------";
                lblanks = "                                   ";
              }

              if (Gen22 == 0)
              {
                blanks2 = "                 ";
                hyphens2 = "-------";
                lblanks2 = "       ";
              }

              else if (Gen22 == 1)
              {
                blanks2 = "       ";
                hyphens2 = "-------";
                lblanks2 = "       ";
              }

              else if (Gen22 == 2)
              {
                blanks2 = "       ";
                hyphens2 = "---------------------";
                lblanks2 = "                     ";
              }

              else if (Gen22 == 3)
              {
                blanks2 = "       ";
                hyphens2 = "-----------------------------------";
                lblanks2 = "                                   ";
              }

	      if(Gen2 == 1)
	      {
                string fl,F,L,f2,l2;
                F = (*l).get_Fname();
                L = (*l).get_Lname();
                fl = (*l).get_spouse();
                size_t pos = fl.find(' ');
                f2 = fl.substr(0,pos);
                l2 = fl.substr(pos+1, fl.length());


                cout << " +--------+" << endl;
                cout << " | " << F; for(size_t k = F.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
                cout << " | " << L; for(size_t k = L.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
                cout << " |        |" << endl;
                cout << " | " << f2; for(size_t k = f2.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
                cout << " | " << l2; for(size_t k = l2.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
                cout << " +--------+" << endl;
	      }

	      else if(Gen2 == 2)
	      {
                string fl,F,L,f2,l2,fl2,f21,l21,f22,l22;
                F = (*l).get_Fname();
                L = (*l).get_Lname();
                fl = (*l).get_spouse();
                size_t pos = fl.find(' ');
                f2 = fl.substr(0,pos);
                l2 = fl.substr(pos+1, fl.length());

                f21 = (*m).get_Fname();
                l21 = (*m).get_Lname();
                fl2 = (*m).get_spouse();
                size_t pos2 = fl2.find(' ');
                f22 = fl2.substr(0,pos2);
                l22 = fl2.substr(pos2+1, fl.length());



                cout << " +--------+" << lblanks << "+--------+" << endl;
                cout << " | " << F; for(size_t k = F.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << f21; for(size_t k = f21.length(); k <7; k++) cout << ' ';  cout << "|" << endl;

                cout << " | " << L; for(size_t k = L.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << l21; for(size_t k = l21.length(); k <7; k++) cout << ' ';  cout << "|" << endl;

                cout << " |        |" << hyphens << "|        |" << endl;
                cout << " | " << f2; for(size_t k = f2.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << f22; for(size_t k = f22.length(); k <7; k++) cout << ' ';  cout << "|" << endl;

                cout << " | " << l2; for(size_t k = l2.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << l22; for(size_t k = l22.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
                cout << " +--------+" << lblanks << "+--------+" << endl;
	      }

              else if(Gen2 == 3)
              {
                string fl,F,L,f2,l2, fl2,f21,l21,f22,l22, fl3,f23,l23,f24,l24;
                F = (*l).get_Fname();
                L = (*l).get_Lname();
                fl = (*l).get_spouse();
                size_t pos = fl.find(' ');
                f2 = fl.substr(0,pos);
                l2 = fl.substr(pos+1, fl.length());

                f21 = (*m).get_Fname();
                l21 = (*m).get_Lname();
                fl2 = (*m).get_spouse();
                size_t pos2 = fl2.find(' ');
                f22 = fl2.substr(0,pos2);
                l22 = fl2.substr(pos2+1, fl2.length());

                f23 = (*n).get_Fname();
                l23 = (*n).get_Lname();
                fl3 = (*n).get_spouse();
                size_t pos3 = fl3.find(' ');
                f24 = fl3.substr(0,pos3);
                l24 = fl3.substr(pos3+1, fl3.length());


                cout << " +--------+" << lblanks << "+--------+" << lblanks2 << "+--------+" << endl;
                cout << " | " << F; for(size_t k = F.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << f21; for(size_t k = f21.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks2;
                cout << "| " << f23; for(size_t k = f23.length(); k <7; k++) cout << ' ';  cout << "|" << endl;

                cout << " | " << L; for(size_t k = L.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << l21; for(size_t k = l21.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks2;
                cout << "| " << l23; for(size_t k = l23.length(); k <7; k++) cout << ' ';  cout << "|" << endl;

                cout << " |        |" << hyphens << "|        |" << hyphens2 << "|        |" << endl;
                cout << " | " << f2; for(size_t k = f2.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << f22; for(size_t k = f22.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks2;
                cout << "| " << f24; for(size_t k = f24.length(); k <7; k++) cout << ' ';  cout << "|" << endl;

                cout << " | " << l2; for(size_t k = l2.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks;
                cout << "| " << l22; for(size_t k = l22.length(); k <7; k++) cout << ' ';  cout << "|" << lblanks2;
                cout << "| " << l24; for(size_t k = l24.length(); k <7; k++) cout << ' ';  cout << "|" << endl;
                cout << " +--------+" << lblanks << "+--------+" << lblanks2 << "+--------+" << endl;
	      }

	      if(Gen21 == 0 && Gen22 == 0 && Gen23 == 0)
		return;


		for(int z = 0; z < 3; z++)
		{
			if(Gen21 == 0)
			  cout << line_blank;

			else
			  cout << line;

			cout << lblanks;

			if(Gen22 == 0)
                          cout << line_blank;

                        else
                          cout << line;

			cout << lblanks2;

                        if(Gen23 == 0)
                          cout << line_blank;

                        else
                          cout << line;

			cout << endl;
		}


	      list<Person>::iterator o,p,q,r,s,t,u,v,w,x;
	      int count2=0;

		if(Gen21 != 0)
                for(j = (*l).children.begin(); j!=(*l).children.end(); j++)
                {
                  count2 ++;

                  for(h = a.begin(); h!= a.end(); h++)
                  {
                    if((*j) == ((*h).get_Fname() + ' ' + (*h).get_Lname()))
                      if(count2 == 1)
                         o = h;
                      else if ( count2 == 2)
                         p = h;
                      else if (count2 == 3)
                         q = h;
                  }
                }

		count2 = 0;
		if(Gen22 != 0)
                for(j = (*m).children.begin(); j!=(*m).children.end(); j++)
                {
                  count2 ++;

                  for(h = a.begin(); h!= a.end(); h++)
                  {
                    if((*j) == ((*h).get_Fname() + ' ' + (*h).get_Lname()))
                      if(count2 == 1)
                         r = h;
                      else if ( count2 == 2)
                         s = h;
                      else if (count2 == 3)
                         t = h;
                  }
                }

		count2 = 0;
		if(Gen23 != 0)
                for(j = (*n).children.begin(); j!=(*n).children.end(); j++)
                {
                  count2 ++;

                  for(h = a.begin(); h!= a.end(); h++)
                  {
                    if((*j) == ((*h).get_Fname() + ' ' + (*h).get_Lname()))
                      if(count2 == 1)
                         u = h;
                      else if ( count2 == 2)
                         v = h;
                      else if (count2 == 3)
                         w = h;
                  }
                }

		list<Person>::iterator  array[9/*(Gen21+Gen22+Gen23)*/];

		if(Gen21 != 0)
		for(int qw = 0; qw < Gen21; qw++)
		{
		  array[qw] = o;
		  if(qw == 1)
		    array[qw] = p;
		  if(qw == 2)
		    array[qw] = q;
		}

		if(Gen22 != 0)
                for(int qw = 3/*Gen21*/; qw < (6/*Gen21+Gen22*/); qw++)
                {
                  array[qw] = r;
                  if(qw == 4)
                    array[qw] = s;
                  if(qw == 5)
                    array[qw] = t;
                }

		if(Gen23 != 0)
                for(int qw = (6/*Gen21+Gen22*/); qw < (9/*Gen21+Gen22+Gen23*/); qw++)
                {
                  array[qw] = u;
                  if(qw == 7)
                    array[qw] = v;
                  if(qw == 8)
                    array[qw] = w;
                }

		for(int count3 = 0; count3 <7; count3++)
		{
		  cout << " ";

		  for(int count4 = 0; count4 < (9/*Gen21+Gen22+Gen23*/); count4++)
		  {
                    string fl,F,L,f2,l2;

		    if(Gen21 == 0 && count4 == 0 )
		    {
		      cout << blanks;
		      count4 = 3;
		    }
		    else if(Gen21 == 0 && Gen22 == 0)
		    {
		      cout << blanks << blanks2;
		      count4 = 6;
		    }

		    if(Gen22 == 0 && count4 == 3)
		    {
		      cout << blanks2;
		      count4 = 6;
		    }

		    if(Gen23 == 0 && count4 == 6)
		      break;

		    x = array[count4];
//cout << count4;
		    if(count3 == 1)
                      F = (*x).get_Fname();
		    if(count3 == 2)
                      L = (*x).get_Lname();
		    if(count3 == 4 || count3 == 5)
		    {
                      fl = (*x).get_spouse();
                      size_t pos = fl.find(' ');
                      f2 = fl.substr(0,pos);
                      l2 = fl.substr(pos+1, fl.length());
		    }

		    switch(count3)
		    {
		      case 0:
		      cout << "+--------+"; 
		      if(count4 == (Gen21-1))
		      {
		        cout << blanks;
			count4 = 2;
			break;
		      } 
		      else if(count4 == (2+Gen22))
		      {
			cout << blanks2;
			count4 = 5;
			break;
		      }
		      else if(count4 == (5+Gen23))
		      {
                        cout << blanks;
                        count4 = 9;
		      }
		      else
			cout << "    ";
		      break;

		      case 1:
		      cout << "| " << F; for(size_t k = F.length(); k <7; k++) cout << ' ';  cout << "|";
                      if(count4 == (Gen21-1))
                      {
                        cout << blanks;
                        count4 = 2;
                        break;
                      }
                      else if(count4 == (2+Gen22))
                      {
                        cout << blanks2;
                        count4 = 5;
                        break;
                      }
                      else if(count4 == (5+Gen23))
                      {
                        cout << blanks;
                        count4 = 9;
                      }
                      else
                        cout << "    ";
		      break;

                      case 2:
                      cout << "| " << L; for(size_t k = L.length(); k <7; k++) cout << ' ';  cout << "|";
                      if(count4 == (Gen21-1))
                      {
                        cout << blanks;
                        count4 = 2;
                        break;
                      }
                      else if(count4 == (2+Gen22))
                      {
                        cout << blanks2;
                        count4 = 5;
                        break;
                      }
                      else if(count4 == (5+Gen23))
                      {
                        cout << blanks;
                        count4 = 9;
                      }
                      else
                        cout << "    ";
                      break;

                      case 3:
		      cout << "|        |";
                      if(count4 == (Gen21-1))
                      {
                        cout << blanks;
                        count4 = 2;
                        break;
                      }
                      else if(count4 == (2+Gen22))
                      {
                        cout << blanks2;
                        count4 = 5;
                        break;
                      }
                      else if(count4 == (5+Gen23))
                      {
                        cout << blanks;
                        count4 = 9;
                      }
                      else
                        cout << "----";
                      break;

                      case 4:
                      cout << "| " << f2; for(size_t k = f2.length(); k <7; k++) cout << ' ';  cout << "|";
                      if(count4 == (Gen21-1))
                      {
                        cout << blanks;
                        count4 = 2;
                        break;
                      }
                      else if(count4 == (2+Gen22))
                      {
                        cout << blanks2;
                        count4 = 5;
                        break;
                      }
                      else if(count4 == (5+Gen23))
                      {
                        cout << blanks;
                        count4 = 9;
                      }
                      else
                        cout << "    ";
                      break;

                      case 5:
                      cout << "| " << l2; for(size_t k = l2.length(); k <7; k++) cout << ' ';  cout << "|";
                      if(count4 == (Gen21-1))
                      {
                        cout << blanks;
                        count4 = 2;
                        break;
                      }
                      else if(count4 == (2+Gen22))
                      {
                        cout << blanks2;
                        count4 = 5;
                        break;
                      }
                      else if(count4 == (5+Gen23))
                      {
                        cout << blanks;
                        count4 = 9;
                      }
                      else
                        cout << "    ";
                      break;

                      case 6:
		      cout << "+--------+";
                      if(count4 == (Gen21-1))
                      {
                        cout << blanks;
                        count4 = 2;
                        break;
                      }
                      else if(count4 == (2+Gen22))
                      {
                        cout << blanks2;
                        count4 = 5;
                        break;
                      }
                      else if(count4 == (5+Gen23))
                      {
                        cout << blanks;
                        count4 = 9;
                      }
                      else
                        cout << "    ";
                      break;
		   }

 		  }

		  cout << endl;

	        }
		return;
	    }
	    return;
	  }
	}

	cout << "UID: " << uid << " NOT FOUND" << endl;
}