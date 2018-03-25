#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef char byte;

int N, M;
byte* A;
byte* VECTOR;
byte* RESULT;

int OFFSET_TO_MATRIX, OFFSET_TO_VECTOR;
bool IS_VECTOR_CAN_BE_PLACED_IN_MEMORY = false;

int BYTES_LIMIT = 100000;
int RESULT_BYTES_LIMIT = 100000;
ifstream input;
ofstream output;

void generate_input_bin() {
	int N = 3, M = 4;
	output.open("input.bin", ios::binary | ios::out);

	output.write((byte*) &N, sizeof(int));
	output.write((byte*) &M, sizeof(int));

	byte zero = 0;
	for (int i = 1; i <= N * M; i++) {
		if (i < 10) {
			output.write((byte*) &i, sizeof(byte));
		} else {
			output.write((byte*) &zero, sizeof(byte));
		}
	}

	for (int i = 1; i <= M; i++) {
		output.write((byte*) &i, sizeof(byte));		
	}

	output.close();
}

int main() {
	//generate_input_bin();

	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &N, sizeof(int));
	input.read((byte*) &M, sizeof(int));
	
	OFFSET_TO_MATRIX = 8;
	OFFSET_TO_VECTOR = 8 + N * M;
	
	output.open("output.bin", ios::binary | ios::out);
	
	output.seekp(0 + N - 1);
	output.write("", 1);
	output.seekp(0);

	int block_width  = min(M, BYTES_LIMIT);
	int block_height = min(N, max(BYTES_LIMIT / block_width, 1));
	int block_size = block_height * block_width; 
	int vector_block_size = block_width;
	//int vector_block_size = min(BYTES_LIMIT, M);
	//IS_VECTOR_CAN_BE_PLACED_IN_MEMORY = (vector_block_size == M);
	int result_block_size = block_height;

	A      = new byte[block_size];
	VECTOR = new byte[vector_block_size];
	RESULT = new byte[result_block_size];

	// if (IS_VECTOR_CAN_BE_PLACED_IN_MEMORY) {
		// input.seekg(OFFSET_TO_VECTOR);
		// input.read((byte*) VECTOR, vector_block_size);				
	// }

	for (int i = 0; i < N; i += block_height) {
		for (int j = 0; j < block_height; j++) {
			RESULT[j] = 0;
		}
				
		for (int j = 0; j < M; j += block_width) {
			int clamp = min(M - j, block_width);

			// Read matrix block
			int offset = i * M + j;
			input.seekg(OFFSET_TO_MATRIX + offset);
			input.read((byte*) A, block_height * clamp);

			// Read vector block
			//if (!IS_VECTOR_CAN_BE_PLACED_IN_MEMORY) {
				offset = j;
				input.seekg(OFFSET_TO_VECTOR + offset);
				input.read((byte*) VECTOR, clamp);				
			//}

			// Multiply
			for (int counter = 0; counter < min(result_block_size, N - i); counter++) {
				int offset = 0;
				//if (IS_VECTOR_CAN_BE_PLACED_IN_MEMORY) {
				//	offset = j;
				//}
				for (int k = 0; k < clamp; k++) {
					RESULT[counter] += A[counter * block_width + k] * VECTOR[offset + k];
				}
			}
		}

		//for (int j = 0; j < block_height; j++) {
		//	cout << (int) RESULT[j] << endl;
		//}

		output.write((byte*) RESULT, min(result_block_size, N - i));
	}

	delete A, VECTOR, RESULT;

	return 0;
}