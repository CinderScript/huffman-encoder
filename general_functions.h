/*
 * Title:		Lab: Huffman
 * Filename:	general_functions.h
 * Description - contains generic functions for searching and sorting
 *
 * THIS HEADER FILE CONTAINS FUNCTION DEFINITIONS:
 *
 * Template functions aren't real functions until compilation - during compilation the function
 * is built with the the template type being set to whatever the usage of the function
 * dictates.  The template function is compiled into several functions of each type being used.
 *
 * So anywhere this file in included (#include<>), if this file doesn't contain the definitions
 * for the templates, there will be no compiled version of the function that matches the
 * caller's type requirement.
 *
 * See:
 * http://www.parashift.com/c++-faq-lite/templates-defn-vs-decl.html
 * and
 * https://stackoverflow.com/questions/3040480/c-template-function-compiles-in-header-but-not-implementation
 *
 * THE INTERNET RECOMMENDS THAT THE DEFINITION BE INCLUDED IN THE .h FILE,
 * OR INCLUDE THE .cpp FILE.

 *
 * Author:		Maynard, Greg
 * Date:		06/12/2021
 */



/*
 *
 */

#ifndef HUFFMAN_MAYNAG_GENERAL_FUNCTIONS_H
#define HUFFMAN_MAYNAG_GENERAL_FUNCTIONS_H

#include <vector>
#include <functional>


using std::function;

/**
 * Reads in bytes from the istream and outputs its binary string representation.
 * @param in - source of bytes
 * @param out - binary string output
 */
size_t DecodeBytesToBinaryString(std::istream &in, std::ostream &out);

/**
 * Reads in a binary string and writes the bits to the ostream.
 * @param in - source of binary string
 * @param out - bytes output
 */
size_t EncodeBinaryStringToBytes(std::istream &in, std::ostream &out);


// THIS HEADER FILE CONTAINS FUNCTION DEFINITIONS - see the the file comment header above.

/**
 * Gets a pointer from the reference of argument object.  This is used for
 * the template functions where we don't know if we are getting a pointer or
 * a reference - so we ensure it is a pointer
 * @tparam T - The object's type
 * @param obj - the object
 * @return pointer to the object
 */
template<typename T>
T* ToPtr(T & obj) { return &obj; }
/**
 * The argument is already a pointer, so return it.  This is used for
 * the template functions where we don't know if we are getting a pointer or
 * a reference - so we ensure it is a pointer
 * @tparam T - The object's type
 * @param obj - the object
 * @return pointer to the object
 */
template<typename T>
T* ToPtr(T * prt) { return prt; }

/**
 * Finds the index of a given element in a vector.  This
 * function accepts both vectors of values and pointers.
 * @tparam T - Type of elements in the vector and element to compare
 * @param vector - vector to search
 * @param element - element to search for
 * @return index of the found element, else -1 if not found
 */
template<class T>
int IndexOf(std::vector<T> vector, T element){
	for ( size_t i = 0; i < vector.size(); ++i ) {
		if ( *ToPtr( vector[i] ) == *ToPtr(element) )
			return i;
	}
	return -1;
}
/**
 * Finds the index of a given element in a vector.  This
 * function accepts both vectors of values and pointers.
 *
 * To use this function, make sure that the class
 * T has overloaded the == operator for T == K
 *
 * @tparam T - Type of the elements in the vector
 * @tparam K - Type of the search element
 * @param vector - vector to search
 * @param element - element to search for
 * @return index of the found element, else -1 if not found
 */
template<class T, class K>
int IndexOf(std::vector<T> vector, K element){
	for ( size_t i = 0; i < vector.size(); ++i ) {
		if ( *ToPtr( vector[i] ) == *ToPtr(element))
			return i;
	}
	return -1;
}

/**
 * Generic - Swaps elements a and b.
 * @tparam T - Type of element a and b.
 * @param a - element to swap
 * @param b - element to swap
 */
template <class T>
void Swap(T &a, T &b){
	T temp = a;
	a = b;
	b = temp;
}

/**
 * Performs a Bubble Sort on the contents of the vector.
 * @tparam T - Type of the elements in the vector
 * @param list - the vector to sort
 * @param _comparator - the function used to determine element order. elements are in the
 * 						correct position when the comparator callback returns true;
 */
template <class T>
void BubbleSort(std::vector<T> &list, function<bool(T, T)> _comparator ){
	size_t size = list.size();
	for ( size_t i = 1; i < size; ++i ) {
		for ( size_t j = 0; j < size - i; ++j ) {
			T* first = &list[j];
			T* second = &list[j+1];
			if ( _comparator( list[j] , list[j+1]) ) { // send the elements to the callback
				Swap(*first, *second);
			}
		}
	}
}
#endif //HUFFMAN_MAYNAG_GENERAL_FUNCTIONS_H
