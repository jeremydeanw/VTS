//
//  ParserUtils.cpp
//  VTS
//
//  Created by Papoj Thamjaroenporn on 10/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "ParserUtils.h"

void tokenize( std::string &str, std::vector< std::string > &tokens, char &delim )
{
	std::stringstream stream(str);
	std::string temp;
	while (getline(stream, temp, delim)) {
		if (!temp.empty()) {
			tokens.push_back(temp);
		}	
	}
}

void tokenizeBySpaces( std::string &str, std::vector< std::string > &tokens )
{
	str.erase( remove( str.begin(), str.end(), '\n' ), str.end() ) ;		// remove EOL
	str.erase( remove( str.begin(), str.end(), '\r' ), str.end() ) ;		// remove EOL
	
	char delim = ' ';
	tokenize( str, tokens, delim );
}

void tokenizeByComma( std::string &str, std::vector< std::string > &tokens )
{	
	char delim = ',';
	tokenize( str, tokens, delim );
}