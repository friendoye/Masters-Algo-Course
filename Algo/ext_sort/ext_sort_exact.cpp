#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

typedef char byte;
typedef unsigned long long qword;

qword N;
qword *A, *B;
qword* BUFFER;

int INPUT_OFFSET = 8;
int OUTPUT_OFFSET = 8;
int ACCEPTABLE_SIZE = 5; // in bytes
ifstream input;
fstream output, buffer;

void generate_input_bin(qword N) {
	output.open("input.bin", ios::binary | ios::out);
	
	output.write((byte*) &N, sizeof(qword));
	
	srand(time(NULL));
	for (qword i = N; i > 0; i--) {
		qword x = rand() % 256;
		output.write((byte*) &x, sizeof(qword));		
	}

	output.close();
}

void read_from_input(qword* array, int offset, int size) {
	input.seekg(INPUT_OFFSET + offset * sizeof(qword));
	input.read((byte*) array, size * sizeof(qword));
	cout << "Read input " << offset << "(" << size << ")" << endl;
	for (int i = 0; i < size; i++) {
		cout << array[i] << " ";
	}
	cout << endl;
}

void read_from_output(fstream* strm, qword* array, int offset, int size) {
	strm->seekg(INPUT_OFFSET + offset * sizeof(qword));
	strm->read((byte*) array, size * sizeof(qword));
	cout << "Read output " << offset << "(" << size << ")" << endl;
	for (int i = 0; i < size; i++) {
		cout << array[i] << " ";
	}
	cout << endl;
}

void write_to_output(fstream* strm, qword* array, int offset, int size) {
	strm->seekp(OUTPUT_OFFSET + offset * sizeof(qword));
	strm->write((byte*) array, size * sizeof(qword));
	cout << "Write output " << offset << "(" << size << ")" << endl;
	for (int i = 0; i < size; i++) {
		cout << array[i] << " ";
	}
	cout << endl;
}

void copy_from_to(fstream* from, fstream* to, int offset, int size) {
	from->seekg(INPUT_OFFSET + offset * sizeof(qword));
	to->seekp(OUTPUT_OFFSET + offset * sizeof(qword));
	while (size > 0) {
		int block_size = min(2 * ACCEPTABLE_SIZE, size);
		from->read((byte*) BUFFER, block_size * sizeof(qword));
		to->write((byte*) BUFFER, block_size * sizeof(qword));
		size -= block_size;
	}
}

void ext_merge_sort(int offset, int size, int depth) {
	int middle = offset + size / 2;
	int real_size = size, real_offset = offset;
	if (size <= ACCEPTABLE_SIZE) {
		read_from_input(A, offset, size);
		//read_from_input(B, middle, size - middle);
		//merge_sort(A, 0, size);
		sort(A, A + size);
		write_to_output(&buffer, A, offset, size);
		//merge_with_sorted_on_hdd(A, size, offset);
		//write_to_output(A, offset, size);
		//qword* C = merge(A, B, middle - offset, size - middle);
	} else {
		int startA = offset, sizeA = size / 2;
		int startB = middle, sizeB = size - size / 2;
		int blockSizeA = min(ACCEPTABLE_SIZE, sizeA);
		int blockSizeB = min(ACCEPTABLE_SIZE, sizeB);

		ext_merge_sort(startA, sizeA, depth + 1);
		ext_merge_sort(startB, sizeB, depth + 1);

		read_from_output(&buffer, A, startA, blockSizeA);
		read_from_output(&buffer, B, startB, blockSizeB);

		int l_index = 0;
	    int r_index = 0;
	    int current_index = 0;

	    while (current_index < size) {
	        if (r_index >= blockSizeB || blockSizeB == 0
	        	|| (l_index < blockSizeA && A[l_index] <= B[r_index])) {
	            BUFFER[current_index] = A[l_index];
	            l_index++;
	        } else {
	        	BUFFER[current_index] = B[r_index];
	            r_index++;
	        }
	        current_index++;

	        if (l_index == blockSizeA && blockSizeA != 0) {
        		l_index = 0;
        		startA += blockSizeA;
        		sizeA = max(0, sizeA - blockSizeA);
        		blockSizeA = min(ACCEPTABLE_SIZE, sizeA);
        		if (blockSizeA != 0) {
					read_from_output(&buffer, A, startA, blockSizeA);
        		}
	        }
	        if (r_index == blockSizeB && blockSizeB != 0) {
        		r_index = 0;
        		startB += blockSizeB;
        		sizeB = max(0, sizeB - blockSizeB);
        		blockSizeB = min(ACCEPTABLE_SIZE, sizeB);
				if (blockSizeB != 0) {
					read_from_output(&buffer, B, startB, blockSizeB);
				}
	        }
	        if (current_index == ACCEPTABLE_SIZE * 2) {
        		write_to_output(&output, BUFFER, offset, ACCEPTABLE_SIZE * 2);
        		offset += ACCEPTABLE_SIZE * 2;
        		current_index = 0;
	        	size -= ACCEPTABLE_SIZE * 2;
	        }
		}
		write_to_output(&output, BUFFER, offset, size);
		output.flush();
		copy_from_to(&output, &buffer, real_offset, real_size);
	}
}

int main() {
	//generate_input_bin(67);
	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &N, sizeof(qword));
	
	output.open("output.bin", ios::binary | ios::out | ios::in | ios::trunc);
	output.seekp((N + 1) * sizeof(qword) - 1);
	output.write("", 1);
	output.seekp(0);
	output.write((byte*) &N, sizeof(int));
	
	buffer.open("buffer.bin", ios::binary | ios::out | ios::in | ios::trunc);
	buffer.seekp((N + 1) * sizeof(qword) - 1);
	buffer.write("", 1);
	buffer.seekp(0);
	buffer.write((byte*) &N, sizeof(int));

	A = new qword[ACCEPTABLE_SIZE];
	B = new qword[ACCEPTABLE_SIZE];
	BUFFER = new qword[ACCEPTABLE_SIZE * 2];

	ext_merge_sort(0, N, 0);

	output.flush();
	output.close();
	buffer.close();
	input.close();

	delete A, B, BUFFER;

	return 0;
}