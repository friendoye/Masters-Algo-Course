#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned long long llong;
typedef unsigned int uint;

ifstream input;
ofstream output;
int N;

const int LINES_PER_BLOCK = 30000;
const int ADD_TO_FILTER = 1;
const int CHECK_IN_FILTER = 0;

class BlumFilter {
public:
	BlumFilter(int insert_amount, float false_positive_rate) {
		float float_filter_size = ceil(-1 * insert_amount * log(false_positive_rate) / (log(2) * log(2)));
		hash_func_number = (int) ceil(float_filter_size * log(2) / insert_amount);
		filter_size = (int) float_filter_size;
		bits = new char[filter_size / 8 + 1]();
		cout << hash_func_number << "\t" << filter_size;
	}

	virtual ~BlumFilter() {
		delete bits;
	}

	void add(string item) {
		llong hash = string_hash(item);
		uint hashB = hash & ((llong) 0xFFFFFFFF);
		uint hashA = hash & ~((llong) 0xFFFFFFFF << 32);
		//cout << "Add" << endl;
		for (int i = 0; i < hash_func_number; i++) {
			//cout << n_hash(i, hashA, hashB) << endl;
			set_bit(n_hash(i, hashA, hashB));
		}
	}

	bool check(string item) {
		llong hash = string_hash(item);
		uint hashB = hash & ((llong) 0xFFFFFFFF);
		uint hashA = hash & ~((llong) 0xFFFFFFFF >> 32);
		//cout << "Check" << endl;
		for (int i = 0; i < hash_func_number; i++) {
			//cout << n_hash(i, hashA, hashB) << endl;
			if (is_bit_set(n_hash(i, hashA, hashB)) == false) {
				return false;
			}
		}
		return true;
	}
private:
	llong string_hash(string item) {
		return (llong) hasher(item);
	}

	void set_bit(uint position) {
		uint offset = position / 8;
		uint mask = 0x1 << (7 - position % 8);
		bits[offset] |= mask;
	}

	bool is_bit_set(uint position) {
		uint offset = position / 8;
		uint mask = 0x1 << (7 - position % 8);
		return (bool) (bits[offset] & mask);
	}

	uint n_hash(int n, uint hashA, uint hashB) {
    	return (hashA + n * hashB) % filter_size;
	}

	hash<string> hasher;
  	char *bits;
	int filter_size; // 5751036
	int hash_func_number; // 7
};

int main() {
	input.open("input.txt", ios::in);
	input >> N;

	BlumFilter filter = BlumFilter(1000000, 0.005);
	
	output.open("output.txt");

	int request_type;
	string item;
	bool exists;
	int flush_counter = 0;
	while (N > 0) {
		input >> request_type >> item;
		switch (request_type) {
			case ADD_TO_FILTER:
				filter.add(item);
				break;
			case CHECK_IN_FILTER:
				exists = filter.check(item);
				output << (bool) exists;
				break;
		}
		
		N--;

		flush_counter++;
		if (flush_counter >= LINES_PER_BLOCK) {
			output.flush();
			flush_counter = 0;
		}
	}
	
	output.close();
	input.close();

	return 0;
}