/*
 * test_penn_lexicon.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: carlos
 */

#include "penn_lexicon.cpp"
#include "reader.cpp"

/**
 * Unit test
 */
int main( void )
{
	std::string filename;
	std::cout << "Enter filename for Penn treebank in CoNLL format: ";
	std::cin >> filename;
	bool bSuccess = english::initLexicon(filename,true);
	std::cout << "Successfully loaded the lexicon? " << bSuccess << "\n";
	for(;;)
	{
		std::string word;
		std::cout << "Enter a word: ";
		std::cin >> word;
		std::cout << "That word is " << (english::isKnown(word)?"known":"unknown") << ".\n";
		std::cout << "It can have the following Penn tags: \n";
		std::set<english::CTag> setOfTags = english::getPossibleTags(word);
		for ( std::set<english::CTag>::iterator it = setOfTags.begin() ; it != setOfTags.end() ; ++it )
		{
			std::cout << " " << *it;
		}
		std::cout << "\n";
		std::cout << "And the following morphological analyses: \n";
		std::set<english::CMorph> setOfMorphs = english::getPossibleMorph(word);
		for ( std::set<english::CMorph>::iterator it = setOfMorphs.begin() ; it != setOfMorphs.end() ; ++it )
		{
			std::cout << " " << (*it).str() << "\n";
		}
		std::cout << "\n";
	}
}
