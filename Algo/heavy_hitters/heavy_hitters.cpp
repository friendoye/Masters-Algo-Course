#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

class HeavyHittersCounter {
public:
	HeavyHittersCounter(int k) {
		target_amount = k;
	}

	void accept(int value) {
		auto result_iterator = heavy_hitters_map.find(value);
		if (result_iterator != heavy_hitters_map.end()) {
			result_iterator->second++;
		} else {
			heavy_hitters_map.insert(make_pair(value, 1));
			if (heavy_hitters_map.size() == target_amount) {
				reduce_rank();	
			}
		}
	}

	void print_heavy_hitters() {
		int min_print_values = target_amount - 1;
		auto map_iterator = heavy_hitters_map.begin();
		while (map_iterator != heavy_hitters_map.end()) {
			cout << map_iterator->first << " ";
			map_iterator++;
			min_print_values--;
		}
		while (min_print_values-- > 0) {
			cout << "0 ";
		}
		cout << endl;
	}

private:
	static bool min_value_predicate(const pair<int, int>& left, const pair<int, int>& right) {
        return left.second < right.second;
    }

	int get_min_rank() {
		return min_element(heavy_hitters_map.begin(), 
			               heavy_hitters_map.end(), 
			               HeavyHittersCounter::min_value_predicate)->second;
	}

	void reduce_rank() {
		auto map_iterator = heavy_hitters_map.begin();
		auto min_rank = get_min_rank();
		while (map_iterator != heavy_hitters_map.end()) {
			map_iterator->second -= min_rank;
			if (map_iterator->second == 0) {
				map_iterator = heavy_hitters_map.erase(map_iterator);
			} else {
				map_iterator++;	
			}
		}
	}

	int target_amount;
	map<int, int> heavy_hitters_map;
};

int main() {
	int k;
	
	//ifstream input;
	//input.open("input.txt", ios::in);
	cin >> k;

	HeavyHittersCounter counter = HeavyHittersCounter(k);
	
	int value;
	cin >> value;
	while (value != -1) {
		if (value > 0) {
			counter.accept(value);
		} else if (value == 0) {
			counter.print_heavy_hitters();
		}
		cin >> value;
	}
	
	//input.close();

	return 0;
}