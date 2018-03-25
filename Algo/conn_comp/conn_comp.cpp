#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef char byte;

int N, M;

ifstream input;
ofstream output;

const int EDGES_PER_BLOCK = 10000;
int * EDGES;

class ComponentsCounter {
public:
	ComponentsCounter(int _vertex_amount) {
		vertex_parent = new int[_vertex_amount]();
		vertex_amount = _vertex_amount;
		component_number = _vertex_amount;
		while (_vertex_amount-- > 0) {
			vertex_parent[_vertex_amount] = _vertex_amount;
		}
	}

	virtual ~ComponentsCounter() {
		delete vertex_parent;
	}

	void check_edge(int a, int b) {
		a = a - 1; b = b - 1;
		
		if (a == b) {
			return;
		}

		int p_a = find_root(a);
		int p_b = find_root(b);

		if (p_a == p_b) {
			return;
		} 

		int vertex_size_p_a = find_length_to_root(a);
		int vertex_size_p_b = find_length_to_root(b);
		if (vertex_size_p_a > vertex_size_p_b) {
			vertex_parent[p_b] = p_a;
		} else {
			vertex_parent[p_a] = p_b;
		}
		
		component_number--;
	}

	int get_components_amount() {
		return component_number;
	}

private:
	int find_root(int x) {
		while (vertex_parent[x] != x) {
			x = vertex_parent[x];
		}
		return x;
	}

	int find_length_to_root(int x) {
		int length = 0;
		while (vertex_parent[x] != x) {
			x = vertex_parent[x];
			length++;
		}
		return length;
	}

	int *vertex_parent;
	int vertex_amount;
	int component_number;
};

void generate_input_bin() {
	int N = 3, M = 4;
	output.open("input.bin", ios::binary | ios::out);
	int array[8] = {4, 3, 1, 2, 2, 3, 2, 3};
	output.write((byte *) &array, 32);

	output.close();
}

int main() {
	//generate_input_bin();

	input.open("input.bin", ios::binary | ios::in);
	input.read((byte*) &N, sizeof(int));
	input.read((byte*) &M, sizeof(int));

	ComponentsCounter comp_counter = ComponentsCounter(N);
	EDGES = new int[EDGES_PER_BLOCK * 2];

	output.open("output.bin", ios::binary | ios::out);

	while (M > 0) {
		int clamp = min(EDGES_PER_BLOCK, M);
		
		input.read((byte*) EDGES, clamp * 2 * sizeof(int));
		
		for (int i = 0; i < clamp; i++) {
			comp_counter.check_edge(EDGES[2 * i], EDGES[2 * i + 1]);
		}

		M -= EDGES_PER_BLOCK;
	}

	M = comp_counter.get_components_amount();
	cout << M << endl;
	output.write((byte*) &M, sizeof(int));

	output.close();
	input.close();

	return 0;
}