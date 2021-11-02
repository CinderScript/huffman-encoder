/*
 * Title:		Lab: Huffman
 * Filename:	huffman.h
 * Description: Defines the Huffman class which has methods for building a huffman binary tree
 * 				and building an encoding table for text.
 *
 * Author:		Maynard, Greg
 * Date:		06/12/2021
 */
#ifndef HUFFMAN_MAYNAG_HUFFMAN_H
#define HUFFMAN_MAYNAG_HUFFMAN_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

using std::function;
using std::vector;
using std::map;
using std::string;
using std::ostream;
using std::istream;


class Huffman
{
	struct FrequencyNode{
		string _key;
		size_t _frequency = 0;

		FrequencyNode* _left;
		FrequencyNode* _right;

		FrequencyNode(const string &key, size_t frequency,
						 FrequencyNode* left = nullptr, FrequencyNode* right = nullptr);
		FrequencyNode& operator++(int);
		bool operator == (const FrequencyNode &node) const;
		bool operator == (const string &key) const;
		bool operator != (const FrequencyNode& node) const;
		bool operator != (const string& key) const;
	};

	struct CharacterCode
	{
		char _character;
		string _code;

		bool operator == (const char &key) const;
	};

	// *** SORTING FUNCTIONS
	/**
	 * Return True if the first is less than the second (when need to swap)
	 */
	function<bool(FrequencyNode*, FrequencyNode*)> descendingByKey =
			[](const FrequencyNode* first, const FrequencyNode* second){
				return first->_key < second->_key;
			};
	function<bool(FrequencyNode*, FrequencyNode*)> descendingByFrequency =
			[](const FrequencyNode* first, const FrequencyNode* second){
				return first->_frequency < second->_frequency;
			};

	const string ENCODING_TABLE_START = "*TBL*";
	const string ENCODING_TABLE_END = "*TBL*";

	vector<FrequencyNode*> _frequencyTable;
	vector<CharacterCode*> _encodingTable;
	FrequencyNode* _huffmanTree = nullptr;


	void PostOrderTraversal(FrequencyNode* node, function<void(FrequencyNode*)> callback );


public:
	~Huffman();
	Huffman(istream &inStream);
	Huffman();

	void BuildTables(istream &inStream);
	bool WriteDecodedText(istream &inStream, ostream &outStream);

	void PostOrderTraversal(function<void(FrequencyNode*)> callback);

	void PrintFrequencyTable(ostream &out);
	void PrintEncodingTable(ostream &out);
	string GetCharacterCode(const char c);
	size_t WriteEncodedText(istream &in, ostream &out);
};


#endif //HUFFMAN_MAYNAG_HUFFMAN_H
