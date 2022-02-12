// Task: https://codeforces.com/problemset/problem/1572/F

#include <iostream>
#include <vector>


int main() {
    int cities_num = 0;
    int events_num = 0;

    std::cin >> cities_num >> events_num;

    struct Tower {
        int height = 0;
        int radius;
        int broadcast_count = 1;
    };

    std::vector<Tower> towers(cities_num);

    for (int i = 0; i < cities_num; ++i) {
        towers[i].radius = i;
    }

    int max_height = 0;

    while (events_num--) {
        int event_type = 0;
        std::cin >> event_type;

        switch (event_type) {
            case 1:
            {
                int city = 0;
                int new_radius = 0;

                std::cin >> city >> new_radius;

                city--;
                max_height++;
                
                towers[city].height = max_height;
                towers[city].radius = new_radius;

                for (int i = 0; i < city; ++i) {
                    if (city < towers[i].radius) {
                        for (int j = city; j < towers[i].radius; ++j) {
                            towers[j].broadcast_count--;
                        }

                        towers[i].radius = city;
                    }
                }

                for (int i = city + 1; i < new_radius; ++i) {
                    towers[i].broadcast_count++;
                }

                break;
            }

            case 2:
            {
                int left = 0;
                int right = 0;

                std::cin >> left >> right;

                int sum = 0;

                for (int city = left - 1; city < right; ++city) {
                    sum += towers[city].broadcast_count;
                }

                std::cout << sum << std::endl;

                break;
            }
        }
    }

    return 0;
}