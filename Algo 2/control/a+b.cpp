#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef char byte;

int ARRAY_SIZE = 200000;

fstream input;
fstream output;

byte* A;
byte* B;

void generate_input_bin() {
	output.open("input.bin", ios::binary | ios::out);

	int N = 5;
	byte B = 0;
	output.write((byte*) &N, sizeof(int));
	B = 4;
	output.write((byte*) &B, sizeof(byte));
	B = 4;
	output.write((byte*) &B, sizeof(byte));
	B = 4;
	output.write((byte*) &B, sizeof(byte));
	B = 4;
	output.write((byte*) &B, sizeof(byte));
	B = 4;
	output.write((byte*) &B, sizeof(byte));

	N = 5;
	output.write((byte*) &N, sizeof(int));
	B = 9;
	output.write((byte*) &B, sizeof(byte));
	B = 9;
	output.write((byte*) &B, sizeof(byte));
	B = 9;
	output.write((byte*) &B, sizeof(byte));
	B = 9;
	output.write((byte*) &B, sizeof(byte));
	B = 9;
	output.write((byte*) &B, sizeof(byte));

	output.close();
}

void read_from(fstream* from, int position, int amount, byte* target) {
	from->seekg(position);
	from->read(target, amount);	
}

void write_to(fstream* to, int position, int amount, byte* target) {
	to->seekp(position);
	to->write(target, amount);	
}

byte sum_with_first(byte* dst, byte* src, int dst_chunk_size, int src_chunk_size, 
	int amount, byte overflow) {
	for (int i = amount - 1; i >= 0; i--) {
		if (dst_chunk_size > 0 && src_chunk_size > 0) {
			dst[i] = dst[i] + src[i] + overflow;
			dst_chunk_size--; src_chunk_size--;
		} else if (dst_chunk_size > 0) {
			dst[i] = dst[i] + overflow;
			dst_chunk_size--;
		} else if (src_chunk_size > 0) {
			dst[i] = src[i] + overflow;
			src_chunk_size--;
		} else {
			break;
		}
		
		if (dst[i] > 9) {
			overflow = 1;
			dst[i] -= 10;
		} else {
			overflow = 0;
		}
	}

	return overflow;
}

int main() {
	//generate_input_bin();

	int a_len, b_len, len;

	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &a_len, sizeof(int));
	input.seekg(4 + a_len);
	input.read((byte*) &b_len, sizeof(int));

	A = new byte[ARRAY_SIZE];
	B = new byte[ARRAY_SIZE];
	len = max(a_len, b_len);

	int start_pos_A = 4;
	int end_pos_A = 4 + a_len;
	int start_pos_B = 4 + a_len + 4;
	int end_pos_B = 4 + a_len + 4 + b_len;
	
	fstream buffer;
	buffer.open("buffer.bin", ios::binary | ios::in | ios::out | ios::trunc);
	buffer.seekp(len - 1);
	buffer.write("", 1);
	buffer.seekp(0);

	int curr_pos_A = end_pos_A;
	int curr_pos_B = end_pos_B;

	byte overflow = 0;
	int buffer_pos = 0;
	int max_chunk_size = 0;
	while (buffer_pos < len) {
		int buf_pos_A = max(curr_pos_A - ARRAY_SIZE, start_pos_A);
		int buf_pos_B = max(curr_pos_B - ARRAY_SIZE, start_pos_B);

		int chuck_size_A = curr_pos_A - buf_pos_A;
		int chuck_size_B = curr_pos_B - buf_pos_B;
		if (chuck_size_A > 0) {
			read_from(&input, buf_pos_A, chuck_size_A, A + (ARRAY_SIZE - chuck_size_A));
		}
		if (chuck_size_B > 0) {
			read_from(&input, buf_pos_B, chuck_size_B, B + (ARRAY_SIZE - chuck_size_B));
		}

		max_chunk_size = max(chuck_size_A, chuck_size_B);
		overflow = sum_with_first(A, B, chuck_size_A, chuck_size_B, 
			ARRAY_SIZE, overflow);
		write_to(&buffer, buffer_pos, max_chunk_size, A + (ARRAY_SIZE - max_chunk_size));
		buffer_pos += max_chunk_size;
		
		curr_pos_A = buf_pos_A;
		curr_pos_B = buf_pos_B;
	}

	output.open("output.bin", ios::binary | ios::out);
	
	if (overflow == 1) {
		len++;
		output.write((byte*) &len, sizeof(int));
		output.write((byte*) &overflow, sizeof(byte));
		len--;
		buffer_pos = 5;
	} else {
		output.write((byte*) &len, sizeof(int));
		buffer_pos = 4;
	}

	read_from(&buffer, len - max_chunk_size, max_chunk_size, A);
	write_to(&output, buffer_pos, max_chunk_size, A);
	buffer_pos += max_chunk_size;
	len -= max_chunk_size;
	while (len > 0) {
		read_from(&buffer, len - ARRAY_SIZE, ARRAY_SIZE, A);
		write_to(&output, buffer_pos, ARRAY_SIZE, A);
		buffer_pos += ARRAY_SIZE;
		len -= ARRAY_SIZE;
	}
	
	delete A, B;

	return 0;
}