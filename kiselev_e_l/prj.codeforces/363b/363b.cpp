// Task: https://codeforces.com/contest/363/problem/B

#include <iostream>


int main() {
	int planks_num = 0;
	int piano_width = 0;

	std::cin >> planks_num >> piano_width;
	
	// Allocating memory
	int *fence = new int[planks_num];
	
	// Reading array
	for (int i = 0; i < planks_num; ++i)
		std::cin >> fence[i];

	int sum = 0;

	// Init summ
	for (int i = 0; i < piano_width; ++i)
		sum += fence[i];

	int min_sum = sum;
	int min_id = 0;

	for (int i = 0; i < planks_num - piano_width; ++i) {
		// Moving summ
		sum -= fence[i];
		sum += fence[i + piano_width];

		if (sum < min_sum) {
			min_sum = sum;
			min_id = i + 1;
		}
	}

	// Deallocating memory
	delete[] fence;

	// Printing result
	std::cout << min_id + 1;

	return 0;
}