/*
 * Title:		Lab: Huffman
 * Filename:	huffman.cpp
 * Description: Defines the Huffman class which has methods for building a huffman binary tree
 * 				and building an encoding table for text.
 *
 * Author:		Maynard, Greg
 * Date:		06/12/2021
 */
#include <iostream>
#include <functional>
#include <iomanip>
#include <sstream>

#include "huffman.h"
#include "general_functions.h"

using std::function;
using std::cout;
using std::endl;
using std::setw;
using std::istream;

// * * * * * * * * * PUBLIC * * * * * * * * * * * * * * * *//

/**
 * Default Constructor
 * @param inStream
 */
Huffman::Huffman() {}

/**
 * Builds the frequencyTable, encodingTable, and huffmanTree
 * @param inStream
 */
Huffman::Huffman(istream &inStream) {
	BuildTables(inStream);
}

Huffman::~Huffman() {
	PostOrderTraversal( [](FrequencyNode* node){
		delete node;
		node = nullptr;
	} );

	for ( auto element : _encodingTable) {
		delete element;
		element = nullptr;
	}

	for ( auto element : _frequencyTable) {
		delete element;
		element = nullptr;
	}
}


/**
 * Builds the frequency table and encoding table
 * @param inStream - text to build the tables from
 */
void Huffman::BuildTables(istream &inStream) {
// *** CREATE THE FREQUENCY TABLE
	vector<FrequencyNode *> treeNodeList;

	char c;
	while (inStream.get(c)) {
		string s(1, c);

		int i = IndexOf<FrequencyNode *, string>(treeNodeList, s);

		if ( i != -1 ) {
			(*treeNodeList[i])++;
			(*_frequencyTable[i])++;
		}
		else {
			treeNodeList.push_back(new FrequencyNode(s, 1));
			_frequencyTable.push_back(new FrequencyNode(s, 1));
		}
	}

	// *** SORT THE SAVED FREQUENCY TABLE
	BubbleSort<FrequencyNode *>(_frequencyTable, descendingByKey);
	BubbleSort<FrequencyNode *>(_frequencyTable, descendingByFrequency);


	// *** CREATE THE HUFFMAN TREE
	size_t size = treeNodeList.size();
	while ( size > 1) {
		// sort by descending key, then by descending frequency
		BubbleSort<FrequencyNode *>(treeNodeList, descendingByKey);
		BubbleSort<FrequencyNode *>(treeNodeList, descendingByFrequency);
		// create the node from the bottom 2 Frequency objects
		FrequencyNode *top = treeNodeList[size - 1];
		FrequencyNode *bottom = treeNodeList[size - 2];

		FrequencyNode *parent = new FrequencyNode(
				bottom->_key + top->_key,
				bottom->_frequency + top->_frequency,
				bottom, top);

		// remove the last two frequencies
		treeNodeList.erase(treeNodeList.end() - 2, treeNodeList.end());

		// add parent to the list
		treeNodeList.push_back(parent);
		size--;
	}

	_huffmanTree = treeNodeList[0];


	// *** CREATE THE ENCODING TABLE
	for ( auto n : _frequencyTable ){
		FrequencyNode* next = _huffmanTree;
		string code = "";
		string key = n->_key;

		// find n's key in the tree
		// for each node until we come to a leaf
		while (next != nullptr){
			FrequencyNode* left = next->_left;
			FrequencyNode* right = next->_right;
			if (next->_key == key)
				break;
			if (left != nullptr)
				if (left->_key.find(key) != string::npos) { // if the key is on the left, go there
					code += "0";
					next = left;
				}
			if (right != nullptr)
				if (right->_key.find(key) != string::npos) { // if the key is on the right, go there
					code += "1";
					next = right;
				}
		}
		// add the code to the encoding table
		CharacterCode* encoding = new CharacterCode();
		encoding->_character = key[0];
		encoding->_code = code;
		_encodingTable.push_back(encoding);
	}
}


/**
 * Traverses the _huffmanTree in Post Order.  callback is called on every node.
 * @param callback - operation to perform on each node
 */
void Huffman::PostOrderTraversal(function<void(FrequencyNode * )> callback) {
	if (_huffmanTree != nullptr)
		PostOrderTraversal(_huffmanTree, callback);
}

/**
 * Prints out the character frequency table to the ostream.
 * @param out - ostream to write to.
 */
void Huffman::PrintFrequencyTable(ostream &out) {
	for (auto n : _frequencyTable){
		string key = n->_key;
		if (key == "\n")
			key = "CR";
		else if (key == "\r")
			key = "LF";
		out << "{element:" << setw(3) << key << ", frequency: " << n->_frequency << "}" << endl;
	}
}

/**
 * Prints out the character encoding table to the ostream.
 * @param out - ostream to write to.
 */
void Huffman::PrintEncodingTable(ostream &out) {
	for (auto n : _encodingTable){
		string key( 1, n->_character);
		if ( key == "\n")
			key = "CR";
		if (key == "\r")
			key = "LF";
		out << "{key:" << setw(3) << key << ", code: " << n->_code << "}" << endl;
	}
}

