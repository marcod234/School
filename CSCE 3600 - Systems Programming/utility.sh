#================================================================#
#                         Marco Duarte                           #
#                    marcoduarte2@my.unt.edu                     #
#                        CSCE 3600:001                           #
#                      February, 12, 2016                        #
#                                                                #
#  This bash program shows the user all the files in the         #
#  current directory, any active processes the user or another   #
#  specified user is running, and the top ten largest files in   #
#  the current directory                                         #
#                                                                #
#================================================================#



#! /bin/bash

menu() #function to print menu and read the user's choice
{
        echo "+*******************************************************************+"
        echo "Enter one of the following options:                                 |"
        echo "1) List and count all non-hidden files in the current directory.    |"
        echo "2) Check if given user (default = current user) is logged in, then  |"
        echo "... list all active processes for that user.                        |"
        echo "3) List the sizes and names of the 10 largest files and directories |"
        echo "... in the current directory.                                       |"
        echo "4) Exit this shell program.                                         |"
        echo "+*******************************************************************+"
	read -p ">" choice
}

choice=1 #set choice to 1 to enter while loop
test=""  #string variable used to check whether a user is logged in

if [ $# -eq 2 ]; then #if alternate euid is entered
	euid=$2       #euid variable is set to specified euid
	user=$(who | grep $euid) #if grep does not find the euid, user will be empty

	if [[ $user > $test ]]; then
		log="(logged in) :" #grep found euid from who command
	else
		log="(NOT logged in) :" #euid not found
	fi
else
	euid=$USER #euid set to default euid
	log="(logged in) :"
fi

if [ $# -eq 0 ]; then
	echo usage: minor3 name [euid] #no parameters

else
	echo "Good day," $1"! Nice to meet you!"
	while [[ $choice -gt 0 && $choice -lt 4 ]] # 0 < choice < 4
	do
		menu
		case $choice in
			1) echo "==> Total number of files:" $(ls | wc -l)          #ls piped with wc which prints 1 file per line (-l) counts the newlines
			   ls;;						            #prints the files
			2) echo "==> Active processes for" $euid $log
			   ps -f -u $euid;;				            #prints processes full format (-f) for specified user (-u)
			3) echo "==> Size and Name of 10 largest files and directories:"
			   du -s -c * | sort -n -r | head -n 10 | sort -n;;         #du summarizes size of files (-s) and prints a total (-c) aka current dir
										    #sorted numerically (-n) and reversed (-r) greatest to least
										    #head displays 10 results (-n 10)
										    #sorted numerically again (-n) displaying least to greatest

			*) echo "Thanks," $1"! Have a great day!";;		    #wild card for any number not 1,2, or 3
		esac
	done
fi
