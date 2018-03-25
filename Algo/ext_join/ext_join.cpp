#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>
#include <functional>
#include <algorithm>
#include <tuple>

using namespace std;

typedef char byte;
typedef unsigned int qword;
typedef pair<qword, qword> transition_pair;
typedef pair<qword, pair<qword, qword>> transition_triple;
//typedef tuple<qword, qword, qword> transition_triple;

int INPUT_OFFSET = 4;
int OUTPUT_OFFSET = 0;
int ACCEPTABLE_SIZE = 6000; // amount of transition_pair
fstream input, output;
fstream buffer;

int N;
transition_pair *A, *B;
transition_pair *BUFFER;
std::function<bool(transition_pair, transition_pair)> current_comparator;

transition_triple *A_FOR_TRIPLE, *B_FOR_TRIPLE;
transition_triple *BUFFER_FOR_TRIPLE;
std::function<bool(transition_triple, transition_triple)> current_comparator_for_triple;

void generate_input_bin() {
	output.open("input.bin", ios::binary | ios::out);

	int N = 1250000;
	qword* array = new qword[2 * N];
	for (int i = 0; i < N; i++) {
		array[i * 2 + 0] = i + 1;
		array[i * 2 + 1] = (i + 1) % N + 1;
	}
	output.write((byte*) &N, 4);
	output.write((byte*) array, 2 * N * sizeof(qword));

	output.close();
}

void copy_from_to(fstream* from, fstream* to, int offset, int size, 
				  byte* buffer, int buffer_size) {
	from->seekg(offset);
	to->seekp(offset);
	while (size > 0) {
		int block_size = min(buffer_size, size);
		from->read((byte*) buffer, block_size);
		to->write((byte*) buffer, block_size);
		size -= block_size;
	}
}

/************ MERGE FOR PAIRS ************/

void read_from_input(fstream* from, transition_pair *pair_array,
	                      int pair_offset, int pair_amount, 
	                      int start_offset) {
	from->seekg(start_offset + pair_offset * 2 * sizeof(qword));
	from->read((byte*) pair_array, pair_amount * sizeof(transition_pair));	
}

void write_to_output(fstream* to, transition_pair *pair_array,
	                      int pair_offset, int pair_amount,
	                      int start_offset) {
	to->seekp(start_offset + pair_offset * 2 * sizeof(qword));
	to->write((byte*) pair_array, pair_amount * sizeof(transition_pair));
}

void ext_merge_sort(int offset, int size) {
	int middle = offset + size / 2;
	int real_size = size, real_offset = offset;
	if (size <= ACCEPTABLE_SIZE) {
		read_from_input(&input, A, offset, size, INPUT_OFFSET);
		sort(A, A + size, current_comparator);
		write_to_output(&buffer, A, offset, size, OUTPUT_OFFSET);
		write_to_output(&output, A, offset, size, OUTPUT_OFFSET);
	} else {
		int startA = offset, sizeA = size / 2;
		int startB = middle, sizeB = size - size / 2;
		int blockSizeA = min(ACCEPTABLE_SIZE, sizeA);
		int blockSizeB = min(ACCEPTABLE_SIZE, sizeB);

		ext_merge_sort(startA, sizeA);
		ext_merge_sort(startB, sizeB);

		read_from_input(&buffer, A, startA, blockSizeA, OUTPUT_OFFSET);
		read_from_input(&buffer, B, startB, blockSizeB, OUTPUT_OFFSET);

		int l_index = 0;
	    int r_index = 0;
	    int current_index = 0;

	    while (current_index < size) {
	        if (r_index >= blockSizeB || blockSizeB == 0
	        	|| (l_index < blockSizeA && current_comparator(A[l_index], B[r_index]))) {
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
					read_from_input(&buffer, A, startA, blockSizeA, OUTPUT_OFFSET);
        		}
	        }
	        if (r_index == blockSizeB && blockSizeB != 0) {
        		r_index = 0;
        		startB += blockSizeB;
        		sizeB = max(0, sizeB - blockSizeB);
        		blockSizeB = min(ACCEPTABLE_SIZE, sizeB);
				if (blockSizeB != 0) {
					read_from_input(&buffer, B, startB, blockSizeB, OUTPUT_OFFSET);
				}
	        }
	        if (current_index == ACCEPTABLE_SIZE * 2) {
        		write_to_output(&output, BUFFER, offset, ACCEPTABLE_SIZE * 2, OUTPUT_OFFSET);
        		offset += ACCEPTABLE_SIZE * 2;
        		current_index = 0;
	        	size -= ACCEPTABLE_SIZE * 2;
	        }
		}
		write_to_output(&output, BUFFER, offset, size, OUTPUT_OFFSET);
		output.flush();
		copy_from_to(&output, &buffer, 
			real_offset * 2 * sizeof(qword), 
			real_size * 2 * sizeof(qword), 
			(byte*) BUFFER, 4 * ACCEPTABLE_SIZE * sizeof(qword));
	}
}