/**
 * Decodes the text from the istream and writes the encoded version to the ostream.
 * @param in - stream to read the text from.
 * @param out - stream to write the encoded text to.
 * @return the number of bytes written to the stream.
 */
size_t Huffman::WriteEncodedText(istream &in, ostream &out) {

	size_t tableByets = 0;

	/// write the table ///
	out << ENCODING_TABLE_START << endl;
	for ( auto code : _encodingTable) {
		string key (1, code->_character);
		if (key == "\n")
			key = "CR";
		else if (key == "\r")
			key = "LF";
		else if (key == " ")
			key = "SP";
		string line = key + " " + code->_code + '\n';  // create encoding table row
		tableByets += line.size();					   // number of bytes added
		out << line;
	}
	out << ENCODING_TABLE_END << endl;


	/// create a stream of 1's 0's

	char c;
	std::stringstream binaryStringStream;
	// get each character from the in file
	while ( in.get(c) ) {
		// write the binary string to the out file.
		binaryStringStream << GetCharacterCode(c);
	}

	/// send stream to encoder - encodes into out
	size_t bytes = EncodeBinaryStringToBytes(binaryStringStream, out);

	out.flush();
	return bytes + tableByets;
}

/**
 * Builds the frequency table from the text in the inStream and then decodes the rest of the text
 * to the given ostream.
 * @param inStream - the encoded table and text
 * @param outStream - stream to write the decoded text to
 */
bool Huffman::WriteDecodedText(istream &inStream, ostream &outStream) {

	/// build out the encoding table ///
	string token;
	inStream >> token;

	if (token != ENCODING_TABLE_START){
		std::cerr << "ERROR: could not find an encoding table." << endl;
		return false;
	}

	while ( inStream >> token ){
		if ( token == ENCODING_TABLE_END )
			break;

		CharacterCode* encoding = new CharacterCode();

		if (token == "CR")
			encoding->_character = '\n';
		else if (token == "LF")
			encoding->_character = '\r';
		else if (token == "SP")
			encoding->_character = ' ';
		else
			encoding->_character = token[0];

		inStream >> token;
		encoding->_code = token;
		_encodingTable.push_back(encoding);
	}
	/// FINISHED GETTING ENCODING TABLE ///


	/// PARSE BITS INTO CHARS ///
	string code;
	char c;
	inStream.get(c);		// consume the newline

	std::stringstream bytestream;
	DecodeBytesToBinaryString(inStream, bytestream);

	while (bytestream.get(c)) {

		string s(1, c);
		// add next bit to the possible code
		code += s;

		// found complete code?
		for ( size_t i = 0; i < _encodingTable.size(); ++i ) {
			if ( _encodingTable[i]->_code == code ) {
				outStream << _encodingTable[i]->_character; // found a character
				code = ""; 									// reset code
				break;
			}
		}
	}
	return true;
}

/**
 * Once the encoding table has been built, GetCharacterCode returns the character code
 * for the given character.  Returns "NotFound" if the character is not in the table.
 * @param c - character to search for.
 * @return The character code from the encoding table.
 */
string Huffman::GetCharacterCode(const char c) {
	int i = IndexOf<CharacterCode *, char>(_encodingTable, c);
	if (i == -1)
		return "NotFound";
	return _encodingTable[i]->_code;
}


// * * * * * * * * * PRIVATE * * * * * * * * * * * * * * * *//

/**
 * Traverses the _huffmanTree in Post Order.  callback is called on every node.
 * This is called by the public overload.
 * @param node - starting node
 * @param callback - operation to perform on each node
 */
void Huffman::PostOrderTraversal(Huffman::FrequencyNode *node, function<void(FrequencyNode *)> callback) {
	if (node->_left != nullptr)
		PostOrderTraversal(node->_left, callback);
	if (node->_right != nullptr)
		PostOrderTraversal(node->_right, callback);

	callback(node);
}


/* * * * * * * * * * * * * * * * * * * * FrequencyNode * * * * * * * * * * * * * * * * * * * * * * */
/**
 * FrequencyNode constructor.  Frequency node is used to build out the huffman tree and the
 * frequency table.
 * @param key - characters in this node
 * @param frequency
 * @param left - left child
 * @param right - right child
 */
Huffman::FrequencyNode::FrequencyNode(const string &key, size_t frequency,
	  Huffman::FrequencyNode *left, Huffman::FrequencyNode *right)
	  : _key(key), _frequency(frequency), _left(left), _right(right){}

// ++
Huffman::FrequencyNode& Huffman::FrequencyNode::operator++(int) {
	_frequency++;
	return *this;
}

// ==
bool Huffman::FrequencyNode::operator==(const Huffman::FrequencyNode &node) const {
	return _key == node._key;
}
bool Huffman::FrequencyNode::operator==(const string &key) const {
	return _key == key;
}

// !=
bool Huffman::FrequencyNode::operator!=(const Huffman::FrequencyNode& node) const {
	return _key != node._key;
}
bool Huffman::FrequencyNode::operator!=(const string &key) const {
	return _key == key;
}



/* * * * * * * * * * * * * * * * * * * * FrequencyNode * * * * * * * * * * * * * * * * * * * * * * */

// ==
bool Huffman::CharacterCode::operator==(const char &key) const {
	return _character == key;
}
