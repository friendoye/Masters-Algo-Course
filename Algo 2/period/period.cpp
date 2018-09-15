#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

ifstream input;
ofstream output;

vector<int> p_function(string word, int len) {
	vector<int> p(len);

	int k = 0;	
	for (int i = 1; i < len; i++) {	
		while ((k > 0) && (word[k] != word[i])) {
			k = p[k - 1];
		}
		if (word[k] == word[i]) {
			k++;
		}
		p[i] = k;
	}

	return p;
}

int main() {
	int len = 0;

	input.open("input.txt");
	input >> len;

	output.open("output.txt");
		
	input.ignore(1);
	string word;
	getline(input, word);
	
	input.close();
	
	stringstream ss;
	vector<int> p_word = p_function(word, len);
	for (int k = 1; k < len; k++) {
		int diff = (k + 1) - p_word[k];
		if ((k + 1) != diff && (k + 1) % diff == 0) {
			ss << k + 1 << " " << (k + 1) / diff << endl;
		}
	}

	output << ss.str();
	
	// cout << word << endl;
	// for (int index : p_word) {
	// 	cout << index << " ";
	// } 
	// cout << endl;
	
	return 0;
}