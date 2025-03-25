#!/usr/bin/env bash

OPTIND=""
RUNNER="./runner"
START=1
END=10000

usage() {

	cat <<DOC
MakeHashes.sh -- create all hash files for testing
usage: MakeHashes.sh [-hs] file_to_hash

Switches:

	-h		Show this help screen
	-s		Use sha256sum instead of runner
DOC

}

# use getopts to collect parameters
while getopts "hs" OPTION
do
	case $OPTION in
		h)
			usage
			exit 1
			;;
		s)
			RUNNER="sha256sum"
			;;
	esac
done

if [[ $RUNNER == "sha256sum" ]]
then
	if ! which sha256sum &> /dev/null
	then
		usage
		echo
		echo "Program $RUNNER not available. Maybe install it?"
		exit 1
	fi
elif [[ ! -x $RUNNER ]]
then
	usage
	echo
	echo "Program $RUNNER not available. (Maybe build it or use -s?)"
	exit 1
fi

for dir in a ones shak zero random
do
	for type in orig mod
	do
		echo "Creating $dir/$dir.$type..."
		if [[ -f $dir/$dir.$type ]]
		then
			rm $dir/$dir.$type
		fi
		for i in $(seq $START $END); do $RUNNER $dir/$type/$i >> $dir/$dir.$type; done
	done

	if [[ -f $dir/$dir.alt ]]
	then
		rm $dir/$dir.alt	
	fi
	echo "Creating $dir/$type.alt (alternating)"
	for i in $(seq $START $END); do $RUNNER $dir/orig/$i >> $dir/$dir.alt; $RUNNER $dir/mod/$i >> $dir/$dir.alt; done
done
