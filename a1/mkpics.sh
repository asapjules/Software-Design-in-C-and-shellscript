#!/bin/bash
# check to see if no input is given
if [ $# -eq 0 ]
# print error message if so
then
	echo "No arguments given, cannot generate table">&2
	exit 1
fi
#print to stdout the first part of the html file
if [ $# -gt 0 ]
then
	if ! echo $1 | grep -Eq '^[0-9]+$' 
	then
		echo "First argument must be a number">&2
		exit
	fi
	else
	col_num=$1
elif [ $1 -lt 1 ] 
then 
	echo "the number of columns must be greater than or equal to 1"
fi
# print the html file according to the format
echo '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">'
echo '<html>'
echo '  <head>'
echo '    <title>Picture<title>'
echo '  </head>'
echo '  <body>'
echo '    <h1>Pictures</h1>'
echo '<table>'
#loop through everything in the input
shift
col_count=0
for i in $@
do
	#check to see if the file is a jpg or not if so add images until number of columns has reached the desired amount
	if [ `file -b "$1" | cut -c -4` = "JPEG" ]
	then	
		echo '<tr>'
		echo '<td><img src = "pictures/'$i' height=100></td>'
			echo '</tr>'
		col_count=$(($col_count+1))
		if [ $col_count=$col_num ]
		then
			# reset the column count and make a new row
			col_count=0
			echo '</tr>'
		fi
	# otherwise print an error message
	else
		echo "This file is not a JPEG, cannot be added to table">&2
	fi
done
# close the table and end the html file
echo ' </table>'
echo '</body> </html>'