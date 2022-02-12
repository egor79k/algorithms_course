/*
    Задача: https://codeforces.com/problemset/problem/1374/E1

    Алиса и Боб прочитают каждую книгу вместе, чтобы быстрее закончить это
задание.

    В семейной библиотеке есть n книг. i-я книга характеризуется тремя целыми
числами: t_i — количество времени, которое Алиса и Боб должны потратить, чтобы
прочитать ее, a_i (равное 1, если Алисе нравится i-я книга, и 0, если не
нравится), и b_i (равное 1, если Бобу нравится i-я книга, и 0, если не нравится).

    Поэтому им нужно выбрать какие-то книги из имеющихся n книг таким образом, что:
- Алисе нравятся не менее k книг из выбранного множества и Бобу нравятся не
менее k книг из выбранного множества; общее время, затраченное на прочтение
этих книг минимизировано.

    Множество, которое они выбирают, одинаковое и для Алисы и для Боба (они
читают одни и те же книги), и они читают все книги вместе, таким образом,
суммарное время чтения равно сумме t_i по всем книгам, которые находятся в
выбранном множестве.

    Ваша задача — помочь им и найти любое подходящее множество книг или
определить, что такое множество найти невозможно.
*/

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>


int main() {
    int books_num = 0; // n
    int interesting_books_num = 0; // k
    std::cin >> books_num >> interesting_books_num;

    // Массивы длительностей чтения
    std::vector<int> read_time[4];

    int time = 0;
    int alice_likes = 0;
    int bob_likes = 0;

    // Разделим все книги на 4 группы:
    // [0] x 0 0 - не нравится А и Б
    // [1] x 1 0 - нравится А, не нравится Б
    // [2] x 0 1 - не нравится A, нравится Б
    // [3] x 1 1 - нравится А и Б

    for (int i = 0; i < books_num; ++i) {
        std::cin >> time >> alice_likes >> bob_likes;
        read_time[2 * alice_likes + bob_likes].push_back(time);
    }

    std::vector<int> p_sum[4];

    // Считаем префиксные суммы массивов отсортированных длительностей чтения
    for (int group = 0; group < 4; ++group) {
        std::sort(read_time[group].begin(), read_time[group].end());

        p_sum[group].resize(read_time[group].size() + 1);
        p_sum[group][0] = 0;

        for (int i = 0; i < read_time[group].size(); ++i) {
            p_sum[group][i + 1] += p_sum[group][i] + read_time[group][i];
        }
    }

    const int MAX_TIME = std::numeric_limits<int>::max();
    int min_time = MAX_TIME;
    int new_min_time = 0;
    int one_like = 0;

    // Ищем перебором оптимальное количество книг, которые нравятся и Алисе и Бобу
    for (int ab_like = 0; ab_like < p_sum[3].size(); ++ab_like) {
        // Число книг, интересных только одному из детей
        one_like = interesting_books_num - ab_like;

        if (one_like >= 0 && one_like < p_sum[1].size() && one_like < p_sum[2].size()) {
            // Число книг интересных и А и Б + только А + только Б
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