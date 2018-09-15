#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

int knp(string& word, string& pattern) {
	vector<int> p_pattern = p_function(pattern, pattern.length());

	for (int k = 0, i = 0; i < word.length(); i++)  {
		while ((k > 0) && (pattern[k] != word[i])) {
			k = p_pattern[k-1];
		}

		if (word[i] == pattern[k]) {
			k++;
		}

		if (k == pattern.length()) {
			return i + 1 - k;
		}
	}

	return -1;
}

int main() {
	int len = 0;

	input.open("input.txt");
	input >> len;

	output.open("output.txt");
		
	input.ignore(1);
	string word1;
	string word2;
	getline(input, word1);
	getline(input, word2);

	if (word1 == word2) {
		output << 0;
		return 0;
	}

	string word11 = word1 + word1;
	long index = knp(word11, word2);
	cout << index << endl;
	if (index == -1) {
		output << -1;
	} else {
		output << index;
	}
	
	return 0;
}
