#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

ifstream input;
ofstream output;
int N;

int* buffer;

const int CACHED_MATRIX_SIZE = 1000;

vector<int> generate_input(int n, int seed) {
    vector<int> d(n * n);
    for (size_t i = 0; i < d.size(); ++i) {
        d[i] = seed; 
        seed = ((long long) seed * 197 + 2017) & 987654;
    }
    return d;
}

// vector<int> generate_input(int n, int seed) {
//     vector<int> d(n * n);
//     for (size_t i = 0; i < d.size(); ++i) {
//         d[i] = i % n; 
//     }
//     return d;
// }

long long get_hash(const vector<int>& d) {
    const long long MOD = 987654321054321LL;
    const long long MUL = 179;
    
    long long result_value = 0;
    for (size_t i = 0; i < d.size(); ++i)
        result_value = (result_value * MUL + d[i]) & MOD;
    return result_value;
}

void print_matrix(int* M, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << M[i * N + j] << " ";
		}
		cout << endl;
	}
}

void matrix_to_buffer(int* M, int N, int i0, int j0, int i_size, int j_size) {
	for (int i = 0; i < i_size; i++) {
		for (int j = 0; j < j_size; j++) {
			buffer[CACHED_MATRIX_SIZE * i + j] = M[N * (i0 + i) + (j0 + j)] ;
		}
	}
}

void buffer_to_matrix(int* M, int N, int i0, int j0, int i_size, int j_size) {
	for (int i = 0; i < i_size; i++) {
		for (int j = 0; j < j_size; j++) {
			M[N * (i0 + i) + (j0 + j)] = buffer[CACHED_MATRIX_SIZE * i + j];
		}
	}
}

void transpose_block(int* M, int N, int i0, int j0, int i_size, int j_size) {
	int temp;
	for (int i = 0; i < i_size; i++) {
		for (int j = 0; j < j_size; j++) {
			temp = M[N * (i0 + i) + (j0 + j)];
			M[N * (i0 + i) + (j0 + j)] = buffer[CACHED_MATRIX_SIZE * j + i];
			buffer[CACHED_MATRIX_SIZE * j + i] = temp;
		}
	}	
}


void fast_transpose(int* M, int N, int i0, int j0, int size) {
	for (int i_offset = 0; i_offset < size; i_offset = min(i_offset + CACHED_MATRIX_SIZE, size)) {
		int i_clamp = min(CACHED_MATRIX_SIZE, size - i_offset);
		for (int j_offset = 0; j_offset <= i_offset; j_offset = min(j_offset + CACHED_MATRIX_SIZE, size)) {
			int j_clamp = min(CACHED_MATRIX_SIZE, size - j_offset);
			matrix_to_buffer(M, N, i0 + i_offset, j0 + j_offset, i_clamp, j_clamp);
			
			// cout << "Extract " << i0 + i_offset << " " << j0 + j_offset << endl;
			// print_matrix(buffer, CACHED_MATRIX_SIZE);
			
			transpose_block(M, N, i0 + j_offset, j0 + i_offset, j_clamp, i_clamp);
			
			// cout << "Rotate " << i0 + i_offset << " " << j0 + j_offset << endl;
			// print_matrix(buffer, CACHED_MATRIX_SIZE);
			
			buffer_to_matrix(M, N, i0 + i_offset, j0 + j_offset, i_clamp, j_clamp);
			// cout << "Matrix" << endl;
			// print_matrix(M, N);
		}
	}
}

int main() {
	int seed, k;
	//input.open("input.txt", ios::in);
	cin >> N >> seed >> k;

	//output.open("output.txt", ios::out);

	vector<int> matrix = generate_input(N, seed);
	buffer = new int[CACHED_MATRIX_SIZE * CACHED_MATRIX_SIZE];
	
	// cout << "Start" << endl;
	// print_matrix(matrix.data(), N);

	int i_start, j_start, size;
	for (int i = 0; i < k; i++) {
		cin >> i_start >> j_start >> size;
		fast_transpose(matrix.data(), N, i_start, j_start, size);
	}
	
	// cout << "End" << endl;
	// print_matrix(matrix.data(), N);

	cout << get_hash(matrix);

	delete buffer;

	//output.close();
	//input.close();

	return 0;
}