bool compare_pairs_by_first(const transition_pair& lhs, const transition_pair& rhs) {
  return lhs.first < rhs.first;
}

bool compare_pairs_by_second(const transition_pair& lhs, const transition_pair& rhs) {
  return lhs.second < rhs.second;
}

/************ MERGE FOR TRIPLES ************/

bool compare_triples_by_first(const transition_triple& lhs, const transition_triple& rhs) {
	return lhs.first < rhs.first;
	//return get<2>(lhs) < get<2>(rhs);
}

void read_from_input_triple(fstream* from, transition_triple *triple_array,
	                      int triple_offset, int triple_amount, 
	                      int start_offset) {
	from->seekg(start_offset + triple_offset * 3 * sizeof(qword));
	from->read((byte*) triple_array, 3 * triple_amount * sizeof(qword));	
	// qword buffer_triple[3] = {0, 0, 0};
	// for (int i = 0; i < triple_amount; i++) {
	// 	from->read((byte*)buffer_triple, 3 * sizeof(qword));	
	// 	triple_array[i] = make_tuple(buffer_triple[0], buffer_triple[1], buffer_triple[2]);
	// }
}

void write_to_output_triple(fstream* to, transition_triple *triple_array,
	                      int triple_offset, int triple_amount,
	                      int start_offset) {
	to->seekp(start_offset + triple_offset * 3 * sizeof(qword));
	to->write((byte*) triple_array, 3 * triple_amount * sizeof(qword));
	// for (int i = 0; i < triple_amount; i++) {
	// 	qword t = get<0>(triple_array[i]);
	// 	to->write((byte*) &t, sizeof(qword));
	// 	t = get<1>(triple_array[i]);
	// 	to->write((byte*) &t, sizeof(qword));
	// 	t = get<2>(triple_array[i]);
	// 	to->write((byte*) &t, sizeof(qword));
	// }
}

void ext_merge_sort_triple(int offset, int size) {
	int middle = offset + size / 2;
	int real_size = size, real_offset = offset;
	if (size <= ACCEPTABLE_SIZE) {
		read_from_input_triple(&input, A_FOR_TRIPLE, offset, size, INPUT_OFFSET);
		sort(A_FOR_TRIPLE, A_FOR_TRIPLE + size, current_comparator_for_triple);
		write_to_output_triple(&buffer, A_FOR_TRIPLE, offset, size, OUTPUT_OFFSET);
		write_to_output_triple(&output, A_FOR_TRIPLE, offset, size, OUTPUT_OFFSET);
	} else {
		int startA = offset, sizeA = size / 2;
		int startB = middle, sizeB = size - size / 2;
		int blockSizeA = min(ACCEPTABLE_SIZE, sizeA);
		int blockSizeB = min(ACCEPTABLE_SIZE, sizeB);

		ext_merge_sort_triple(startA, sizeA);
		ext_merge_sort_triple(startB, sizeB);

		read_from_input_triple(&buffer, A_FOR_TRIPLE, startA, blockSizeA, OUTPUT_OFFSET);
		read_from_input_triple(&buffer, B_FOR_TRIPLE, startB, blockSizeB, OUTPUT_OFFSET);

		int l_index = 0;
	    int r_index = 0;
	    int current_index = 0;

	    while (current_index < size) {
	        if (r_index >= blockSizeB || blockSizeB == 0
	        	|| (l_index < blockSizeA && current_comparator_for_triple(A_FOR_TRIPLE[l_index], B_FOR_TRIPLE[r_index]))) {
	            BUFFER_FOR_TRIPLE[current_index] = A_FOR_TRIPLE[l_index];
	            l_index++;
	        } else {
	        	BUFFER_FOR_TRIPLE[current_index] = B_FOR_TRIPLE[r_index];
	            r_index++;
	        }
	        current_index++;

	        if (l_index == blockSizeA && blockSizeA != 0) {
        		l_index = 0;
        		startA += blockSizeA;
        		sizeA = max(0, sizeA - blockSizeA);
        		blockSizeA = min(ACCEPTABLE_SIZE, sizeA);
        		if (blockSizeA != 0) {
					read_from_input_triple(&buffer, A_FOR_TRIPLE, startA, blockSizeA, OUTPUT_OFFSET);
        		}
	        }
	        if (r_index == blockSizeB && blockSizeB != 0) {
        		r_index = 0;
        		startB += blockSizeB;
        		sizeB = max(0, sizeB - blockSizeB);
        		blockSizeB = min(ACCEPTABLE_SIZE, sizeB);
				if (blockSizeB != 0) {
					read_from_input_triple(&buffer, B_FOR_TRIPLE, startB, blockSizeB, OUTPUT_OFFSET);
				}
	        }
	        if (current_index == ACCEPTABLE_SIZE * 2) {
        		write_to_output_triple(&output, BUFFER_FOR_TRIPLE, offset, ACCEPTABLE_SIZE * 2, OUTPUT_OFFSET);
        		offset += ACCEPTABLE_SIZE * 2;
        		current_index = 0;
	        	size -= ACCEPTABLE_SIZE * 2;
	        }
		}
		write_to_output_triple(&output, BUFFER_FOR_TRIPLE, offset, size, OUTPUT_OFFSET);
		//output.flush();
		copy_from_to(&output, &buffer, 
			real_offset * 3 * sizeof(qword), 
			real_size * 3 * sizeof(qword), 
			(byte*) BUFFER, 2 * ACCEPTABLE_SIZE * 3 * sizeof(qword));
	}
}

