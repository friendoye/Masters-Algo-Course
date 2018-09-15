#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <iomanip>

using namespace std;

ifstream input;
ofstream output;

class BorWithSuffixLinks {
private:	
	struct vertex {
		map<char, int> nexts;
		map<char, int> suffix_nexts;
		bool isLeaf;
		bool isTerminant;
		char edge_char;
		int index;
		int parent;
		int suffix_link;
		int term_cont;

		vertex() {
			index = -1;
			parent = -1;
			suffix_link = -1;
			term_cont = 0;
			isLeaf = false;
			isTerminant = false;
		}
	};

	vector<vertex*> vertexes;
	vector<vertex*> terminant_vertexes;

	int navigate_in_map(const map<char, int> & link_map, char c) {
		auto map_iter = link_map.find(c);
		if (map_iter != link_map.end()) {
			return map_iter->second;
		} else {
			return -1;
		}	
	}

	int find_suffix_link(vertex* vert) {
		if (vert->suffix_link == -1) {
			if (vert->index == 0 || vert->parent == 0) {
				vert->suffix_link = 0;
			} else {
				auto parent_vert = vertexes[vert->parent];
				auto parent_suffix_link = find_suffix_link(parent_vert);
				vert->suffix_link = iterate_suffix_link(
					vertexes[parent_suffix_link], vert->edge_char
				);
			}
		}
		return vert->suffix_link;
	}

	int iterate_suffix_link(vertex* vert, char last_char) {
		int next_suffix_index = navigate_in_map(vert->suffix_nexts, last_char);
		if (next_suffix_index == -1) {
			int next_index = navigate_in_map(vert->nexts, last_char);
			if (next_index != -1) {
				vert->suffix_nexts[last_char] = next_index;
			} else {
				if (vert->index == 0) {
					vert->suffix_nexts[last_char] = 0;
				} else {
					auto parent_suffix_link = find_suffix_link(vert);
					vert->suffix_nexts[last_char] = iterate_suffix_link(
						vertexes[parent_suffix_link], last_char
					);
				}
			}
		}
		return vert->suffix_nexts[last_char];
	}

	void print_prefix(vertex* vert) {
		if (vert->index == 0) {
			return;
		}
		print_prefix(vertexes[vert->parent]);
		cout << vert->edge_char << vert->term_cont << "[" << vert->index << "]";
	}

public:
	BorWithSuffixLinks() {
		vertex* root = new vertex();
		root->index = 0;
		root->parent = 0;
		vertexes.push_back(root);
	}

	void add_string(string word) {
		vertex* curr_v = vertexes[0];
		int curr_index = 0;

		for (char & c : word) {
			int index = navigate_in_map(curr_v->nexts, c);
			if (index != -1) {
				curr_index = index;
				curr_v = vertexes[curr_index];
			} else {
				vertex * new_v = new vertex();
				new_v->edge_char = c;
				new_v->parent = curr_index;
				new_v->index = vertexes.size();
				vertexes.push_back(new_v);

				curr_index = new_v->index;
				curr_v->nexts[c] = curr_index;
				curr_v = new_v;

				// cout << "new index: " << curr_v->index << endl;
			}
			// cout << "map len: " << curr_v->nexts.size() << endl;
		}

		curr_v->isLeaf = curr_v->nexts.size() == 0;
		curr_v->isTerminant = true;
		terminant_vertexes.push_back(curr_v);
		//cout << "vertexes amount: " << vertexes.size() << endl;
	}

	void build_suffix_links() {
		queue<int> q;
		q.push(0);
		
		vertex* curr_v = NULL;
		while (!q.empty()) {
			int v_index = q.front(); q.pop();
			
			curr_v = vertexes[v_index];
			find_suffix_link(curr_v);

			for (auto & p : vertexes[v_index]->nexts) {
				q.push(p.second);
			}
		}
	}

	void compute_max_terms() {
		queue<int> q;
		q.push(0);
		
		vertex* curr_v = NULL;
		while (!q.empty()) {
			int v_index = q.front(); q.pop();

			curr_v = vertexes[v_index];
			vertex* parent_v = vertexes[curr_v->parent];
			vertex* suffix_v = vertexes[curr_v->suffix_link];
			curr_v->term_cont = max(parent_v->term_cont, suffix_v->term_cont);
			if (curr_v->isTerminant) {
				curr_v->term_cont += 1;
			}

			for (auto & p : vertexes[v_index]->nexts) {
				q.push(p.second);
			}
		}
	}

	void print_bor() {
		queue<int> q;
		queue<int> level;
		q.push(0);
		level.push(0);
		int current_level = 0;

		cout << "root" << endl;
		while (!q.empty()) {
			int v_index = q.front(); q.pop();
			int level_index = level.front(); level.pop();
			
			if (level_index > current_level) {
				current_level = level_index;
				cout << endl;
			}

			for (auto & p : vertexes[v_index]->nexts) {
				q.push(p.second);
				level.push(level_index + 1);
				cout << p.first << " ";
			}
		}
	}

	void print_max_terms() {
		for (auto & vert : terminant_vertexes) {
			cout << "(" << vert->edge_char << ") " << vert->term_cont << " ";
		}
		cout << endl;
	}

	int get_max_terms() {
		int max_terms = 0;
		for (auto & vert : terminant_vertexes) {
			if (vert->isLeaf) {
				max_terms = max(max_terms, vert->term_cont);	
			}
		}
		return max_terms;
	}
};

int main() {
	cout << boolalpha; 

	int words_amount = 0;

	input.open("input.txt");
	input >> words_amount;
	input.ignore(1);
		
	BorWithSuffixLinks bor = BorWithSuffixLinks();
	string word;
	while (words_amount-- > 0) {
		getline(input, word);
		// cout << word << endl;
		bor.add_string(word);
	}

	bor.build_suffix_links();
	bor.compute_max_terms();
	//bor.print_max_terms();
	//bor.print_bor();
	
	output.open("output.txt");
	output << bor.get_max_terms();
	output.close();
	
	return 0;
}