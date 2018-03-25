#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef char byte;

int n1, n2, n3;
int m1, m2, m3;
int block_height_1, block_width_1;
int block_height_2, block_width_2;
int block_height_3, block_width_3;
byte* A;
byte* B;
byte* C_LINE;

int offset_to_matrix_A, offset_to_matrix_B, offset_to_matrix_C;

int BYTES_LIMIT = 100000;
ifstream input;
ofstream output;

void generate_input_bin(int N, int M) {
	output.open("input.bin", ios::binary | ios::out);

	output.write((byte*) &N, sizeof(int));
	output.write((byte*) &M, sizeof(int));

	for (int i = 0; i < N * M; i++) {
		output.write((byte*) &i, sizeof(byte));		
	}

	output.write((byte*) &M, sizeof(int));
	output.write((byte*) &N, sizeof(int));

	for (int i = 0; i < N * M; i++) {
		output.write((byte*) &i, sizeof(byte));		
	}

	output.close();
}

int current_row = -1, current_column = -1;

void block_mult(int row_shift, int column_shift) {
	int block_height_1_clamped = min(block_height_1, n1 - row_shift);
	int block_width_2_clamped  = min(block_width_1, m1 - column_shift);

	// Read first matrix if needed
	if (current_row != row_shift) {
		current_row = row_shift;
		input.seekg(offset_to_matrix_A + current_row * m1 + column_shift);
		input.read(A, block_width_2_clamped);
	}

	// Read second matrix if needed
	if (current_column != column_shift) {
		current_column = column_shift;
		input.seekg(offset_to_matrix_B + column_shift);
		input.read(B, block_width_2_clamped);		
	}

	// Calculate and write result
	byte result_element;
	for (int j = 0; j < block_height_1_clamped; j++) {
		// Calculate target element
		for (int k = 0; k < block_width_2_clamped; k++) {
			C_LINE[j] += A[j * block_width_2_clamped + k] * B[k];
		}	
		// Write result to file
		//output.seekp(offset_to_matrix_C + m3 * (current_row + j) + current_column);
	}
}

int main() {
	offset_to_matrix_A = 8;
	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &n1, sizeof(int));
	input.read((byte*) &m1, sizeof(int));
	
	offset_to_matrix_B = 8 + n1 * m1;
	input.seekg(offset_to_matrix_B - 8);
	n2 = m1;
	m2 = 1;
	
	n3 = n1;
	m3 = m2;

	block_width_1  = min(m1, BYTES_LIMIT);
	block_height_1 = min(n1, max(BYTES_LIMIT / block_width_1, 1));

	block_height_2 = min(n2, BYTES_LIMIT);
	block_width_2  = min(m2, max(BYTES_LIMIT / block_height_2, 1));

	block_height_3 = block_height_1;
	block_width_3  = block_width_2;

	offset_to_matrix_C = 0;
	output.open("output.bin", ios::binary | ios::out);
	
	output.seekp(0 + n3 * m3  - 1);
	output.write("", 1);
	output.seekp(0);
	
	A = new byte[block_height_1 * block_width_1];
	B = new byte[block_height_2 * block_width_2];
	C_LINE = new byte[block_height_3];

	for (int i = 0; i < n3; i += block_height_3) {
		int block_height_1_clamped = min(block_height_1, n1 - i);

		for (int j = 0; j < block_height_3; j++) {
			C_LINE[j] = 0;
		}

		for (int j = 0; j < m1; j += block_width_1) {
			block_mult(i, j);
		}

		output.write(C_LINE, block_height_1_clamped);
	}

	delete A, B, C_LINE;

	return 0;
}