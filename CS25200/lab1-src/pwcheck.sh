#!/bin/bash
#initialize password strength
STRENGTH=0
#store password to variable
PASSWORD=`cat $1`
#check if password has proper length
if [ ${#PASSWORD} -lt 6 ] || [ ${#PASSWORD} -gt 32 ]; then
	echo "Error: Password length invalid."
	exit 1
else
	#increase password strength by number of characters in password
	let STRENGTH=STRENGTH+${#PASSWORD}
fi

#check if password contains any special characters
if egrep -q [#$\+%@] $1 ; then
	let STRENGTH=STRENGTH+5
fi

#check if password contains a number
if /bin/egrep -q [0-9] $1 ; then
	let STRENGTH=STRENGTH+5
fi

#check if password contains a letter
if /bin/egrep -q [A-Za-z] $1 ; then
	let STRENGTH=STRENGTH+5
fi

#check if password has consecutive alphanumeric characters
if /bin/egrep -q "([A-Za-z0-9])\1+" $1 ; then
	let STRENGTH=STRENGTH-10
fi

#check if password has three or more consecutive lowercase letters
if /bin/egrep -q "[a-z][a-z][a-z]+" $1 ; then
	let STRENGTH=STRENGTH-3
fi

#check if password has three or more consecutive uppercase letters
if /bin/egrep -q "[A-Z][A-Z][A-Z]+" $1 ; then
	let STRENGTH=STRENGTH-3
fi

#check if password has three or more consecutive numbers
if /bin/egrep -q "[0-9][0-9][0-9]+" $1 ; then
	let STRENGTH=STRENGTH-3
fi

#print the total for the password strength
echo Password Score: $STRENGTH
