// Task: https://codeforces.com/problemset/problem/1374/E1

#include <algorithm>
#include <iostream>
#include <vector>


int main() {
	int books_num = 0; // n
	int interesting_books_num = 0; // k
	std::cin >> books_num >> interesting_books_num;


	std::vector<int> read_time[4];

	int time = 0;
	int alice_likes = 0;
	int bob_likes = 0;

	// Divide all books into 4 groups:
	// [0] x 0 0 - A and B don't like
	// [1] x 1 0 - A likes, B doesn't like
	// [2] x 0 1 - A doesn't like, B likes
	// [3] x 1 1 - A and B like
	for (int i = 0; i < books_num; ++i) {
		std::cin >> time >> alice_likes >> bob_likes;
		read_time[2 * alice_likes + bob_likes].push_back(time);
	}

	std::vector<int> p_sum[4];

	// Counting prefix summs of sorted reading times
	for (int group = 0; group < 4; ++group) {
		std::sort(read_time[group].begin(), read_time[group].end());

		p_sum[group].resize(read_time[group].size() + 1);
		p_sum[group][0] = 0;

		for (int i = 0; i < read_time[group].size(); ++i) {
			p_sum[group][i + 1] += p_sum[group][i] + read_time[group][i];
		}
	}

	const int MAX_TIME = 2e9 + 1;
	int min_time = MAX_TIME;
	int new_min_time = 0;
	int one_like = 0;

	// Find optimal books num, which Alice and Bob like together
	for (int ab_like = 0; ab_like < p_sum[3].size(); ++ab_like) {
		// Number of books interesting for one of children
		one_like = interesting_books_num - ab_like;

		if (one_like >= 0 && one_like < p_sum[1].size() && one_like < p_sum[2].size()) {
			// Count of books interesting for A & B + just for A + just for B 
			new_min_time = p_sum[3][ab_like] + p_sum[2][one_like] + p_sum[1][one_like];

			if (new_min_time < min_time)
				min_time = new_min_time;
		}
	}

	if (min_time != MAX_TIME)
		std::cout << min_time;
	else
		std::cout << -1;

	return 0;
}