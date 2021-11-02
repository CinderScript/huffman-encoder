/*
 * Title:		Lab: Huffman
 * Filename:	main.cpp
 * Description: The entry point for this application.  the main method
 * looks for command line args and selects the appropriate action.
 *
 * Author:		Maynard, Greg
 * Date:		06/12/2021
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "huffman.h"

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char** argv) {

	if (argc < 2){
		cerr << "Not enough arguments!" << endl;
		return 1;
	}

	string command1 = argv[1];
	string inputfile = argv[2];
	ifstream infile(inputfile);
	if ( !infile.is_open() ){
		cerr << "Could not open the file: " << inputfile  << endl;
		return 1;
	}

	if ( command1 == "-frequency"){
		Huffman huff(infile);
		infile.close();
		huff.PrintFrequencyTable(cout);
		return 0;
	}

	if ( command1 == "-encoding-table"){
		Huffman huff(infile);
		infile.close();
		huff.PrintEncodingTable(cout);
		return 0;
	}

	if (argc < 3){
		cerr << "Not enough arguments!" << endl;
		return 1;
	}

	string command2 = argv[3];

	if ( command1 == "-single"){
		Huffman huff(infile);
		infile.close();
		cout << huff.GetCharacterCode(command2[0]) << endl;

		return 0;
	}

	if ( command1 == "-encode"){

		ofstream outfile(command2);
		if ( !outfile.is_open() ){
			cerr << "Could not open the file: " << command2  << endl;
			return 1;
		}

		Huffman huff(infile);
		infile.clear();
		infile.seekg(0);
		std::size_t bytes = huff.WriteEncodedText(infile, outfile);
		cout << bytes << " bytes written (encoded with bits)." << endl;

		infile.close();
		outfile.close();
		return 0;
	}

	if ( command1 == "-decode"){
		ofstream outfile(command2);
		if ( !outfile.is_open() ){
			cerr << "Could not open the file: " << command2  << endl;
			return 1;
		}

		Huffman huff;
		huff.WriteDecodedText(infile, outfile);

		cout << "File decoded." << endl;

		infile.close();
		outfile.close();
		return 0;
	}


	cerr << "Did not recognize the arguments" << endl;

	return 0;
}