/*******************************************/

void merge_files_by_columns(int N, fstream* output) {
	fstream input_sorted_by_first;
	fstream input_sorted_by_second;
	input_sorted_by_first.open("input_sorted_by_first.bin", ios::binary | ios::out | ios::in);
	input_sorted_by_second.open("input_sorted_by_second.bin", ios::binary | ios::out | ios::in);
	input_sorted_by_first.seekg(0);
	input_sorted_by_second.seekp(0);

	output->seekp(0);

	int offset = 0;
	int buffer_size = ACCEPTABLE_SIZE;
	qword* triple = new qword[3];
	while (N > 0) {
		int block_size = min(buffer_size, 4000);// * 2 / 3;
		read_from_input(&input_sorted_by_second, A, offset, block_size, OUTPUT_OFFSET);
		read_from_input(&input_sorted_by_first, B, offset, block_size, OUTPUT_OFFSET);
		for (int i = 0; i < block_size; i++) {
			//cout << BUFFER[i].first << " " << BUFFER[i].second << " " << BUFFER[i + ACCEPTABLE_SIZE].second << endl;
			// triple[0] = A[i].first;
			// triple[1] = A[i].second;
			// triple[2] = B[i].second;
			*(((qword*) BUFFER) + 3 * i) = A[i].first;
			*(((qword*) BUFFER) + 3 * i + 1) = A[i].second;
			*(((qword*) BUFFER) + 3 * i + 2) = B[i].second;
			// output->write((byte*) triple, 3 * sizeof(qword));
		}
		output->write((byte*) BUFFER, block_size * 3 * sizeof(qword));
		offset += block_size;
		N -= block_size;
	}
}

int main() {
	//generate_input_bin();
	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &N, sizeof(int));

	/** Merge in triples **/

	A = new transition_pair[ACCEPTABLE_SIZE];
	B = new transition_pair[ACCEPTABLE_SIZE];
	BUFFER = new transition_pair[2 * ACCEPTABLE_SIZE];

	buffer.open("buffer.bin", ios::binary | ios::out | ios::in | ios::trunc);

	output.open("input_sorted_by_first.bin", ios::binary | ios::out | ios::in | ios::trunc);
	
	current_comparator = compare_pairs_by_first;
	ext_merge_sort(0, N);
	//input.close();
	//buffer.close();
	output.close();

	output.open("input_sorted_by_second.bin", ios::binary | ios::out | ios::in | ios::trunc);
	current_comparator = compare_pairs_by_second;
	ext_merge_sort(0, N);
	output.close();

	fstream merged;
	merged.open("merged.bin", ios::binary | ios::out | ios::in | ios::trunc);
	merge_files_by_columns(N, &merged);

	delete A, B, BUFFER;

	merged.close();
	input.close();

	/** Merge triples itself **/
	
	INPUT_OFFSET = 0;
	ACCEPTABLE_SIZE = ACCEPTABLE_SIZE * 2 / 3;
	
	input.open("merged.bin", ios::binary | ios::out | ios::in);
	output.open("output.bin", ios::binary | ios::out | ios::in | ios::trunc);

	A_FOR_TRIPLE = new transition_triple[ACCEPTABLE_SIZE];
	B_FOR_TRIPLE = new transition_triple[ACCEPTABLE_SIZE];
	BUFFER_FOR_TRIPLE = new transition_triple[2 * ACCEPTABLE_SIZE];

	current_comparator_for_triple = compare_triples_by_first;
	ext_merge_sort_triple(0, N);

	input.close();
	buffer.close();
	output.close();

	delete A_FOR_TRIPLE, B_FOR_TRIPLE, BUFFER_FOR_TRIPLE;
	
	return 0;
}