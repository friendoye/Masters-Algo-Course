#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

ifstream input;
ofstream output;

vector<int> p_function(const string & word, int len) {
	vector<int> p(len, 0);

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

vector<int> z_function(const string & word, int len) {
	vector<int> z(len, 0);

	int l = 0, r = 0;
	for (int i = 1; i < len; i++) {
		if (i <= r) {
			z[i] = min(r - i + 1, z[i - l]);
		}
		while (i + z[i] < len && word[z[i]] == word[i + z[i]]) {
			z[i]++;
		}
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}

	return z;
}

int main() {
	int T = 0;

	input.open("input.txt");
	input >> T;

	output.open("output.txt");
	
	int len = 0;
	while (T-- > 0) {
		input >> len;
		vector<int> int_string(len, 0);
		int_string[0] = 0;
		for (int i = 1; i < len; i++) {
			input >> int_string[i];
		}

		// Z -> P
		try {
			vector<int> generated_int_string1(len, 0);
			build_int_string_from_z_function(generated_int_string1, 
											 int_string, len);
			vector<int> z_func = z_function(generated_int_string1, len);
			if (equal(int_string.begin(), int_string.begin() + len, 
				z_func.begin())) {
				vector<int> p_func = p_function(generated_int_string1, len);
				print_vector(p_func, len);
			} else {
				print_negative_result();
			}
		} catch (...) {
			print_negative_result();
		}

		// P -> Z
		try {
			vector<int> generated_int_string2(len, 0);
			build_int_string_from_p_function(generated_int_string2, 
											 int_string, len);
			vector<int> p_func = p_function(generated_int_string2, len);
			if (equal(int_string.begin(), int_string.begin() + len, 
				p_func.begin())) {
				vector<int> z_func = z_function(generated_int_string2, len);
				print_vector(z_func, len);
			} else {
				print_negative_result();
			}
		} catch (...) {
			print_negative_result();
		}
	}
	
	return 0;
}