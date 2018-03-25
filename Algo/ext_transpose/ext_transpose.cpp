#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef char byte;

int n, m, k;
int N, M;
byte* A;
byte* B;

int MATRIX_SIZE = 400;
ifstream input;
ofstream output;

void generate_input_bin(int N, int M) {
	output.open("input.bin", ios::binary | ios::out);

	output.write((byte*) &N, sizeof(int));
	output.write((byte*) &M, sizeof(int));

	for (int i = 0; i < N * M; i++) {
		output.write((byte*) &i, sizeof(byte));		
	}

	output.close();
}

void block_transpose(int row_shift, int column_shift) {
	int height = min(N, n - row_shift);
	int width  = min(M, m - column_shift);

	input.seekg(8 + row_shift * m + column_shift);
	for (int i = 0; i < height; i++) {
		input.read(&A[i * width], width);
		input.ignore(m - width);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			B[j * height + i] = A[i * width + j];
		}
	}
		
	output.seekp(8 + column_shift * n + row_shift);
	for (int j = 0; j < width; j++) {
		if (n > height) {
			output.seekp(8 + (column_shift + j) * n + row_shift);
		}
		output.write(&B[j * height], height);	
	}

	output.flush();
}

int main() {
	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &n, sizeof(int));
	input.read((byte*) &m, sizeof(int));
	
	output.open("output.bin", ios::binary | ios::out);
	
	output.seekp(8 + m * n - 1);
	output.write("", 1);
	output.seekp(0);
	
	output.write((byte*) &m, sizeof(int));
	output.write((byte*) &n, sizeof(int));
	
	N = min(n, MATRIX_SIZE);
	M = min(m, MATRIX_SIZE * MATRIX_SIZE / N);
	A = new byte[N * M];
	B = new byte[N * M];

	for (int j = 0; j < m; j += M) {
		for (int i = 0; i < n; i += N) {			
			block_transpose(i, j);
		}
	}

	delete A, B;

	return 0;
}