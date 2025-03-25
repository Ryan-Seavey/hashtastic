# Your Craptographic Hash Lab

The [manual for this exercise is on Google Drive](https://docs.google.com/document/d/1EsM4mnw1EQNi2srDxazJyJicQU_fXKECpAjW86x2UJQ/edit); instructions for the lab are there. This readme discusses the contents of this repository in brief.

## Installation Instructions

You will need `python3` of course, and potentially `python3-numpy`. Install it with:

```
$ apt-get install python3-numpy
```

## Test Data

There are six kinds of test data: `a`, `ones`, `random`, `shak`, and `zero`. Each directory has two subdirectories, `orig` -- containing 10k numbered samples (described below) -- and `mod` -- which contains the files from `orig` but with one random bit flipped in each file. That is, the corresponding files in `orig` and `mod` differ by one bit. All samples in `random` are 1250 bytes, but the samples in the other sets range from 1-10,000 bytes (corresponding to their filename). That is, `shak/orig/1` is a one byte sample while file `shak/orig/10000` is a 10,000-byte sample.

The data sources are as follows:

 1. `a` -- files made of the ASCII letter `a`
 1. `ones` -- files made of hexadecimal `0xff` (all binary 1s)
 1. `random` -- 1,250-byte samples of `/dev/urandom`
 1. `shak` -- samples taken from the [complete works of William Shakespeare](https://www.gutenberg.org/ebooks/100.txt.utf-8) (natural language)
 1. `zero` -- files made of hexadecimal `0x00` (all binary 0s)
 
Again, with the exception of `random`, all files increase in size from 1 byte to 10,000 bytes.
 
The lab manual has instructions for automating statistical tests with these input files.

## This version of SMHasher is edited for a hash lab
The student hash goes into src/StudentHash.cpp. You can then compile both the individual runner and SMhasher by running "make". 

To run the individual runner, run ./runner filename
to run SMhasher against the student hash, run ./smhasher StudentHash

You can change which tests are run in the main.cpp file for SMhasher and recompiling.

The SMHasher wiki has [information about what the individual tests](https://github.com/aappleby/smhasher/wiki/SMHasher) are doing and how to to interpret the results.


## [SMHasher](https://github.com/aappleby/smhasher/wiki) is a test suite designed to test the distribution, collision, and performance properties of non-cryptographic hash functions.

This is the home for the [MurmurHash](https://github.com/aappleby/smhasher/tree/master/src) family of hash functions along with the [SMHasher](https://github.com/aappleby/smhasher/tree/master/src) test suite used to verify them. SMHasher is released under the MIT license. All MurmurHash versions are public domain software, and the author disclaims all copyright to their code.

SMHasher is a test suite designed to test the distribution, collision, and performance properties of non-cryptographic hash functions - it aims to be the [DieHarder](http://www.phy.duke.edu/~rgb/General/dieharder.php) of hash testing, and does a pretty good job of finding flaws with a number of popular hashes.

The SMHasher suite also includes [MurmurHash3](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp), which is the latest version in the series of MurmurHash functions - the new version is faster, more robust, and its variants can produce 32- and 128-bit hash values efficiently on both x86 and x64 platforms.


## Updates

### 3/7/2023

I've updated and recreated the test files because there were some collisions and the set of files weren't large enough.

### 1/8/2016

Woo, we're on Github! I've been putting off the migration for a few, uh, years or so, but hopefully Github won't be shutting down any time soon so I don't have to move things again. MurmurHash is still used all over the place, SMHasher is still the defacto hash function test suite, and there are all sorts of interesting new hash functions out there that improve bulk hashing speed and use new shiny hardware instructions for AES and whatnot. Interesting times. :)

I've copied the few wiki pages from code.google.com/p/smhasher to this wiki, though I haven't reformatted them to Markdown yet. The MurmurHash page on Wikipedia should also be linking here now as well. Feel free to send me pull requests.
