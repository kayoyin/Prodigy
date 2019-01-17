#include <isostream>
#include <map>
#include <string>
#include <iterator>

int main()
{
	std::map<std::string, int> mapOfWords;
	//C:48, Db:49, D:50 ...
	mapOfWords.insert(std::make_pair("C",48));
	mapOfWords.insert(std::make_pair("Db",49));
	mapOfWords.insert(std::make_pair("D",50));
	mapOfWords.insert(std::make_pair("Eb",51));
	mapOfWords.insert(std::make_pair("E",52));
	mapOfWords.insert(std::make_pair("F",53));
	mapOfWords.insert(std::make_pair("Gb",54));
	mapOfWords.insert(std::make_pair("G",55));
	mapOfWords.insert(std::make_pair("Ab",56));
	mapOfWords.insert(std::make_pair("A",57));
	mapOfWords.insert(std::make_pair("Bb",58));
	mapOfWords.insert(std::make_pair("B",59));
	mapOfWords.insert(std::make_pair("C_2",60));
}



		
