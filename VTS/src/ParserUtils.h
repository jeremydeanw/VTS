//
//  ParserUtils.h
//  VTS
//
//  Created by Papoj Thamjaroenporn on 10/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __VTS__ParserUtils__
#define __VTS__ParserUtils__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


/////////////////////////////////////////////////////////////////////////////
// Template functions for type conversions. Useful
// for printing text / parsing / debugging

// Template has to be in the same source
// file, not in separate header, since the actual code is generated at compile time.

template <class T>
T stringToNumber ( const std::string &text ) {                          
	std::stringstream ss(text);
	T result;
	return (ss >> result) ? result : 0;
} 

template <class T>
std::string numberToString ( const T &number ) {                          
	std::stringstream ss;
	
	ss << number;
	return ss.str();
} 


template <class T>
std::string vectorToStr ( const std::vector<T> &vec , int offset = 0) {                          
	std::string myStr;
	for (int i = 0; i < vec.size(); i++)
		myStr = myStr + " " + numberToString(vec[i] + offset);
	myStr = myStr + " ";
	return myStr;
} 

/////////////////////////////////////////////////////////////////////////////
// Helpers for parsing

void tokenize( std::string &str, std::vector< std::string > &tokens, char &delim );
void tokenizeBySpace( std::string &str, std::vector< std::string > &tokens );
void tokenizeByComma( std::string &str, std::vector< std::string > &tokens );


#endif /* defined(__VTS__ParserUtils__) */
