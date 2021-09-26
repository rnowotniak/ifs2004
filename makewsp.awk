#!/usr/bin/awk -f

{
	OFS = "</TD><TD>";
	print "<TR>";
	print "<TD>" $2, $3, $4, $5, $6, $7 "</TD>";
	print "</TR>";
}
