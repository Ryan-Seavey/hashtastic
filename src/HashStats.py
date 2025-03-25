#!/usr/bin/env python3

import getopt
import os
import sys
import re
import numpy

def usage():
    instructions = """HashStats.py -- compute some basic stats about hash output
    Usage: HashStats.py -i input_file [-ah]

    Options:

    -i file     File to process containing 256-bit or 32-bit hashes
    -a          Hashes alternate between two algorithms. (optional)
    -h          Show this screen.
    -c file     Write output to file named 'file'
    -v          Be verbose (e.g., print collision file numbers)

    """

    print(instructions)


def main():
    
    altmode = False     # are we differencing every two digests first?
    infile = ""         # input file path
    digest_dict = {}        # place to store binary digests
    numhashes = 0       # how many digests are we processing (for mean)
    digest_len = 0      # how long are the digests in this file (assumes all same)
    collisions = 0      # how many collisions have we seen with this input?
    verbose = 0         # switch for being verbose
    csv_outfile = ''
    
    # use getopts to handle commandline switches (getopt is so rad and
    # exists for bash, too!)

    try:
        opts, args = getopt.getopt(sys.argv[1:], 'i:ahc:v')
    except getopt.GetoptError as err:
        usage()
        print ("Error:")
        print (str(err))
        sys.exit(2)

    for o, a in opts:
        if o == "-a":
            altmode = True
        elif o == "-i":
            infile = a
        elif o == "-h":
            usage()
            sys.exit(0)
        elif o == "-c":
            csv_outfile = a
        else:
            print("Unhandled option.\n")
            usage()
            sys.exit(2)


    if infile == "":
        usage()
        print("You must specify an input file using the option -i filename.\n")
        sys.exit(2)

    # compile regular expression to extract the digest from the file
    p = re.compile("^([0-9a-f]+)\s+[\w/]+/(\d+)$")
   

    # loop through the file and do the following:
    # - match the digest using the regex
    # - convert the digest to a hex integer
    # - convert the integer into a binary string representation
    # - store that binary string in an array

    pairstore = []
    with open(infile, 'r') as myfile:
        for line in myfile:
            
            m = p.match(line)
            try: 
                digest = m.group(1)
                filename = m.group(2)
            except AttributeError:
                print("The regular expression could not find both matches.")
                print("")
                print("Is this data in the right format?")
                print("Format:")
                print("hexadecimal_hash [whitespace] filename")
                print("")
                usage()
                sys.exit(1)
            if digest:
                
                hex_digest_len = len(digest)

                hex_digest = "0x" + digest
                # tricks found here:
                # https://stackoverflow.com/questions/21879454/how-to-convert-a-hex-string-to-hex-number
                # https://stackoverflow.com/questions/1425493/convert-hex-to-binary
                int_digest = int(hex_digest, 16)

                if hex_digest_len == 64:
                    bin_digest = f'{int_digest:0>256b}'
                elif hex_digest_len == 8:
                    bin_digest = f'{int_digest:0>32b}'
                else:
                    print("I don't understand how to convert hashes of this length!")
                    usage()
                    sys.exit(1)

                if altmode:
                    # this file contains alternating orig / mod hashes
                    
                    # get a pair of hashes in pairstore
                    pairstore.append(bin_digest)

                    # if we have one has in pairstore, get another
                    if len(pairstore) < 2:
                        continue # get next hash

                    # now we have a pair -- difference them in bin_digest
                    # if two bits are the same, return 1 for that
                    # bit, otherwise return 0. We would expect
                    # approximately 50/50 1s and 0s
                    bin_digest = ""
                    for i in range(len(pairstore[0])):
                        if pairstore[0][i] == pairstore[1][i]:
                            bin_digest += "1"
                        else:
                            bin_digest += "0"

                    # reset pairstore
                    pairstore = []
                    
                    # in the next code chunk, we will append bin_digest
                    # this difference as the "hash" for the pair as if
                    # we had just read it from the file

                # this file contains what should be 100% unique
                # input

                try: 
                    digest_dict[bin_digest]
                except KeyError:
                    digest_dict[bin_digest] = []

                digest_dict[bin_digest].append(filename) # add filename to dictionary for digest
                numhashes += 1 # got a hash!

    # do some stats on the array of digests!
    digests = list(digest_dict.keys())

    # assume all hashes are of same length
    digest_len = len(digests[0])

    row_ratios = [] # place to hold ratios
    
    # calculate the ratio of 1s to length of digest for each digest
    # this should hover around 0.5 if the hash is good
    for digest in digests:
        ratio = digest.count('1') / digest_len
        row_ratios.append(ratio)

    # compute the mean (yeah, could have summed all the ratios instead
    # of putting them in an array but this way they're all in an array
    # if you want to do something with the info.

    row_ones_mean = numpy.mean(row_ratios)
    row_ones_std = numpy.std(row_ratios)

    # what is the ratio of 1s to 0s in each column?

    column_one_counts = []
    column_one_ratios = []

    # make arrays with one item per column
    for i in range(digest_len):
        column_one_counts.append(0)
        column_one_ratios.append(0)

    # count the 1s in each column
    for digest in digests:
        for i in range(digest_len):
            # check each column for a 1
            if digest[i] == '1':
                column_one_counts[i] += 1 # the number of 1s in this column of all digests

    # calculate the number of 1s / number of hashes for each column

    for i in range(digest_len):
        column_one_ratios[i] = column_one_counts[i] / len(digests)


    # calculate the average 
    column_ones_mean = numpy.mean(column_one_ratios)
    column_ones_std = numpy.std(column_one_ratios)

    # print out summary statistics
    print(f"There were {numhashes} {digest_len}-bit hashes, and {len(digests)} unique digests.")
    print("All below ratios should be near 0.5!")
    print(f"Avg. ratio of 1/0s per digest: {row_ones_mean:.4f} row ones stdev: {row_ones_std:.4f}")
    print(f"Avg. ratio of 1/0s per column: {column_ones_mean:.4f} column ones stdev: {column_ones_std:.4f}")

    print("Column ratios sorted:")
    print(column_one_ratios)
        
    if altmode:
        print("These statistics describe pairs of related inputs.")
        print("Column ratios should be near 0.5 to indicate strict avalache on a 1-bit change.")
    else:
        # print out any collisions if any
        print("Collisions (blank if none):")
        for digest in digests:
    
            if len(digest_dict[digest]) > 1:
                print(f"{len(digest_dict[digest]) -1} collisions for digest {digest}") #, len(digest_dict[digest]), digest)
                if verbose:
                    print("Colliding inputs: " + str(digest_dict[digest]))
                    print()

                collisions += len(digest_dict[digest]) - 1

        print(f"Total collisions: {collisions} (there were {numhashes - collisions} unique hashes)")

    if csv_outfile != '':
        import csv
        with open(csv_outfile, 'w', newline = '') as csvfile:
            csvwriter = csv.writer(csvfile, delimiter = ',', quotechar = '|', quoting=csv.QUOTE_MINIMAL)
            csvwriter.writerow(['hashlen', digest_len])
            csvwriter.writerow(['filecount', numhashes])
            csvwriter.writerow(['row_ones_mean', row_ones_mean, 'row_ones_stdev', row_ones_std])
            csvwriter.writerow(['col_ones_mean', column_ones_mean, 'col_ones_stdev', column_ones_std])
            csvwriter.writerow(['column_means'] + column_one_ratios)
            csvwriter.writerow(['column_means_sorted'] + sorted(column_one_ratios))
            csvwriter.writerow(['collision_count', collisions])

        csvfile.close()

if __name__ == "__main__":
    main()
