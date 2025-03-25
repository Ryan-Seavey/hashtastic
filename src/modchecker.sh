#/usr/bin/env bash

# modchecker -- check all orig/mod files to make sure corresponding files differ

echo "If any files are reported as identical, they need to be fixed."

for filetype in ones a random shak zero
do		
	echo
	echo "Checking files from '$filetype'"
	for group in orig mod
	do
		echo -n "Checking that there are 10k files:"
		count=$(ls $filetype/$group/* | wc -l)
		if (( $count != 10000 ))
		then
			echo "There weren't enough files (should be 10000, was $count)!"
			exit 1
		else
			echo " OK."
		fi

		echo -n "Checking that no $filetype/$group files are duplicates..."
		hashfile=$(mktemp)
		sha256sum $filetype/$group/* > $hashfile
		matches=$(awk '{ print $1 }' $hashfile | sort | uniq -d)
		if [[ ! -z $matches ]]
		then
			echo "Found duplicates!"
			for match in $matches
			do
				grep $match $hashfile
			done
			exit 1
		fi
		echo " OK."

	done

	echo -n "Check that orig-mod corresponding files are not identical (please wait)..."
	for i in {1..10000} 
	do 
		if diff -q --report-identical-files $filetype/orig/$i $filetype/mod/$i | grep -i identical
		then
			echo
			echo "$filetype/orig/$i and $filetype/mod/$i match!"
			exit 1
		fi
	done	
	echo " OK."

done
