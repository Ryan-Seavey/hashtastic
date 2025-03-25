#!/usr/bin/env bash

# make a CSV file with everything in it

CMD="./HashStats.py"
CSV="hashes.csv"
TMP=$(mktemp)

if [[ -f $CSV ]]
then
	rm $CSV
fi

for SRC in a ones zero random shak
do
	echo "filetype_$SRC" >> $CSV
	$CMD -i $SRC/$SRC.orig -c $TMP
	cat $TMP >> $CSV
	echo >> $CSV
	echo "filetype_$SRC-alt" >> $CSV
	$CMD -i $SRC/$SRC.alt -a -c $TMP
	cat $TMP >> $CSV
	echo >> $CSV
done

echo "Done."
echo "See your output data in 'hashes.csv'"
