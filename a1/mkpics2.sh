#!/bin/sh
# of the number of inputs is 0  print error message
if [ $# -ne 2 ]
then
	echo "Invalid input, input must have a column number and a directory">&2
	exit
fi
# if the number of inputs  is two check to see if the first input is a number
if [ $# -eq 2 ]
# if not then there print an error message
then
	if ! echo $1 | grep -Eq '^[0-9]+$'
        then
                echo "First argument must be a number">&2
                exit
        else
        	col_num=$1
	fi
fi
# if there are more than 2 parameters then print the error message
if [ $# -gt 2 ]
then
	echo "To many parameters, input must have a column number and a directory only">&2
fi
if { $1 -lt 1 ]
then
	echo "the number of columns must be 1 or more">&2
	exit
fi
# for any other case
else
	echo "please use valid parameters only">&2
fi
# print the html file as according to the assignment
echo '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">'
echo '<html>'
echo '  <head>'
echo '    <title>Pictures</title>'
echo ' '
echo '  </head>'
echo '  <body>'
echo '  <h1>Pictures</h1>'
echo ' '
#count the number of directories are in the root folder
dir_count=0
# have a counter for the number of columns
col_count=0
#check to see if the second input is a directory
if [ -d "$2" ]
then
# for each item in the directory, check to see which are years
	for year in "$2"/*
	do
		if [ -d "$year" ]
		then
			# format the year so that it is only numbers
			year_formatted=$(echo $year | cut -c 3-)
			# check to see if the year fits
			dir_count=$(($dir_count+1))
			if echo "$year_formatted" | grep -Eq '^[0-9][0-9][0-9][0-9]$'
			then
				echo "<h2>$year_formatted<h2>"
				echo "<table>"
				echo "  <tr>"
				#check each item in the year folder
				for month in "$year"/*
				do
					#if the current item is a directory
					if [ -d  "$month" ]
					then
						# format the month
						month_formatted=$(echo $month | cut -d "/" -f 3)
						# check  to see that the name has 2 numbers only
						if echo $month_formatted | grep -Eq '^[0-9][0-9]$'
						then
							# check to each file in month
							for img in "$month"/*
							do
								#if the file is a JPEG file then format it to get rid of the ./, year, and month
								if [ `file -b "$img" | cut -d " " -f 1` = "JPEG" ]
								then
									img_formatted=$(echo $img | cut -d "/" -f 4)
									# add the html format with the proper year month and file image
									echo '<td><img src='$year_formatted/$month_formatted/$img_formatted 'height=100></td>'
									# go up in column count then if the column count reaches the desired amount, reset it to 0
									col_count=$(($col_count+1))
									if [ $col_count = $col_num ]
									then
										$col_count=0
										echo  "  </tr>"
									fi
								# if the file is not a jpeg run an errot message
								else
									echo "$img is not a JPEG, cannot be added to table">&2
								fi
							done
						fi
					fi
				done
				# add the end of table format 
				echo "</table>"
			fi
		fi
	done
# if the second input is not a directory, then run an error message
else
	echo "no directory inputted, please use an actual directory">&2
	exit
fi
if [ dir_count=0 ]
then
	echo "<table>"
	echo "</table>"
fi
# finish formating the html file
echo '</body>'
echo '</html>'
