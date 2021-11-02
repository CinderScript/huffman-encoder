
#include <iostream>
#include <bitset>

#include "general_functions.h"

using std::bitset;

/**
 * Reads in bytes from the istream and outputs its binary string representation.
 * @param in - source of bytes
 * @param out - binary string output
 */
size_t DecodeBytesToBinaryString(std::istream &in, std::ostream &out){
	size_t bytes = 0;
	char c;
	while( in.get(c) ){
		std::bitset<8> byte(c);
		out << byte;   // write the 1's 0's as characters
		bytes++;
	}
	return bytes;
}


/**
 * Reads in a binary string and writes the bits to the ostream.
 * @param in - source of binary string
 * @param out - bytes output
 */
size_t EncodeBinaryStringToBytes(std::istream &in, std::ostream &out){
	size_t bytes = 0;
	bitset<8> byte;
	while ( in >> byte ){
		out << (char)(byte.to_ulong()); // write the byte
		bytes++;
	}
	return bytes;
}