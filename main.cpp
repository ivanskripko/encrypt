#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

// Класс, который содежит пару для замены (какой символ и на какой меняем)
class ReplacePair 
{
private:
	char myFrom;
	char myTo;
public:
	ReplacePair () 
	{
		myFrom = 0;
		myTo = 0;
	}

	ReplacePair (char from, char to) : myFrom(from), myTo(to) 
	{
	}

	ReplacePair (const ReplacePair& rp)
	{		
		myFrom = rp.getFrom();
		myTo = rp.getTo();
	}

	char getFrom() const
	{
		return myFrom;
	}

	char getTo() const
	{
		return myTo;
	}

	void setFrom (char from) 
	{
		myFrom = from;
	}

	void setTo (char to)
	{
		myTo = to;
	}
};

bool isChange(int probabilityValue = 50);
int generateFromRange (int from, int to);
bool sortReplacePairs (ReplacePair rp1, ReplacePair rp2);
void printError (const string& s);
void fillRaplacePairsVector (ifstream& file, vector<ReplacePair>& v);
int findFirstElementPosistion (vector<ReplacePair>& v, char c);
int findLastElementPosistion (vector<ReplacePair>& v, char c);

int main (int argc, const char* argv[]) 
{

	vector<ReplacePair> v;
	const string replaces = "replaces.txt";

	ifstream replaceFile;
	replaceFile.open(replaces.c_str(), ios::in);

	if (!replaceFile.is_open()) {		
		printError(replaces);
		return -1;
	}

	fillRaplacePairsVector(replaceFile, v); 
	
	replaceFile.close();		

	sort(v.begin(), v.end(), sortReplacePairs); 
	/* 
	можно было записать через лямбда функцию
	sort(v.begin(), v.end(), [&](ReplacePair rp1, ReplacePair rp2) {
		return rp1.getFrom() < rp2.getFrom();
	}); 
	*/

	string input = "in.txt";
	string output = "out.txt";

	if (argc == 2) {
		input = argv[1];
	} else if (argc == 3) {
		input = argv[1];
		output = argv[2];
	} else if (argc > 3) {
		cerr << "Wrong number of arguments!" << endl;
		cout << "Usage: \nNo arguments - standart input and output." << endl;
		cout << "First argument - input, second argument - output." << endl;
		return -1;
	}
	

	ifstream inFile;
	ofstream outFile;

	inFile.open(input.c_str(), ios::in);	
	outFile.open(output.c_str(), ios::out);

	if (!inFile.is_open()) {
		printError(input);
		return -1;
	}
	if (!outFile.is_open()) {
		printError(output);
		return -1;
	}

	srand(time(0));
	
	int c = 0;
	while ((c = inFile.get()) != EOF) {
		if (isChange()) {

			c = tolower(c);

			int posBeg = findFirstElementPosistion(v, c);
			int posEnd = findLastElementPosistion(v, c);
			int position = 0;

			if (posBeg != -1) {
				if (posBeg != posEnd) {
					position = generateFromRange(posBeg, posEnd);					
				} else {
					position = posBeg;
				}
				c = v.at(position).getTo();
			} else {
				c = toupper(c);
			}
		} 

		outFile.put((char)c);
	}

	inFile.close();
	outFile.close();
	return 0;
}

bool isChange(int probabilityValue) 
{
	const int percent = 100;
	int n = rand() % percent;
	return n < probabilityValue;
}

int generateFromRange (int from, int to) 
{
	return rand() % ((to + 1) - from) + from;
}

bool sortReplacePairs (ReplacePair rp1, ReplacePair rp2) 
{
	return rp1.getFrom() < rp2.getFrom();
}

void printError (const string& s) 
{
	cerr << "File: " << s << " cannot be opened." << endl;
}

void fillRaplacePairsVector (ifstream& file, vector<ReplacePair>& v) 
{	
	const int size = 256;

	char from = 0;
	char to = 0;

	while (file >> from >> to) {		
		v.push_back(ReplacePair(from, to));
		file.ignore(size, '\n');
	}
}

int findFirstElementPosistion (vector<ReplacePair>& v, char c) 
{
	int pos = -1;	
	for (vector<ReplacePair>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it->getFrom() == c) {
			pos = distance(v.begin(), it);
			break;
		}		
	}
	return pos;
}

int findLastElementPosistion (vector<ReplacePair>& v, char c) 
{
	int pos = -1;	
	for (vector<ReplacePair>::iterator it = v.end() - 1; it != v.begin(); --it) {
		if (it->getFrom() == c) { 
			pos = distance(v.begin(), it);
			break;
		}		
	}
	return pos;
}